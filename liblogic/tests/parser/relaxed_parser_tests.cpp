#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include <string>

#include "../../src/parser/relaxed_parser.hpp"
#include "relaxed_parser_listener.hpp"
#include "../../src/parser/unexpected_token_error.hpp"
#include "lexeme.hpp"
#include "../../src/parser/dictionary.h"

namespace eloquent::logic::testing
{

class mock_relaxed_parser_listener : public relaxed_parser_listener_t
{
public:
    bool started = false;
    bool finished = false;
    int parens_started = 0;
    int pure_atom_in_parens_count = 0;
    int mismatched_parens_count = 0;
    int unexpected_token_count = 0;

    void didStart() override { started = true; }
    void didFinish() override { finished = true; }
    void startParsingParens(const lexeme&) override { parens_started++; }
    void didFindPureAtomInParens(const lexeme&, const lexeme&, const lexeme&) override { pure_atom_in_parens_count++; }
    void mismatchedParens(const lexeme&) override { mismatched_parens_count++; }
    void foundUnexpectedToken(const lexeme&) override { unexpected_token_count++; }
};

class RelaxedParserTest : public ::testing::Test
{
protected:
    std::shared_ptr<mock_relaxed_parser_listener> listener;

    void SetUp() override
    {
        listener = std::make_shared<mock_relaxed_parser_listener>();
    }

    std::vector<lexeme> make_stream(const std::vector<lexeme>& tokens)
    {
        std::vector<lexeme> stream = tokens;
        stream.push_back(lexeme::make(lexeme_type::Eof, "", 0, 0));
        return stream;
    }

    lexeme Atom(const std::string& name)
    {
        return lexeme::make(lexeme_type::Atom, name, 0, 0);
    }

    lexeme Sym(lexeme_type type, const std::string& val = "")
    {
        return lexeme::make(type, val, 0, 0);
    }
};

TEST_F(RelaxedParserTest, ParsesSingleAtom)
{
    auto tokens = make_stream({ Atom("P") });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    ASSERT_NE(tree->rootRef(), nullptr);
    EXPECT_EQ(tree->rootRef()->getLexeme().type(), lexeme_type::Atom);
    EXPECT_EQ(tree->rootRef()->getLexeme().token(), "P");
    EXPECT_TRUE(listener->started);
    EXPECT_TRUE(listener->finished);
}

TEST_F(RelaxedParserTest, ParsesUnaryNot)
{
    // Formula: ~P
    auto tokens = make_stream({
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Atom("P")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::NotOp);
    ASSERT_EQ(root->num_children(), 1);
    EXPECT_EQ(root->childAt(0)->getLexeme().type(), lexeme_type::Atom);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
}

TEST_F(RelaxedParserTest, ParsesDoubleNegation)
{
    // Formula: ~~P
    auto tokens = make_stream({
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Atom("P")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::NotOp);
    ASSERT_EQ(root->num_children(), 1);

    auto child = root->childAt(0);
    EXPECT_EQ(child->getLexeme().type(), lexeme_type::NotOp);
    ASSERT_EQ(child->num_children(), 1);
    EXPECT_EQ(child->childAt(0)->getLexeme().token(), "P");
}

TEST_F(RelaxedParserTest, ParsesBinaryAnd)
{
    // Formula: P & Q
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("Q")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(root->num_children(), 2);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
}

TEST_F(RelaxedParserTest, ParsesBinaryOr)
{
    // Formula: P | Q
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::OrOp, symbols::SYMB_OR),
        Atom("Q")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::OrOp);
    ASSERT_EQ(root->num_children(), 2);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
}

TEST_F(RelaxedParserTest, ParsesBinaryImplies)
{
    // Formula: P -> Q
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL),
        Atom("Q")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::ImpliesOp);
    ASSERT_EQ(root->num_children(), 2);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
}

TEST_F(RelaxedParserTest, ParsesBinaryIff)
{
    // Formula: P <-> Q
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::IffOp, symbols::SYMB_IFF),
        Atom("Q")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::IffOp);
    ASSERT_EQ(root->num_children(), 2);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
}

TEST_F(RelaxedParserTest, OperatorPrecedenceAndOr)
{
    // Formula: P & Q | R  =>  (P & Q) | R because & (power 10) > | (power 9)
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("Q"),
        Sym(lexeme_type::OrOp, symbols::SYMB_OR),
        Atom("R")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    // Root should be OrOp
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::OrOp);
    ASSERT_EQ(root->num_children(), 2);

    // Left child should be P & Q
    auto left = root->childAt(0);
    EXPECT_EQ(left->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(left->num_children(), 2);
    EXPECT_EQ(left->childAt(0)->getLexeme().token(), "P");
    EXPECT_EQ(left->childAt(1)->getLexeme().token(), "Q");

    // Right child should be R
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "R");
}

