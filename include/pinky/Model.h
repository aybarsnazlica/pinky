#pragma once

#include "pinky/Token.h"

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace pinky {

class Node {
public:
  virtual ~Node();
  virtual std::string ToString() const = 0;
};

class Expr : public Node {};

using ExprPtr = std::unique_ptr<Expr>;

class Stmt : public Node {};

using StmtPtr = std::unique_ptr<Stmt>;

class Decl : public Stmt {};

class Integer : public Expr {
public:
  Integer(int value, int line);

  std::string ToString() const override;

  int Value;
  int Line;
};

class Float : public Expr {
public:
  Float(double value, int line);

  std::string ToString() const override;

  double Value;
  int Line;
};

class Bool : public Expr {
public:
  Bool(bool value, int line);

  std::string ToString() const override;

  bool Value;
  int Line;
};

class String : public Expr {
public:
  String(std::string value, int line);

  std::string ToString() const override;

  std::string Value;
  int Line;
};

class UnOp : public Expr {
public:
  UnOp(Token op, ExprPtr operand, int line);

  std::string ToString() const override;

  Token Op;
  ExprPtr Operand;
  int Line;
};

class BinOp : public Expr {
public:
  BinOp(Token op, ExprPtr left, ExprPtr right, int line);

  std::string ToString() const override;

  Token Op;
  ExprPtr Left;
  ExprPtr Right;
  int Line;
};

class LogicalOp : public Expr {
public:
  LogicalOp(Token op, ExprPtr left, ExprPtr right, int line);

  std::string ToString() const override;

  Token Op;
  ExprPtr Left;
  ExprPtr Right;
  int Line;
};

class Grouping : public Expr {
public:
  Grouping(ExprPtr value, int line);

  std::string ToString() const override;

  ExprPtr Value;
  int Line;
};

class Identifier : public Expr {
public:
  Identifier(std::string name, int line);

  std::string ToString() const override;

  std::string Name;
  int Line;
};

class Stmts : public Node {
public:
  Stmts(std::vector<StmtPtr> stmts, int line);

  std::string ToString() const override;

  std::vector<StmtPtr> Statements;
  int Line;
};

class PrintStmt : public Stmt {
public:
  PrintStmt(ExprPtr value, std::string end, int line);

  std::string ToString() const override;

  ExprPtr Value;
  std::string End;
  int Line;
};

class IfStmt : public Stmt {
public:
  IfStmt(ExprPtr test, std::unique_ptr<Stmts> thenStmts,
         std::unique_ptr<Stmts> elseStmts, int line);

  std::string ToString() const override;

  ExprPtr Test;
  std::unique_ptr<Stmts> ThenStmts;
  std::unique_ptr<Stmts> ElseStmts;
  int Line;
};

class WhileStmt : public Stmt {
public:
  WhileStmt(ExprPtr test, std::unique_ptr<Stmts> bodyStmts, int line);

  std::string ToString() const override;

  ExprPtr Test;
  std::unique_ptr<Stmts> BodyStmts;
  int Line;
};

class Assignment : public Stmt {
public:
  Assignment(ExprPtr left, ExprPtr right, int line);

  std::string ToString() const override;

  ExprPtr Left;
  ExprPtr Right;
  int Line;
};

class LocalAssignment : public Stmt {
public:
  LocalAssignment(ExprPtr left, ExprPtr right, int line);

  std::string ToString() const override;

  ExprPtr Left;
  ExprPtr Right;
  int Line;
};

class ForStmt : public Stmt {
public:
  ForStmt(std::unique_ptr<Identifier> ident, ExprPtr start, ExprPtr end,
          ExprPtr step, std::unique_ptr<Stmts> bodyStmts, int line);

  std::string ToString() const override;

  std::unique_ptr<Identifier> Ident;
  ExprPtr Start;
  ExprPtr End;
  ExprPtr Step;
  std::unique_ptr<Stmts> BodyStmts;
  int Line;
};

class Param : public Decl {
public:
  Param(std::string name, int line);

  std::string ToString() const override;

  std::string Name;
  int Line;
};

using ParamPtr = std::unique_ptr<Param>;

class FuncDecl : public Decl {
public:
  FuncDecl(std::string name, std::vector<ParamPtr> params,
           std::unique_ptr<Stmts> bodyStmts, int line);

  std::string ToString() const override;

  std::string Name;
  std::vector<ParamPtr> Params;
  std::unique_ptr<Stmts> BodyStmts;
  int Line;
};

class FuncCall : public Expr {
public:
  FuncCall(std::string name, std::vector<ExprPtr> args, int line);

  std::string ToString() const override;

  std::string Name;
  std::vector<ExprPtr> Args;
  int Line;
};

class FuncCallStmt : public Stmt {
public:
  explicit FuncCallStmt(std::unique_ptr<FuncCall> expr);

  std::string ToString() const override;

  std::unique_ptr<FuncCall> ExprValue;
};

class RetStmt : public Stmt {
public:
  RetStmt(ExprPtr value, int line);

  std::string ToString() const override;

  ExprPtr Value;
  int Line;
};

std::ostream &operator<<(std::ostream &stream, const Node &node);

} // namespace pinky