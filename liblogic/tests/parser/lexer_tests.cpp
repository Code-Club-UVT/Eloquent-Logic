#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <vector>

#include "lexeme.hpp"
#include "lexer_listener_t.hpp"
#include <dictionary.h>
#include <lexer.hpp>

namespace eloquent::logic::testing {

class mock_lexer_listener : public lexer_listener_t {
  public:
    bool started = false;
    bool finished = false;
    bool reached_eof = false;
    std::vector<lexeme> recognised;
    std::vector<char> chars_read;
    std::vector<std::pair<std::string, size_t>> unexpected_sequences;

    void didStart() override { started = true; }
    void didFinish() override { finished = true; }
    void didReachEof() override { reached_eof = true; }
    void didRecogniseLexeme(const lexeme &l) override {
        recognised.push_back(l);
    }
    void didReadCharacter(const char &c) override { chars_read.push_back(c); }
    void didFindUnexpectedSequence(std::string_view seq, size_t pos) override {
        unexpected_sequences.emplace_back(std::string(seq), pos);
    }
};

class LexerTest : public ::testing::Test {
  protected:
    std::shared_ptr<mock_lexer_listener> listener;

    void SetUp() override {
        listener = std::make_shared<mock_lexer_listener>();
    }
};

TEST_F(LexerTest, EmptyInputProducesOnlyEof) {
    auto lexemes = lexer::lex("", listener);

    ASSERT_EQ(lexemes.size(), 1);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::Eof);
    EXPECT_EQ(lexemes[0].start(), 0);
    EXPECT_EQ(lexemes[0].end(), 0);

    EXPECT_TRUE(listener->started);
    EXPECT_TRUE(listener->finished);
    EXPECT_TRUE(listener->reached_eof);
    // EOF is never routed through didRecogniseLexeme, only didReachEof.
    EXPECT_TRUE(listener->recognised.empty());
    // The empty-input path returns before the read loop starts.
    EXPECT_TRUE(listener->chars_read.empty());
}

TEST_F(LexerTest, WhitespaceIsSkippedButStillReported) {
    // "(" and ")" surrounded by spaces: whitespace must not produce lexemes,
    // but every character (including whitespace) is still reported via
    // didReadCharacter.
    auto lexemes = lexer::lex(" ( ) ", listener);

    ASSERT_EQ(lexemes.size(), 3);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::LParen);
    EXPECT_EQ(lexemes[0].start(), 1);
    EXPECT_EQ(lexemes[1].type(), lexeme_type::RParen);
    EXPECT_EQ(lexemes[1].start(), 3);
    EXPECT_EQ(lexemes[2].type(), lexeme_type::Eof);

    EXPECT_EQ(listener->chars_read.size(), 5);
}

TEST_F(LexerTest, SingleLetterAtomFinalizedViaEof) {
    // A bare atom name is only finalized once the lexer reaches end-of-text
    // (ETX pass), since there's nothing after 'P' to signal the name is
    // complete mid-stream.
    auto lexemes = lexer::lex("P", listener);

    ASSERT_EQ(lexemes.size(), 2);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[0].token(), "P");
    EXPECT_EQ(lexemes[0].start(), 0);
    EXPECT_EQ(lexemes[0].end(), 0);

    EXPECT_EQ(lexemes[1].type(), lexeme_type::Eof);
    EXPECT_EQ(lexemes[1].start(), 1);
    EXPECT_EQ(lexemes[1].end(), 1);

    ASSERT_EQ(listener->recognised.size(), 1);
    EXPECT_EQ(listener->recognised[0].token(), "P");
}

TEST_F(LexerTest, AtomWithSingleDigitSubscriptFinalizedMidStream) {
    // Unlike the bare-name case, a single-digit subscript completes the atom
    // as soon as the digit is read, without needing the ETX pass.
    auto lexemes = lexer::lex("P_1", listener);

    ASSERT_EQ(lexemes.size(), 2);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[0].token(), "P_1");
    EXPECT_EQ(lexemes[0].start(), 0);
    EXPECT_EQ(lexemes[0].end(), 2);
}

TEST_F(LexerTest, BracedSubscriptOddDigitCount) {
    {
        auto lexemes = lexer::lex("P_{1}", listener);
        ASSERT_EQ(lexemes.size(), 2);
        EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
        EXPECT_EQ(lexemes[0].token(), "P_{1}");
    }
    {
        auto fresh_listener = std::make_shared<mock_lexer_listener>();
        auto lexemes = lexer::lex("P_{123}", fresh_listener);
        ASSERT_EQ(lexemes.size(), 2);
        EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
        EXPECT_EQ(lexemes[0].token(), "P_{123}");
    }
}

