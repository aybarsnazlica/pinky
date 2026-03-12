#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "TestFramework.h"
#include "Lexer.h"
#include "Token.h"

namespace pinky {
struct LexerTestAccess {
  static void SetStart(Lexer &lexer, std::size_t start) {
    lexer.TokenStart = start;
  }
  static void SetCurr(Lexer &lexer, std::size_t curr) {
    lexer.Current = curr;
  }
  static void SetLine(Lexer &lexer, int line) {
    lexer.LineNumber = line;
  }
  static std::size_t SourceSize(const Lexer &lexer) {
    return lexer.Source.size();
  }
  static const std::vector<Token> &Tokens(const Lexer &lexer) {
    return lexer.Tokens;
  }
};
} // namespace pinky

namespace {
using pinky::test::ExpectEqual;
using pinky::test::ExpectTrue;
using pinky::test::TestFailure;

pinky::Lexer MakePositionedLexer(std::string source, std::size_t curr = 1,
                                 int line = 1) {
  pinky::Lexer lexer(std::move(source));
  pinky::LexerTestAccess::SetStart(lexer, 0);
  pinky::LexerTestAccess::SetCurr(lexer, curr);
  pinky::LexerTestAccess::SetLine(lexer, line);
  return lexer;
}

const std::vector<pinky::Token> &TokensOf(const pinky::Lexer &lexer) {
  return pinky::LexerTestAccess::Tokens(lexer);
}

std::vector<pinky::TokenType>
TokenTypes(const std::vector<pinky::Token> &tokens) {
  std::vector<pinky::TokenType> out;
  out.reserve(tokens.size());
  for (const auto &token : tokens) {
    out.push_back(token.Kind);
  }
  return out;
}

void AssertTokenTypes(const std::vector<pinky::TokenType> &got,
                      const std::vector<pinky::TokenType> &want) {
  if (got.size() != want.size()) {
    std::ostringstream stream;
    stream << "token count = " << got.size() << ", want " << want.size();
    throw TestFailure(stream.str());
  }

  for (std::size_t i = 0; i < want.size(); ++i) {
    if (got[i] != want[i]) {
      std::ostringstream stream;
      stream << "token[" << i << "] = " << pinky::ToString(got[i]) << ", want "
             << pinky::ToString(want[i]);
      throw TestFailure(stream.str());
    }
  }
}

void TestTokenDebugString() {
  const pinky::Token token{pinky::TokenType::Identifier, "hello", 7};
  ExpectEqual(token.DebugString(),
              std::string("(TOK_IDENTIFIER, \"hello\", 7)"),
              "DebugString() mismatch");
}

void TestAdvanceMovesForward() {
  pinky::Lexer lexer("ab");
  ExpectEqual(lexer.Advance(), 'a', "first advance mismatch");
  ExpectEqual(lexer.Advance(), 'b', "second advance mismatch");
}

void TestPeekReturnsCurrentOrEOF() {
  pinky::Lexer lexer("x");
  ExpectEqual(lexer.Peek(), 'x', "Peek() mismatch");
  lexer.Advance();
  ExpectEqual(lexer.Peek(), '\0', "Peek() at EOF mismatch");
}

void TestLookaheadReadsFutureOrEOF() {
  pinky::Lexer lexer("abcd");
  ExpectEqual(lexer.Lookahead(1), 'b', "Lookahead(1) mismatch");
  ExpectEqual(lexer.Lookahead(2), 'c', "Lookahead(2) mismatch");
  ExpectEqual(lexer.Lookahead(10), '\0', "Lookahead(10) mismatch");
}

void TestMatchConsumesOnSuccess() {
  pinky::Lexer lexer("==");
  lexer.Advance();
  const bool firstMatch = lexer.Match('=');
  const bool secondMatch = lexer.Match('=');
  ExpectTrue(firstMatch, "expected first match to succeed");
  ExpectTrue(!secondMatch, "expected second match to fail");
}

void TestHandleNumberAddsIntegerToken() {
  auto lexer = MakePositionedLexer("123");
  lexer.HandleNumber();

  const auto &tokens = TokensOf(lexer);
  ExpectEqual(tokens.size(), static_cast<std::size_t>(1),
              "token count mismatch");
  ExpectEqual(tokens[0].Kind, pinky::TokenType::Integer, "token type mismatch");
  ExpectEqual(tokens[0].Lexeme, std::string("123"), "lexeme mismatch");
}

void TestHandleNumberAddsFloatToken() {
  auto lexer = MakePositionedLexer("12.5");
  lexer.HandleNumber();

  const auto &tokens = TokensOf(lexer);
  ExpectEqual(tokens.size(), static_cast<std::size_t>(1),
              "token count mismatch");
  ExpectEqual(tokens[0].Kind, pinky::TokenType::Float, "token type mismatch");
  ExpectEqual(tokens[0].Lexeme, std::string("12.5"), "lexeme mismatch");
}

void TestHandleStringAddsStringToken() {
  auto lexer = MakePositionedLexer("\"abc\"");
  lexer.HandleString('"');

  const auto &tokens = TokensOf(lexer);
  ExpectEqual(tokens.size(), static_cast<std::size_t>(1),
              "token count mismatch");
  ExpectEqual(tokens[0].Kind, pinky::TokenType::String, "token type mismatch");
  ExpectEqual(tokens[0].Lexeme, std::string("\"abc\""), "lexeme mismatch");
}

void TestHandleIdentifierAddsKeywordOrIdentifierToken() {
  auto keywordLexer = MakePositionedLexer("while");
  keywordLexer.HandleIdentifier();

  const auto &keywordTokens = TokensOf(keywordLexer);
  ExpectEqual(keywordTokens[0].Kind, pinky::TokenType::While,
              "keyword token type mismatch");

  auto identifierLexer = MakePositionedLexer("abc_12");
  identifierLexer.HandleIdentifier();

  const auto &identifierTokens = TokensOf(identifierLexer);
  ExpectEqual(identifierTokens[0].Kind, pinky::TokenType::Identifier,
              "identifier token type mismatch");
  ExpectEqual(identifierTokens[0].Lexeme, std::string("abc_12"),
              "identifier lexeme mismatch");
}

void TestAddTokenUsesCurrentSliceAndLine() {
  auto lexer = MakePositionedLexer("token", 0, 4);
  pinky::LexerTestAccess::SetCurr(lexer,
                                  pinky::LexerTestAccess::SourceSize(lexer));
  lexer.AddToken(pinky::TokenType::Identifier);

  const auto &tokens = TokensOf(lexer);
  ExpectEqual(tokens.size(), static_cast<std::size_t>(1),
              "token count mismatch");
  ExpectEqual(tokens[0].Kind, pinky::TokenType::Identifier,
              "token type mismatch");
  ExpectEqual(tokens[0].Lexeme, std::string("token"), "lexeme mismatch");
  ExpectEqual(tokens[0].Line, 4, "line mismatch");
}

void TestTokenizesSingleAndDoubleCharOperators() {
  pinky::Lexer lexer("()+-*/^%?:;= == ~= <= >= < > :=");
  const auto tokens = lexer.Tokenize();

  const auto got = TokenTypes(tokens);
  const std::vector<pinky::TokenType> want = {
      pinky::TokenType::LParen,    pinky::TokenType::RParen,
      pinky::TokenType::Plus,      pinky::TokenType::Minus,
      pinky::TokenType::Star,      pinky::TokenType::Slash,
      pinky::TokenType::Caret,     pinky::TokenType::Mod,
      pinky::TokenType::Question,  pinky::TokenType::Colon,
      pinky::TokenType::Semicolon, pinky::TokenType::EQ,
      pinky::TokenType::EQEQ,      pinky::TokenType::NE,
      pinky::TokenType::LE,        pinky::TokenType::GE,
      pinky::TokenType::LT,        pinky::TokenType::GT,
      pinky::TokenType::Assign,
  };

  AssertTokenTypes(got, want);
}

void TestTokenizesKeywordsIdentifiersAndNumbers() {
  pinky::Lexer lexer("if then else local foo _bar 12 3.5");
  const auto tokens = lexer.Tokenize();

  const auto got = TokenTypes(tokens);
  const std::vector<pinky::TokenType> want = {
      pinky::TokenType::If,         pinky::TokenType::Then,
      pinky::TokenType::Else,       pinky::TokenType::Local,
      pinky::TokenType::Identifier, pinky::TokenType::Identifier,
      pinky::TokenType::Integer,    pinky::TokenType::Float,
  };
  AssertTokenTypes(got, want);

  ExpectEqual(tokens[4].Lexeme, std::string("foo"),
              "tokens[4].Lexeme mismatch");
  ExpectEqual(tokens[5].Lexeme, std::string("_bar"),
              "tokens[5].Lexeme mismatch");
  ExpectEqual(tokens[6].Lexeme, std::string("12"), "tokens[6].Lexeme mismatch");
  ExpectEqual(tokens[7].Lexeme, std::string("3.5"),
              "tokens[7].Lexeme mismatch");
}

void TestTokenizesStringsAndSkipsLineComments() {
  pinky::Lexer lexer(
      "println \"hello\"\n-- this is a comment\nprintln 'world'\n");
  const auto tokens = lexer.Tokenize();

  const auto got = TokenTypes(tokens);
  const std::vector<pinky::TokenType> want = {
      pinky::TokenType::Println,
      pinky::TokenType::String,
      pinky::TokenType::Println,
      pinky::TokenType::String,
  };
  AssertTokenTypes(got, want);

  ExpectEqual(tokens[1].Lexeme, std::string("\"hello\""),
              "tokens[1].Lexeme mismatch");
  ExpectEqual(tokens[3].Lexeme, std::string("'world'"),
              "tokens[3].Lexeme mismatch");
  ExpectEqual(tokens[0].Line, 1, "tokens[0].Line mismatch");
  ExpectEqual(tokens[2].Line, 3, "tokens[2].Line mismatch");
}
} // namespace

