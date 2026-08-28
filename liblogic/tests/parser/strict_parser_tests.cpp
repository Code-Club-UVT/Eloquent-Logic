#include <gtest/gtest.h>
#include <memory>
#include <vector>

#include "strict_parser_listener_t.hpp"
#include <lexeme.hpp>
#include <strict_parser.hpp>
#include <unknown_variable_error.hpp>

namespace eloquent::logic::testing {

// No-op listener implementation for testing purposes
class mock_parser_listener : public strict_parser_listener_t {
  public:
    bool found_double_lequi = false;
    void didStart() override {}
    void didFinish() override {}
    void startedProcessingParanthesis() override {}
    void acceptUnaryOpVariant() override {}
    void acceptBinaryOpVariant() override {}
    void foundAstError(const Cursor &) override {}
    void didTryInvalidPosition(NodeObsPtr, size_t) override {}
    void wrongArityForNode(NodeObsPtr) override {}
    void foundUnexpectedToken(const lexeme &) override {}
    void foundDoubleLEqui(const lexeme &) override {
        found_double_lequi = true;
    };
};

class StrictParserTest : public ::testing::Test {
  protected:
    std::shared_ptr<mock_parser_listener> listener;

    void SetUp() override {
        listener = std::make_shared<mock_parser_listener>();
    }

    // Helper to build a vector of lexemes ending with EOF
    std::vector<lexeme> make_stream(const std::vector<lexeme> &tokens) {
        std::vector<lexeme> stream = tokens;
        stream.push_back(lexeme::make(lexeme_type::Eof, "", 0, 0));
        return stream;
    }

    // Creates an Atom lexeme
    lexeme Atom(const std::string &name) {
        return lexeme::make(lexeme_type::Atom, name, 0, 0);
    }

    // Creates an Operator or Symbol lexeme
    lexeme Sym(lexeme_type type, const std::string &val = "") {
        return lexeme::make(type, val, 0, 0);
    }
};

TEST_F(StrictParserTest, ParsesSingleAtom) {
    auto tokens = make_stream({Atom("P")});

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = strict_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    ASSERT_NE(tree->rootRef(), nullptr);
    EXPECT_EQ(tree->rootRef()->getLexeme().type(), lexeme_type::Atom);
    EXPECT_EQ(tree->rootRef()->getLexeme().token(), "P");
}

TEST_F(StrictParserTest, ParsesUnaryOperator) {
    // Formula: (~P)
    auto tokens = make_stream({Sym(lexeme_type::LParen),
                               Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
                               Atom("P"), Sym(lexeme_type::RParen)});

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = strict_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    // Verify operator root and operand child
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::NotOp);
    ASSERT_EQ(root->num_children(), 1);
    EXPECT_EQ(root->childAt(0)->getLexeme().type(), lexeme_type::Atom);
}
TEST_F(StrictParserTest, RejectsDoubleLEqui) {
    // Formula: P \lequi Q \lequi R -> disallowed because of double logical
    // consequence operators
    auto tokens = make_stream(
        {Sym(lexeme_type::LParen), Atom("P"), Sym(lexeme_type::LEquiOp),
         Sym(lexeme_type::LParen), Atom("Q"), Sym(lexeme_type::LEquiOp),
         Atom("R"), Sym(lexeme_type::RParen), Sym(lexeme_type::RParen)});

    EXPECT_THROW(strict_parser::parse(tokens, listener),
                 unknown_variable_error);
    EXPECT_TRUE(listener->found_double_lequi);
}
TEST_F(StrictParserTest, ParsesBinaryOperator) {
    // Formula: (P & Q)
    auto tokens = make_stream({Sym(lexeme_type::LParen), Atom("P"),
                               Sym(lexeme_type::AndOp, symbols::SYMB_AND),
                               Atom("Q"), Sym(lexeme_type::RParen)});

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = strict_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    EXPECT_EQ(root->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(root->num_children(), 2);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
}

TEST_F(StrictParserTest, ParsesNestedStrictFormula) {
    // Formula: ((P & Q) -> (~R))
    auto tokens = make_stream(
        {Sym(lexeme_type::LParen), Sym(lexeme_type::LParen), Atom("P"),
         Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q"),
         Sym(lexeme_type::RParen),
         Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL),
         Sym(lexeme_type::LParen), Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
         Atom("R"), Sym(lexeme_type::RParen), Sym(lexeme_type::RParen)});

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = strict_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);

    // Validate AST Root -> Implies
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::ImpliesOp);
    ASSERT_EQ(root->num_children(), 2);

    // Left sub-tree -> AndOp
    auto left = root->childAt(0);
    EXPECT_EQ(left->getLexeme().type(), lexeme_type::AndOp);

    // Right sub-tree -> NotOp
    auto right = root->childAt(1);
    EXPECT_EQ(right->getLexeme().type(), lexeme_type::NotOp);
}

TEST_F(StrictParserTest, RejectsUnparenthesizedBinaryOp) {
    // Non-strict formula: P & Q (missing outer parentheses)
    auto tokens = make_stream(
        {Atom("P"), Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q")});

    EXPECT_THROW(strict_parser::parse(tokens, listener),
                 unknown_variable_error);
}

TEST_F(StrictParserTest, RejectsUnmatchedParentheses) {
    // Missing closing parenthesis: (P & Q
    auto tokens =
        make_stream({Sym(lexeme_type::LParen), Atom("P"),
                     Sym(lexeme_type::AndOp, symbols::SYMB_AND), Atom("Q")});

    EXPECT_THROW(strict_parser::parse(tokens, listener),
                 unknown_variable_error);
}

TEST_F(StrictParserTest, RejectsMissingOperator) {
    // Invalid binary expression: (P Q)
    auto tokens = make_stream({Sym(lexeme_type::LParen), Atom("P"), Atom("Q"),
                               Sym(lexeme_type::RParen)});

    EXPECT_THROW(strict_parser::parse(tokens, listener),
                 unknown_variable_error);
}

TEST_F(StrictParserTest, RejectsDoubleNegativeWithoutParens) {
    // Strict syntax requires parens for each operator: (~(~P)) vs (~~P)
    auto tokens = make_stream({Sym(lexeme_type::LParen),
                               Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
                               Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
                               Atom("P"), Sym(lexeme_type::RParen)});

    EXPECT_THROW(strict_parser::parse(tokens, listener),
                 unknown_variable_error);
}

TEST_F(StrictParserTest, RejectsEmptyParentheses) {
    // Expression: ()
    auto tokens =
        make_stream({Sym(lexeme_type::LParen), Sym(lexeme_type::RParen)});

    EXPECT_THROW(strict_parser::parse(tokens, listener),
                 unknown_variable_error);
}

} // namespace eloquent::logic::testing