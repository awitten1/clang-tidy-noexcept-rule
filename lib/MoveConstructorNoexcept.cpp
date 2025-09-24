
#include "MoveConstructorNoexcept.h"

using namespace clang::ast_matchers;
using namespace clang;

void WARFinder::registerMatchers(
    clang::ast_matchers::MatchFinder *Finder) {
  Finder->addMatcher(functionDecl(isDefinition()).bind("funcDef"), this);
}

void WARFinder::check(
    const clang::ast_matchers::MatchFinder::MatchResult &Result) {
  // Get the matched FunctionDecl node
  const auto *MatchedDecl = Result.Nodes.getNodeAs<FunctionDecl>("funcDef");

  // If the node is null or not in the main file, ignore it
  if (!MatchedDecl || !Result.SourceManager->isInMainFile(MatchedDecl->getLocation()))
    return;

  // Create a diagnostic note at the function's location to show it was found
  diag(MatchedDecl->getLocation(), "Found function definition: %0")
      << MatchedDecl->getNameAsString();
}
