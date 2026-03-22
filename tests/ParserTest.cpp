#include "Parser.h"
#include <vector>

#include "TestFramework.h"

namespace pinky {
struct ParserTestAccess {
  static void SetCurr(Parser &parser, std::size_t curr) {
    parser.Current = curr;
  }
  static std::size_t Curr(const Parser &parser) {
    return parser.Current;
  }
  static const std::vector<Token> &Tokens(const Parser &parser) {
    return parser.Tokens;
  }
};
} // namespace pinky

namespace {
using pinky::test::ExpectEqual;

void TestAdvanceMovesForward() {
  const std::vector<pinky::Token> tokens = {
      {pinky::TokenType::Identifier, "alpha", 1},
      {pinky::TokenType::Integer, "42", 1},
  };

  pinky::Parser parser(tokens);

  const auto first = parser.Advance();
  ExpectEqual(first.Kind, pinky::TokenType::Identifier,
              "first token kind mismatch");
  ExpectEqual(first.Lexeme, std::string("alpha"),
              "first token lexeme mismatch");
  ExpectEqual(first.Line, 1, "first token line mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              static_cast<std::size_t>(1),
              "current index after first advance mismatch");

  const auto second = parser.Advance();
  ExpectEqual(second.Kind, pinky::TokenType::Integer,
              "second token kind mismatch");
  ExpectEqual(second.Lexeme, std::string("42"), "second token lexeme mismatch");
  ExpectEqual(second.Line, 1, "second token line mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              static_cast<std::size_t>(2),
              "current index after second advance mismatch");
}
} // namespace

namespace pinky::test {

const std::vector<TestCase> &RegisteredTests() {
  static const std::vector<TestCase> tests = {
      {"TESTAdvanceMovesForward", TestAdvanceMovesForward},
  };
  return tests;
}

} // namespace pinky::test