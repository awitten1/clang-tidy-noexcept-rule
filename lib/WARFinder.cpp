
#include "WARFinder.h"

using namespace clang::ast_matchers;
using namespace clang;

class WARVisitor : public RecursiveASTVisitor<WARVisitor> {
public:
  explicit WARVisitor(WARFinder *check) : check_(check) {}

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

  bool VisitBinaryOperator(BinaryOperator *BO) {
    if (BO->getOpcode() != BO_Assign)
      return true;

    Expr *LHS = BO->getLHS()->IgnoreParenImpCasts();
    Expr *RHS = BO->getRHS();

    ReadVarVisitor RHSVisitor(read_vars_);
    RHSVisitor.TraverseStmt(RHS);

    if (const auto *DRE = dyn_cast<DeclRefExpr>(LHS)) {
      if (const auto *WrittenVar = dyn_cast<VarDecl>(DRE->getDecl())) {
        if (read_vars_.count(WrittenVar)) {
          check_->diag(BO->getBeginLoc(),
                      "WAR dependency detected on variable '%0'")
              << WrittenVar->getName();
        }
      }
    }

    return true;
  }

private:
  WARFinder *check_;
  llvm::DenseSet<const VarDecl *> read_vars_;
};



void WARFinder::registerMatchers(MatchFinder *Finder) {
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