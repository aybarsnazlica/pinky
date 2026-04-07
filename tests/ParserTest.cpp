#include "pinky/Parser.h"
#include "pinky/Errors.h"
#include <vector>

#include "TestFramework.h"
#include "pinky/Token.h"

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
using pinky::test::ExpectTrue;

pinky::Parser MakeParser(std::size_t current = 0) {
  return pinky::Parser({{pinky::TokenType::Identifier, "alpha", 1},
                        {pinky::TokenType::Integer, "42", 2},
                        {pinky::TokenType::Print, "print", 3}},
                       current);
}

void TestAdvanceMovesForward() {
  auto parser = MakeParser();

  const auto first = parser.Advance();
  ExpectEqual(first.Kind, pinky::TokenType::Identifier,
              "first token kind mismatch");
  ExpectEqual(first.Lexeme, std::string("alpha"),
              "first token lexeme mismatch");
  ExpectEqual(first.Line, 1, "first token line mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{1},
              "current index after first advance mismatch");

  const auto second = parser.Advance();
  ExpectEqual(second.Kind, pinky::TokenType::Integer,
              "second token kind mismatch");
  ExpectEqual(second.Lexeme, std::string("42"), "second token lexeme mismatch");
  ExpectEqual(second.Line, 2, "second token line mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{2},
              "current index after second advance mismatch");
}

void TestPeekReturnsCurrentToken() {
  auto parser = MakeParser(1);

  const auto token = parser.Peek();
  ExpectEqual(token.Kind, pinky::TokenType::Integer,
              "peek token kind mismatch");
  ExpectEqual(token.Lexeme, std::string("42"), "peek token lexeme mismatch");
  ExpectEqual(token.Line, 2, "peek token line mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{1},
              "peek should not advance current index");
}

void TestIsNextMatchesCurrentToken() {
  auto parser = MakeParser(1);

  ExpectTrue(parser.IsNext(pinky::TokenType::Integer),
             "expected current token type to match");
  ExpectTrue(!parser.IsNext(pinky::TokenType::Identifier),
             "expected non-matching token type to return false");
}

void TestIsNextReturnsFalseAtEnd() {
  auto parser = MakeParser(3);

  ExpectTrue(!parser.IsNext(pinky::TokenType::Print),
             "expected IsNext() to return false at end of tokens");
}

void TestExpectReturnsAndAdvancesOnMatch() {
  auto parser = MakeParser(1);

  const auto token = parser.Expect(pinky::TokenType::Integer);
  ExpectEqual(token.Kind, pinky::TokenType::Integer,
              "Expect() token kind mismatch");
  ExpectEqual(token.Lexeme, std::string("42"),
              "Expect() token lexeme mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{2},
              "Expect() should advance current index on match");
}

void TestExpectThrowsOnMismatch() {
  auto parser = MakeParser(1);

  try {
    static_cast<void>(parser.Expect(pinky::TokenType::Identifier));
    throw pinky::test::TestFailure("Expected ParseError on mismatched token");
  } catch (const pinky::ParseError &error) {
    ExpectEqual(error.Line(), 2, "mismatch parse error line mismatch");
    ExpectEqual(error.Message(),
                std::string("Expected TOK_IDENTIFIER found 42"),
                "mismatch parse error message mismatch");
    ExpectEqual(pinky::ParserTestAccess::Curr(parser),
                std::size_t{1},
                "Expect() should not advance on mismatch");
  }
}

void TestExpectThrowsAtEndOfParsing() {
  auto parser = MakeParser(3);

  try {
    static_cast<void>(parser.Expect(pinky::TokenType::Print));
    throw pinky::test::TestFailure("Expected ParseError at end of parsing");
  } catch (const pinky::ParseError &error) {
    ExpectEqual(error.Line(), 3, "end-of-parse error line mismatch");
    ExpectEqual(error.Message(),
                std::string("Found print at the end of parsing"),
                "end-of-parse error message mismatch");
    ExpectEqual(
        pinky::ParserTestAccess::Curr(parser), std::size_t{3},
        "Expect() should leave current index unchanged at end of parsing");
  }
}

void TestPreviousTokenReturnsPriorTokenWithoutMutatingParser() {
  auto parser = MakeParser(2);

  const auto token = parser.PreviousToken();
  ExpectEqual(token.Kind, pinky::TokenType::Integer,
              "PreviousToken() kind mismatch");
  ExpectEqual(token.Lexeme, std::string("42"),
              "PreviousToken() lexeme mismatch");
  ExpectEqual(token.Line, 2, "PreviousToken() line mismatch");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{2},
              "PreviousToken() should not mutate current index");
}

void TestMatchMatchesCurrentToken() {
  auto parser = MakeParser(1);

  ExpectTrue(parser.Match(pinky::TokenType::Integer),
             "expected current token type to match");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{2},
              "Match() should advance current index on match");
}

void TestMatchReturnsFalseOnMismatchWithoutAdvancing() {
  auto parser = MakeParser(1);

  ExpectTrue(!parser.Match(pinky::TokenType::Identifier),
             "expected non-matching token type to return false");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{1},
              "Match() should not advance current index on mismatch");
}

void TestMatchReturnsFalseAtEndWithoutAdvancing() {
  auto parser = MakeParser(3);

  ExpectTrue(!parser.Match(pinky::TokenType::Print),
             "expected Match() to return false at end of tokens");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{3},
              "Match() should not advance current index at end of tokens");
}

void TestMatchCanBeUsedSequentially() {
  auto parser = MakeParser();

  ExpectTrue(parser.Match(pinky::TokenType::Identifier),
             "expected first Match() call to consume the identifier");
  ExpectTrue(parser.Match(pinky::TokenType::Integer),
             "expected second Match() call to consume the integer");
  ExpectTrue(!parser.Match(pinky::TokenType::Integer),
             "expected Match() to return false after tokens change");
  ExpectEqual(pinky::ParserTestAccess::Curr(parser),
              std::size_t{2},
              "Match() should only advance for successful calls");
}


} // namespace

namespace pinky::test {

const std::vector<TestCase> &RegisteredTests() {
  static const std::vector<TestCase> tests = {
      {"TESTAdvanceMovesForward", TestAdvanceMovesForward},
      {"TESTPeekReturnsCurrentToken", TestPeekReturnsCurrentToken},
      {"TESTIsNextMatchesCurrentToken", TestIsNextMatchesCurrentToken},
      {"TESTIsNextReturnsFalseAtEnd", TestIsNextReturnsFalseAtEnd},
      {"TESTExpectReturnsAndAdvancesOnMatch",
       TestExpectReturnsAndAdvancesOnMatch},
      {"TESTExpectThrowsOnMismatch", TestExpectThrowsOnMismatch},
      {"TESTExpectThrowsAtEndOfParsing", TestExpectThrowsAtEndOfParsing},
      {"TESTPreviousTokenReturnsPriorTokenWithoutMutatingParser",
       TestPreviousTokenReturnsPriorTokenWithoutMutatingParser},
      {"TESTMatchMatchesCurrentToken", TestMatchMatchesCurrentToken},
      {"TESTMatchReturnsFalseOnMismatchWithoutAdvancing",
       TestMatchReturnsFalseOnMismatchWithoutAdvancing},
      {"TESTMatchReturnsFalseAtEndWithoutAdvancing",
       TestMatchReturnsFalseAtEndWithoutAdvancing},
      {"TESTMatchCanBeUsedSequentially", TestMatchCanBeUsedSequentially},
  };
  return tests;
}

} // namespace pinky::test
