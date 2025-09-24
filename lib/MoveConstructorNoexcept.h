
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

class MatchCallbackImpl
    : public clang::ast_matchers::MatchFinder::MatchCallback {
  void
  run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override {
    std::cout << "Found a match" << std::endl;
  }
};

namespace MyNameSpace {}

template <typename T>
class Matcher
    : public clang::ast_matchers::internal::SingleNodeMatcherInterface<T> {};

class MoveConstructorNoexcepts : public clang::tidy::ClangTidyCheck {
public:
  MoveConstructorNoexcepts(clang::StringRef Name,
                           clang::tidy::ClangTidyContext *Context)
      : clang::tidy::ClangTidyCheck(Name, Context) {}

  void registerMatchers(clang::ast_matchers::MatchFinder *Finder) override;

  void
  check(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
};

class MoveConstructorNoexceptModule : public clang::tidy::ClangTidyModule {
public:
  void addCheckFactories(
      clang::tidy::ClangTidyCheckFactories &CheckFactories) override {
    CheckFactories.registerCheck<MoveConstructorNoexcepts>(
        "move-constructor-noexceptasdf");
  }
};

static clang::tidy::ClangTidyModuleRegistry::Add<MoveConstructorNoexceptModule>
    X("move-constructor-noexcept", "Adds my lint checks.");