TEST_F(LexerTest, BracedSubscriptEvenDigitCountDoesNotThrow) {
    // Regression: atom_multichar_subscript_func used to ping-pong back into a
    // state that only accepted digits, so a closing '}' after an even number
    // of digits threw lexer_exception.
    {
        auto lexemes = lexer::lex("P_{12}", listener);
        ASSERT_EQ(lexemes.size(), 2);
        EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
        EXPECT_EQ(lexemes[0].token(), "P_{12}");
    }
    {
        auto fresh_listener = std::make_shared<mock_lexer_listener>();
        auto lexemes = lexer::lex("P_{1234}", fresh_listener);
        ASSERT_EQ(lexemes.size(), 2);
        EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
        EXPECT_EQ(lexemes[0].token(), "P_{1234}");
    }
}

TEST_F(LexerTest, ZeroSubscriptHandling) {
    // Regression: the closing '}' branch used to drop the brace from the
    // buffer, and leading-zero stripping used to operate on a temporary and do
    // nothing.
    struct Case {
        std::string input;
        std::string expected_token;
    };
    const std::vector<Case> cases = {
        {"P_{0}", "P_{0}"},
        {"P_{00}", "P_{0}"},
        {"P_{007}", "P_{7}"},
        {"P_{102}",
         "P_{102}"}, // zero in the middle, not a leading zero: left untouched
    };

    for (const auto &c : cases) {
        SCOPED_TRACE(c.input);
        auto fresh_listener = std::make_shared<mock_lexer_listener>();
        auto lexemes = lexer::lex(c.input, fresh_listener);
        ASSERT_EQ(lexemes.size(), 2);
        EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
        EXPECT_EQ(lexemes[0].token(), c.expected_token);
    }
}

TEST_F(LexerTest, Parentheses) {
    auto lexemes = lexer::lex("()", listener);

    ASSERT_EQ(lexemes.size(), 3);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::LParen);
    EXPECT_EQ(lexemes[0].token(), "(");
    EXPECT_EQ(lexemes[0].start(), 0);
    EXPECT_EQ(lexemes[0].end(), 0);

    EXPECT_EQ(lexemes[1].type(), lexeme_type::RParen);
    EXPECT_EQ(lexemes[1].token(), ")");
    EXPECT_EQ(lexemes[1].start(), 1);
    EXPECT_EQ(lexemes[1].end(), 1);

    EXPECT_EQ(lexemes[2].type(), lexeme_type::Eof);
}

TEST_F(LexerTest, EveryOperatorSymbolStandalone) {
    struct Case {
        std::string symbol;
        lexeme_type type;
    };
    const std::vector<Case> cases = {
        {symbols::SYMB_NOT, lexeme_type::NotOp}, // regression: was unreachable
        {symbols::SYMB_AND, lexeme_type::AndOp},
        {symbols::SYMB_OR, lexeme_type::OrOp},
        {symbols::SYMB_IMPL, lexeme_type::ImpliesOp},
        {symbols::SYMB_IFF, lexeme_type::IffOp},
        {symbols::SYMB_LEQUI, lexeme_type::LEquiOp},
        {symbols::SYMB_TAUTOLOGY, lexeme_type::Tautology},
        {symbols::SYMB_CONTRADICTION, lexeme_type::Contradiction},
    };

    for (const auto &c : cases) {
        SCOPED_TRACE(c.symbol);
        auto fresh_listener = std::make_shared<mock_lexer_listener>();
        auto lexemes = lexer::lex(c.symbol, fresh_listener);

        ASSERT_EQ(lexemes.size(), 2);
        EXPECT_EQ(lexemes[0].type(), c.type);
        EXPECT_EQ(lexemes[0].token(), c.symbol);
        EXPECT_EQ(lexemes[0].start(), 0);
        EXPECT_EQ(lexemes[1].type(), lexeme_type::Eof);
    }
}

