#include "Model.h"
#include "TestFramework.h"

#include <memory>

namespace {

using pinky::Assignment;
using pinky::BinOp;
using pinky::Bool;
using pinky::ExprPtr;
using pinky::Float;
using pinky::ForStmt;
using pinky::FuncCall;
using pinky::FuncCallStmt;
using pinky::FuncDecl;
using pinky::Identifier;
using pinky::IfStmt;
using pinky::Integer;
using pinky::LocalAssignment;
using pinky::LogicalOp;
using pinky::Param;
using pinky::ParamPtr;
using pinky::PrintStmt;
using pinky::RetStmt;
using pinky::StmtPtr;
using pinky::Stmts;
using pinky::String;
using pinky::Token;
using pinky::TokenType;
using pinky::UnOp;
using pinky::WhileStmt;
using pinky::test::ExpectEqual;

void TestLiteralNodesToString() {
  ExpectEqual(Integer(17, 3).ToString(), std::string("Integer[17]"),
              "Integer ToString mismatch");
  ExpectEqual(Float(3.5, 3).ToString(), std::string("Float[3.5]"),
              "Float ToString mismatch");
  ExpectEqual(Bool(true, 3).ToString(), std::string("Bool[true]"),
              "Bool ToString mismatch");
  ExpectEqual(String("hello", 3).ToString(), std::string("String[hello]"),
              "String ToString mismatch");
  ExpectEqual(Identifier("value", 3).ToString(),
              std::string("Identifier[\"value\"]"),
              "Identifier ToString mismatch");
}

void TestExpressionNodesToString() {
  UnOp negate(Token{TokenType::Minus, "-", 2}, std::make_unique<Integer>(17, 2),
              2);
  ExpectEqual(negate.ToString(), std::string("UnOp(\"-\", Integer[17])"),
              "UnOp ToString mismatch");

  BinOp sum(Token{TokenType::Plus, "+", 2},
            std::make_unique<Identifier>("x", 2),
            std::make_unique<Integer>(1, 2), 2);
  ExpectEqual(sum.ToString(),
              std::string("BinOp(\"+\", Identifier[\"x\"], Integer[1])"),
              "BinOp ToString mismatch");

  LogicalOp logicalAnd(Token{TokenType::And, "and", 2},
                       std::make_unique<Bool>(true, 2),
                       std::make_unique<Bool>(false, 2), 2);
  ExpectEqual(logicalAnd.ToString(),
              std::string("LogicalOp(\"and\", Bool[true], Bool[false])"),
              "LogicalOp ToString mismatch");
}

void TestStatementAndDeclarationNodesToString() {
  PrintStmt print(std::make_unique<String>("hello", 5), "\n", 5);
  ExpectEqual(print.ToString(),
              std::string("PrintStmt(String[hello], end=\"\\n\")"),
              "PrintStmt ToString mismatch");

  std::vector<StmtPtr> thenStatements;
  thenStatements.push_back(std::make_unique<PrintStmt>(
      std::make_unique<String>("then", 6), "\n", 6));

  std::vector<StmtPtr> elseStatements;
  elseStatements.push_back(
      std::make_unique<PrintStmt>(std::make_unique<String>("else", 7), "", 7));

  IfStmt ifStmt(std::make_unique<Bool>(true, 5),
                std::make_unique<Stmts>(std::move(thenStatements), 6),
                std::make_unique<Stmts>(std::move(elseStatements), 7), 5);
  ExpectEqual(
      ifStmt.ToString(),
      std::string(
          "IfStmt(Bool[true], then:Stmts([PrintStmt(String[then], "
          "end=\"\\n\")]), else:Stmts([PrintStmt(String[else], end=\"\")]))"),
      "IfStmt ToString mismatch");

  std::vector<StmtPtr> bodyStatements;
  bodyStatements.push_back(
      std::make_unique<Assignment>(std::make_unique<Identifier>("x", 9),
                                   std::make_unique<Integer>(1, 9), 9));
  WhileStmt whileStmt(std::make_unique<Bool>(false, 9),
                      std::make_unique<Stmts>(std::move(bodyStatements), 9), 9);
  ExpectEqual(
      whileStmt.ToString(),
      std::string("WhileStmt(Bool[false], Stmts([Assignment(Identifier[\"x\"], "
                  "Integer[1])]))"),
      "WhileStmt ToString mismatch");

  std::vector<StmtPtr> forBodyStatements;
  forBodyStatements.push_back(
      std::make_unique<LocalAssignment>(std::make_unique<Identifier>("y", 11),
                                        std::make_unique<Integer>(2, 11), 11));
  ForStmt forStmt(
      std::make_unique<Identifier>("i", 11), std::make_unique<Integer>(0, 11),
      std::make_unique<Integer>(10, 11), nullptr,
      std::make_unique<Stmts>(std::move(forBodyStatements), 11), 11);
  ExpectEqual(
      forStmt.ToString(),
      std::string("ForStmt(Identifier[\"i\"], Integer[0], Integer[10], None, "
                  "Stmts([LocalAssignment(Identifier[\"y\"], Integer[2])]))"),
      "ForStmt ToString mismatch");

  std::vector<ParamPtr> params;
  params.push_back(std::make_unique<Param>("name", 13));

  std::vector<StmtPtr> functionStatements;
  functionStatements.push_back(
      std::make_unique<RetStmt>(std::make_unique<Identifier>("name", 13), 13));

  FuncDecl funcDecl("greet", std::move(params),
                    std::make_unique<Stmts>(std::move(functionStatements), 13),
                    13);
  ExpectEqual(funcDecl.ToString(),
              std::string("FuncDecl(\"greet\", [Param[\"name\"]], "
                          "Stmts([RetStmt[Identifier[\"name\"]]]))"),
              "FuncDecl ToString mismatch");
}

void TestFuncCallWrappingToString() {
  std::vector<ExprPtr> args;
  args.push_back(std::make_unique<Integer>(1, 15));
  args.push_back(std::make_unique<Identifier>("value", 15));

  auto funcCall = std::make_unique<FuncCall>("sum", std::move(args), 15);
  ExpectEqual(
      funcCall->ToString(),
      std::string("FuncCall(\"sum\", [Integer[1], Identifier[\"value\"]])"),
      "FuncCall ToString mismatch");

  FuncCallStmt statement(std::move(funcCall));
  ExpectEqual(statement.ToString(),
              std::string("FuncCallStmt(FuncCall(\"sum\", [Integer[1], "
                          "Identifier[\"value\"]]))"),
              "FuncCallStmt ToString mismatch");
}

} // namespace

namespace pinky::test {

const std::vector<TestCase> &RegisteredTests() {
  static const std::vector<TestCase> tests = {
      {"TESTLiteralNodesToString", TestLiteralNodesToString},
      {"TESTExpressionNodesToString", TestExpressionNodesToString},
      {"TESTStatementAndDeclarationNodesToString",
       TestStatementAndDeclarationNodesToString},
      {"TESTFuncCallWrappingToString", TestFuncCallWrappingToString},
  };

  return tests;
}

} // namespace pinky::test