TEST_F(RelaxedParserTest, OperatorPrecedenceNotAnd)
{
    // Formula: ~P & Q  =>  (~P) & Q
    auto tokens = make_stream({
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("Q")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    // Root should be AndOp
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(root->num_children(), 2);

    // Left child should be ~P
    auto left = root->childAt(0);
    EXPECT_EQ(left->getLexeme().type(), lexeme_type::NotOp);
    ASSERT_EQ(left->num_children(), 1);
    EXPECT_EQ(left->childAt(0)->getLexeme().token(), "P");

    // Right child should be Q
    EXPECT_EQ(root->childAt(1)->getLexeme().token(), "Q");
}

TEST_F(RelaxedParserTest, ParsesParenthesizedExpression)
{
    // Formula: P & (Q | R)  =>  overrides default precedence
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Sym(lexeme_type::LParen),
        Atom("Q"),
        Sym(lexeme_type::OrOp, symbols::SYMB_OR),
        Atom("R"),
        Sym(lexeme_type::RParen)
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    // Root should be AndOp
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(root->num_children(), 2);
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");

    // Right child should be Q | R
    auto right = root->childAt(1);
    EXPECT_EQ(right->getLexeme().type(), lexeme_type::OrOp);
    ASSERT_EQ(right->num_children(), 2);
    EXPECT_EQ(right->childAt(0)->getLexeme().token(), "Q");
    EXPECT_EQ(right->childAt(1)->getLexeme().token(), "R");
}

TEST_F(RelaxedParserTest, RejectsPureAtomInParens)
{
    // Formula: (P) -> disallowed in relaxed syntax
    auto tokens = make_stream({
        Sym(lexeme_type::LParen),
        Atom("P"),
        Sym(lexeme_type::RParen)
    });

    EXPECT_THROW(relaxed_parser::parse(tokens, listener), unexpected_token_error);
    EXPECT_GT(listener->pure_atom_in_parens_count, 0);
}

TEST_F(RelaxedParserTest, RejectsMismatchedParens)
{
    // Formula: (P & Q (missing closing paren)
    auto tokens = make_stream({
        Sym(lexeme_type::LParen),
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("Q")
    });

    EXPECT_THROW(relaxed_parser::parse(tokens, listener), unexpected_token_error);
    EXPECT_GT(listener->mismatched_parens_count, 0);
}

TEST_F(RelaxedParserTest, RejectsEmptyParens)
{
    // Formula: ()
    auto tokens = make_stream({
        Sym(lexeme_type::LParen),
        Sym(lexeme_type::RParen)
    });

    EXPECT_THROW(relaxed_parser::parse(tokens, listener), unexpected_token_error);
}

TEST_F(RelaxedParserTest, RejectsTrailingOperator)
{
    // Formula: P &
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND)
    });

    EXPECT_THROW(relaxed_parser::parse(tokens, listener), unexpected_token_error);
}

TEST_F(RelaxedParserTest, ParsesMultipleNegations)
{
    // Formula: ~~~~~~P (6 negations)
    auto tokens = make_stream({
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Sym(lexeme_type::NotOp, symbols::SYMB_NOT),
        Atom("P")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto curr = tree->rootRef();
    for (int i = 0; i < 6; ++i)
    {
        ASSERT_NE(curr, nullptr);
        EXPECT_EQ(curr->getLexeme().type(), lexeme_type::NotOp);
        ASSERT_EQ(curr->num_children(), 1);
        curr = curr->childAt(0);
    }
    ASSERT_NE(curr, nullptr);
    EXPECT_EQ(curr->getLexeme().type(), lexeme_type::Atom);
    EXPECT_EQ(curr->getLexeme().token(), "P");
}

TEST_F(RelaxedParserTest, RightAssociationImplies)
{
    // Formula: P -> Q -> R  =>  P -> (Q -> R)
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL),
        Atom("Q"),
        Sym(lexeme_type::ImpliesOp, symbols::SYMB_IMPL),
        Atom("R")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    // Root should be ImpliesOp (first ->)
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::ImpliesOp);
    ASSERT_EQ(root->num_children(), 2);

    // Left child should be P
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");

    // Right child should be Q -> R
    auto right = root->childAt(1);
    EXPECT_EQ(right->getLexeme().type(), lexeme_type::ImpliesOp);
    ASSERT_EQ(right->num_children(), 2);
    EXPECT_EQ(right->childAt(0)->getLexeme().token(), "Q");
    EXPECT_EQ(right->childAt(1)->getLexeme().token(), "R");
}

TEST_F(RelaxedParserTest, RightAssociationAnd)
{
    // Formula: P & Q & R  =>  P & (Q & R) since all n-ary operators are right-associative
    auto tokens = make_stream({
        Atom("P"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("Q"),
        Sym(lexeme_type::AndOp, symbols::SYMB_AND),
        Atom("R")
    });

    std::shared_ptr<syntax_tree> tree;
    EXPECT_NO_THROW(tree = relaxed_parser::parse(tokens, listener));

    ASSERT_NE(tree, nullptr);
    auto root = tree->rootRef();
    ASSERT_NE(root, nullptr);

    // Root should be AndOp (first &)
    EXPECT_EQ(root->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(root->num_children(), 2);

    // Left child should be P
    EXPECT_EQ(root->childAt(0)->getLexeme().token(), "P");

    // Right child should be Q & R
    auto right = root->childAt(1);
    EXPECT_EQ(right->getLexeme().type(), lexeme_type::AndOp);
    ASSERT_EQ(right->num_children(), 2);
    EXPECT_EQ(right->childAt(0)->getLexeme().token(), "Q");
    EXPECT_EQ(right->childAt(1)->getLexeme().token(), "R");
}

TEST_F(RelaxedParserTest, RejectsMissingOperator)
{
    // Formula: P Q
    auto tokens = make_stream({
        Atom("P"),
        Atom("Q")
    });

    EXPECT_THROW(relaxed_parser::parse(tokens, listener), unexpected_token_error);
}

} // namespace eloquent::logic::testing
