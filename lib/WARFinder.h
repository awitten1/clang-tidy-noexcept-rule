
#pragma once

#include <iostream>

#include <clang-tidy/ClangTidyCheck.h>
#include <clang-tidy/ClangTidyModule.h>
#include <clang-tidy/ClangTidyModuleRegistry.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/ASTMatchers/ASTMatchers.h>
#include <clang/ASTMatchers/ASTMatchersInternal.h>
#include <clang/ASTMatchers/ASTMatchFinder.h>
#include <clang/AST/RecursiveASTVisitor.h>

class WARFinder : public clang::tidy::ClangTidyCheck {
public:
  WARFinder(clang::StringRef Name,
                           clang::tidy::ClangTidyContext *Context)
      : clang::tidy::ClangTidyCheck(Name, Context) {}

  void registerMatchers(clang::ast_matchers::MatchFinder *Finder) override;

  void
  check(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};

class WARFinderModule : public clang::tidy::ClangTidyModule {
public:
  void addCheckFactories(
      clang::tidy::ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<WARFinder>(
        "move-constructor-noexceptasdf");
  }
};

static clang::tidy::ClangTidyModuleRegistry::Add<WARFinderModule>
    X("move-constructor-noexcept", "Adds my lint checks.");
