
#include "MoveConstructorNoexcept.h"

using namespace clang::ast_matchers;
using namespace clang;

class WARVisitor : public RecursiveASTVisitor<WARVisitor> {
public:
  explicit WARVisitor(WARFinder *Check) : Check(Check) {}

  // This function is called for every binary operator, including '='
  bool VisitBinaryOperator(BinaryOperator *BO) {
    // We only care about simple assignment expressions for this example
    if (BO->getOpcode() != BO_Assign)
      return true;

    Expr *LHS = BO->getLHS()->IgnoreParenImpCasts(); // The variable being written to
    Expr *RHS = BO->getRHS(); // The expression being read from

    // --- Step 1: Traverse the RHS to find all variables being read ---
    // A small, inner visitor to find all DeclRefExprs (variable references)
    class ReadVarVisitor : public RecursiveASTVisitor<ReadVarVisitor> {
    public:
      llvm::DenseSet<const VarDecl *> &ReadVars;
      ReadVarVisitor(llvm::DenseSet<const VarDecl *> &Vars) : ReadVars(Vars) {}
      bool VisitDeclRefExpr(DeclRefExpr *DRE) {
        if (const auto *VD = dyn_cast<VarDecl>(DRE->getDecl())) {
          ReadVars.insert(VD);
        }
        return true;
      }
    };

    ReadVarVisitor RHSVisitor(ReadVars);
    RHSVisitor.TraverseStmt(RHS);

    // --- Step 2: Analyze the LHS to find the variable being written to ---
    if (const auto *DRE = dyn_cast<DeclRefExpr>(LHS)) {
      if (const auto *WrittenVar = dyn_cast<VarDecl>(DRE->getDecl())) {
        // --- Step 3: Check for WAR dependency ---
        if (ReadVars.count(WrittenVar)) {
          Check->diag(BO->getBeginLoc(),
                      "Write-After-Read (WAR) dependency detected on variable '%0'")
              << WrittenVar->getName();
        }
      }
    }

    return true; // Continue traversal
  }

private:
  WARFinder *Check;
  llvm::DenseSet<const VarDecl *> ReadVars;
};


// --- Main Check Logic ---

void WARFinder::registerMatchers(MatchFinder *Finder) {
  // We start by finding every function definition
  Finder->addMatcher(functionDecl(isDefinition()).bind("func"), this);
}

void WARFinder::check(const MatchFinder::MatchResult &Result) {
  const auto *Func = Result.Nodes.getNodeAs<FunctionDecl>("func");

  // Don't analyze functions in system headers
  if (!Func || !Func->hasBody() || !Result.SourceManager->isInMainFile(Func->getLocation()))
    return;

  // For each function, create a new visitor and traverse its body.
  // The visitor will handle the logic and call back to `diag` to report errors.
  WARVisitor Visitor(this);
  Visitor.TraverseStmt(Func->getBody());
}