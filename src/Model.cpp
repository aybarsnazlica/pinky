#include "Model.h"

#include "Utils.h"

#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace pinky {
Node::~Node() = default;

Integer::Integer(const int value, const int line) : Value(value), Line(line) {}

std::string Integer::ToString() const {
  return "Integer[" + Stringify(Value) + "]";
}

Float::Float(const double value, const int line) : Value(value), Line(line) {}

std::string Float::ToString() const {
  return "Float[" + Stringify(Value) + "]";
}

Bool::Bool(const bool value, const int line) : Value(value), Line(line) {}

std::string Bool::ToString() const {
  return "Bool[" + Stringify(Value) + "]";
}

String::String(std::string value, const int line)
    : Value(std::move(value)), Line(line) {}

std::string String::ToString() const {
  return "String[" + Value + "]";
}

UnOp::UnOp(Token op, ExprPtr operand, const int line)
    : Op(std::move(op)), Operand(std::move(operand)), Line(line) {}

std::string UnOp::ToString() const {
  return "UnOp(\"" + Op.Lexeme + "\", " + Operand->ToString() + ")";
}

BinOp::BinOp(Token op, ExprPtr left, ExprPtr right, const int line)
    : Op(std::move(op)), Left(std::move(left)), Right(std::move(right)),
      Line(line) {}

std::string BinOp::ToString() const {
  return "BinOp(\"" + Op.Lexeme + "\", " + Left->ToString() + ", " +
         Right->ToString() + ")";
}

LogicalOp::LogicalOp(Token op, ExprPtr left, ExprPtr right, const int line)
    : Op(std::move(op)), Left(std::move(left)), Right(std::move(right)),
      Line(line) {}

std::string LogicalOp::ToString() const {
  return "LogicalOp(\"" + Op.Lexeme + "\", " + Left->ToString() + ", " +
         Right->ToString() + ")";
}

Grouping::Grouping(ExprPtr value, const int line)
    : Value(std::move(value)), Line(line) {}

std::string Grouping::ToString() const {
  return "Grouping(" + Value->ToString() + ")";
}

Identifier::Identifier(std::string name, const int line)
    : Name(std::move(name)), Line(line) {}

std::string Identifier::ToString() const {
  return "Identifier[\"" + Name + "\"]";
}

Stmts::Stmts(std::vector<StmtPtr> stmts, const int line)
    : Statements(std::move(stmts)), Line(line) {}

std::string Stmts::ToString() const {
  std::string result = "Stmts([";

  for (std::size_t index = 0; index < Statements.size(); ++index) {
    if (index > 0) {
      result += ", ";
    }
    result += Statements[index]->ToString();
  }

  result += "])";
  return result;
}

PrintStmt::PrintStmt(ExprPtr value, std::string end, const int line)
    : Value(std::move(value)), End(std::move(end)), Line(line) {}

std::string PrintStmt::ToString() const {
  std::string end = End;
  if (end == "\n") {
    end = "\\n";
  }

  return "PrintStmt(" + Value->ToString() + ", end=\"" + end + "\")";
}

IfStmt::IfStmt(ExprPtr test, std::unique_ptr<Stmts> thenStmts,
               std::unique_ptr<Stmts> elseStmts, const int line)
    : Test(std::move(test)), ThenStmts(std::move(thenStmts)),
      ElseStmts(std::move(elseStmts)), Line(line) {}

std::string IfStmt::ToString() const {
  const std::string elseStmts =
      ElseStmts == nullptr ? "None" : ElseStmts->ToString();
  return "IfStmt(" + Test->ToString() + ", then:" + ThenStmts->ToString() +
         ", else:" + elseStmts + ")";
}

WhileStmt::WhileStmt(ExprPtr test, std::unique_ptr<Stmts> bodyStmts,
                     const int line)
    : Test(std::move(test)), BodyStmts(std::move(bodyStmts)), Line(line) {}

std::string WhileStmt::ToString() const {
  return "WhileStmt(" + Test->ToString() + ", " + BodyStmts->ToString() + ")";
}

Assignment::Assignment(ExprPtr left, ExprPtr right, const int line)
    : Left(std::move(left)), Right(std::move(right)), Line(line) {}

std::string Assignment::ToString() const {
  return "Assignment(" + Left->ToString() + ", " + Right->ToString() + ")";
}

LocalAssignment::LocalAssignment(ExprPtr left, ExprPtr right, const int line)
    : Left(std::move(left)), Right(std::move(right)), Line(line) {}

std::string LocalAssignment::ToString() const {
  return "LocalAssignment(" + Left->ToString() + ", " + Right->ToString() + ")";
}

ForStmt::ForStmt(std::unique_ptr<Identifier> ident, ExprPtr start, ExprPtr end,
                 ExprPtr step, std::unique_ptr<Stmts> bodyStmts, const int line)
    : Ident(std::move(ident)), Start(std::move(start)), End(std::move(end)),
      Step(std::move(step)), BodyStmts(std::move(bodyStmts)), Line(line) {}

std::string ForStmt::ToString() const {
  const std::string step = Step == nullptr ? "None" : Step->ToString();
  return "ForStmt(" + Ident->ToString() + ", " + Start->ToString() + ", " +
         End->ToString() + ", " + step + ", " + BodyStmts->ToString() + ")";
}

Param::Param(std::string name, const int line)
    : Name(std::move(name)), Line(line) {}

std::string Param::ToString() const {
  return "Param[\"" + Name + "\"]";
}

FuncDecl::FuncDecl(std::string name, std::vector<ParamPtr> params,
                   std::unique_ptr<Stmts> bodyStmts, const int line)
    : Name(std::move(name)), Params(std::move(params)),
      BodyStmts(std::move(bodyStmts)), Line(line) {}

std::string FuncDecl::ToString() const {
  std::string params = "[";

  for (std::size_t index = 0; index < Params.size(); ++index) {
    if (index > 0) {
      params += ", ";
    }
    params += Params[index]->ToString();
  }

  params += ']';
  return "FuncDecl(\"" + Name + "\", " + params + ", " + BodyStmts->ToString() +
         ")";
}

FuncCall::FuncCall(std::string name, std::vector<ExprPtr> args, const int line)
    : Name(std::move(name)), Args(std::move(args)), Line(line) {}

std::string FuncCall::ToString() const {
  std::string args = "[";

  for (std::size_t index = 0; index < Args.size(); ++index) {
    if (index > 0) {
      args += ", ";
    }
    args += Args[index]->ToString();
  }

  args += ']';
  return "FuncCall(\"" + Name + "\", " + args + ")";
}

FuncCallStmt::FuncCallStmt(std::unique_ptr<FuncCall> expr)
    : ExprValue(std::move(expr)) {}

std::string FuncCallStmt::ToString() const {
  return "FuncCallStmt(" + ExprValue->ToString() + ")";
}

RetStmt::RetStmt(ExprPtr value, const int line)
    : Value(std::move(value)), Line(line) {}

std::string RetStmt::ToString() const {
  return "RetStmt[" + Value->ToString() + "]";
}

std::ostream &operator<<(std::ostream &stream, const Node &node) {
  return stream << node.ToString();
}

} // namespace pinky