namespace pinky::test {

const std::vector<TestCase> &RegisteredTests() {
  static const std::vector<TestCase> tests = {
      {"TESTTokenDebugString", TestTokenDebugString},
      {"TESTAdvanceMovesForward", TestAdvanceMovesForward},
      {"TESTPeekReturnsCurrentOrEOF", TestPeekReturnsCurrentOrEOF},
      {"TESTLookaheadReadsFutureOrEOF", TestLookaheadReadsFutureOrEOF},
      {"TESTMatchConsumesOnSuccess", TestMatchConsumesOnSuccess},
      {"TESTHandleNumberAddsIntegerToken", TestHandleNumberAddsIntegerToken},
      {"TESTHandleNumberAddsFloatToken", TestHandleNumberAddsFloatToken},
      {"TESTHandleStringAddsStringToken", TestHandleStringAddsStringToken},
      {"TESTHandleIdentifierAddsKeywordOrIdentifierToken",
       TestHandleIdentifierAddsKeywordOrIdentifierToken},
      {"TESTAddTokenUsesCurrentSliceAndLine",
       TestAddTokenUsesCurrentSliceAndLine},
      {"TESTTokenizesSingleAndDoubleCharOperators",
       TestTokenizesSingleAndDoubleCharOperators},
      {"TESTTokenizesKeywordsIdentifiersAndNumbers",
       TestTokenizesKeywordsIdentifiersAndNumbers},
      {"TESTTokenizesStringsAndSkipsLineComments",
       TestTokenizesStringsAndSkipsLineComments},
  };

  return tests;
}

} // namespace pinky::test
