#pragma once

#include <vector>

#include "Model.h"
#include "Token.h"

namespace pinky {

class Parser {
public:
  explicit Parser(std::vector<Token> tokens, std::size_t current = 0);

  Token Advance();

  Token Peek() const;

  bool IsNext(TokenType expectedType) const;

  Token Expect(TokenType expectedType);

  Token PreviousToken() const;

  bool Match(TokenType expectedType);

  ExprPtr Primary();

  ExprPtr Exponent();

  ExprPtr Unary();

  ExprPtr Modulo();

  ExprPtr Multiplication();

  ExprPtr Addition();

  ExprPtr Comparison();

  ExprPtr Equality();

  ExprPtr LogicalAnd();

  ExprPtr LogicalOr();

  ExprPtr Expr();

  std::unique_ptr<pinky::PrintStmt> PrintStmt(std::string end);

  std::unique_ptr<pinky::IfStmt> IfStmt();

  std::unique_ptr<pinky::WhileStmt> WhileStmt();

  std::unique_ptr<pinky::ForStmt> ForStmt();

  std::vector<ExprPtr> Args();

  std::vector<ParamPtr> Params();

  std::unique_ptr<pinky::FuncDecl> FuncDecl();

  std::unique_ptr<pinky::RetStmt> RetStmt();

  std::unique_ptr<pinky::LocalAssignment> LocalAssign();

  StmtPtr Stmt();

  std::unique_ptr<pinky::Stmts> Stmts();

  std::unique_ptr<pinky::Stmts> Program();

  std::unique_ptr<pinky::Stmts> Parse();

private:
  friend struct ParserTestAccess;

  std::vector<Token> Tokens;
  std::size_t Current = 0;
};
} // namespace pinky