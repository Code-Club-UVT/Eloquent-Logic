#include <ANTLRInputStream.h>
#include <iostream>
#include <LogicLex.h>
#include <LogicParse.h>
#include <tree_sanitizer.h>
#include <vector>
#include <string>

#include <liblogic/liblogic.h>
#include <gtest/gtest.h>


// -----------------------------
namespace eloquent::logic {
    TEST(ParserTest, see_return_of_parser)
    {
        std::string inputText = "P \\\\wedge Q";
        // Create an input stream from the input text
        antlr4::ANTLRInputStream input(inputText);

        // Create a lexer that feeds off of input CharStream
        LogicLex lexer(&input);

        // Create a buffer of tokens pulled from the lexer
        antlr4::CommonTokenStream tokens(&lexer);

        // Create a parser that feeds off the tokens buffer
        LogicParse parser(&tokens);

        // Begin parsing at startRule
        antlr4::tree::ParseTree *tree = parser.program();


        antlr4::tree::ParseTreeWalker walker;
        // Create and use your custom visitor
        TreeSanitizer listener;
        walker.walk(&listener,tree);
        auto& cleaned_tree = listener.cursor;

        CursorPrivate right;
        right.make_root();
        right.set_node(NodeBuilder::makeNewAndNode());
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("Q"));


        ASSERT_TRUE(cleaned_tree.are_trees_equal(right));
    }

    TEST(ParserTest, tautology)
    {
        std::string inputText = "P \\\\wedge \\\\top";
        // Create an input stream from the input text
        antlr4::ANTLRInputStream input(inputText);

        // Create a lexer that feeds off of input CharStream
        LogicLex lexer(&input);

        // Create a buffer of tokens pulled from the lexer
        antlr4::CommonTokenStream tokens(&lexer);

        // Create a parser that feeds off the tokens buffer
        LogicParse parser(&tokens);

        // Begin parsing at startRule
        antlr4::tree::ParseTree *tree = parser.program();


        antlr4::tree::ParseTreeWalker walker;
        // Create and use your custom visitor
        TreeSanitizer listener;
        walker.walk(&listener,tree);
        auto& cleaned_tree = listener.cursor;

        CursorPrivate right;
        right.make_root();
        right.set_node(NodeBuilder::makeNewAndNode());
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewTautologyNode());


        ASSERT_TRUE(cleaned_tree.are_trees_equal(right));
    }

    TEST(ParserTest, contradiction)
    {
        std::string inputText = "P \\\\wedge \\\\bot";
        // Create an input stream from the input text
        antlr4::ANTLRInputStream input(inputText);

        // Create a lexer that feeds off of input CharStream
        LogicLex lexer(&input);

        // Create a buffer of tokens pulled from the lexer
        antlr4::CommonTokenStream tokens(&lexer);

        // Create a parser that feeds off the tokens buffer
        LogicParse parser(&tokens);

        // Begin parsing at startRule
        antlr4::tree::ParseTree *tree = parser.program();


        antlr4::tree::ParseTreeWalker walker;
        // Create and use your custom visitor
        TreeSanitizer listener;
        walker.walk(&listener,tree);
        auto& cleaned_tree = listener.cursor;

        CursorPrivate right;
        right.make_root();
        right.set_node(NodeBuilder::makeNewAndNode());
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewAtomicNode("P"));
        right.up();
        right.spawn_new_child_node();
        right.set_node(NodeBuilder::makeNewContradictionNode());


        ASSERT_TRUE(cleaned_tree.are_trees_equal(right));
    }
}