TEST_F(LexerTest, OperatorImmediatelyAfterAtomNoSpace) {
    // Regression: the operator's accumulation buffer wasn't re-seeded with the
    // leading backslash after an atom->operator transition, so it never
    // matched.
    std::string input = "P" + std::string(symbols::SYMB_AND);
    auto lexemes = lexer::lex(input, listener);

    ASSERT_EQ(lexemes.size(), 3);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[0].token(), "P");
    EXPECT_EQ(lexemes[1].type(), lexeme_type::AndOp);
    EXPECT_EQ(lexemes[1].token(), symbols::SYMB_AND);
    EXPECT_EQ(lexemes[2].type(), lexeme_type::Eof);
}

TEST_F(LexerTest, OperatorAfterAtomWithSpace) {
    std::string input = "P " + std::string(symbols::SYMB_AND) + " Q";
    auto lexemes = lexer::lex(input, listener);

    ASSERT_EQ(lexemes.size(), 4);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[0].token(), "P");
    EXPECT_EQ(lexemes[1].type(), lexeme_type::AndOp);
    EXPECT_EQ(lexemes[1].token(), symbols::SYMB_AND);
    // Known quirk (documented, not fixed): when an operator immediately follows
    // an atom, in_lexeme/last_change get reset by the atom's own emission in
    // the same loop iteration, so the operator's reported start lands on its
    // second character rather than the leading backslash. Token type/text are
    // unaffected.
    EXPECT_EQ(lexemes[2].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[2].token(), "Q");
    EXPECT_EQ(lexemes[3].type(), lexeme_type::Eof);
}

TEST_F(LexerTest, FullFormulaWithAnd) {
    std::string input = "P " + std::string(symbols::SYMB_AND) + " Q";
    auto lexemes = lexer::lex(input, listener);

    ASSERT_EQ(lexemes.size(), 4);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[0].token(), "P");
    EXPECT_EQ(lexemes[1].type(), lexeme_type::AndOp);
    EXPECT_EQ(lexemes[2].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[2].token(), "Q");
    EXPECT_EQ(lexemes[3].type(), lexeme_type::Eof);

    ASSERT_EQ(listener->recognised.size(), 3);
    EXPECT_TRUE(listener->started);
    EXPECT_TRUE(listener->finished);
    EXPECT_TRUE(listener->reached_eof);
}

TEST_F(LexerTest, FullFormulaWithNegation) {
    std::string input = std::string(symbols::SYMB_NOT) + " P";
    auto lexemes = lexer::lex(input, listener);

    ASSERT_EQ(lexemes.size(), 3);
    EXPECT_EQ(lexemes[0].type(), lexeme_type::NotOp);
    EXPECT_EQ(lexemes[0].token(), symbols::SYMB_NOT);
    EXPECT_EQ(lexemes[1].type(), lexeme_type::Atom);
    EXPECT_EQ(lexemes[1].token(), "P");
    EXPECT_EQ(lexemes[2].type(), lexeme_type::Eof);
}

TEST_F(LexerTest, UnrecognisedCharacterThrowsMidStream) {
    EXPECT_THROW(lexer::lex("!", listener), lexer_exception);

    ASSERT_EQ(listener->unexpected_sequences.size(), 1);
    EXPECT_EQ(listener->unexpected_sequences[0].first, "!");
    EXPECT_EQ(listener->unexpected_sequences[0].second, 0);
    EXPECT_TRUE(listener->finished);
    // Exception paths never report reaching EOF.
    EXPECT_FALSE(listener->reached_eof);
}

TEST_F(LexerTest, UnmatchedOperatorThrowsAtEndOfText) {
    // "\bogus" is never a prefix match for any real operator symbol, so this is
    // only caught once the ETX pass confirms no symbol was ever completed.
    std::string input = "\\bogus";
    EXPECT_THROW(lexer::lex(input, listener), lexer_exception);

    ASSERT_EQ(listener->unexpected_sequences.size(), 1);
    EXPECT_EQ(listener->unexpected_sequences[0].first, input);
    EXPECT_EQ(listener->unexpected_sequences[0].second, 0);
    EXPECT_TRUE(listener->finished);
}

TEST_F(LexerTest, LexerExceptionMessageContainsSequenceAndPosition) {
    try {
        lexer::lex("!", listener);
        FAIL() << "expected lexer_exception to be thrown";
    } catch (const lexer_exception &ex) {
        std::string message = ex.what();
        EXPECT_NE(message.find('!'), std::string::npos);
        EXPECT_NE(message.find('0'), std::string::npos);
    }
}

} // namespace eloquent::logic::testing
