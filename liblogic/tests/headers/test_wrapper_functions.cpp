//
// Created by xcell on 24.02.2025.
//

#include "test_wrapper_functions.h"
#include <ANTLRInputStream.h>
#include <LogicLex.h>
#include <LogicParse.h>
#include <../../src/transformations/n-ization.h>

namespace eloquent::logic::tests{
    // NOW: with optional argument to N-ize the expression out of the box!
    CursorPrivate build_tree(const std::string& exp, const bool n_izer_switch){
        // Create an input stream from the input expression
        antlr4::ANTLRInputStream input(exp);

        // Create a lexer that feeds off of input CharStream
        LogicLex lexer(&input);

        // Create a buffer of tokens pulled from the lexer
        antlr4::CommonTokenStream tokens(&lexer);

        // Create a parser that feeds off the tokens buffer
        LogicParse parser(&tokens);

        // Begin parsing at startRule
        antlr4::tree::ParseTree *tree = parser.program();

        antlr4::tree::ParseTreeWalker walker;

        // Create and use our custom visitor
        TreeSanitizer listener;
        walker.walk(&listener,tree);
        auto& cleanedtree = listener.cursor; // the root of the cleaned tree

        // Do we want our tree to be N-ized straight off the bat?
        // EXPERIMENTAL - to be developed to account for larger trees.
        if (n_izer_switch) {
            N_izer fixer;
            fixer.replace(cleanedtree.root);
        }

        return cleanedtree;
    }

    std::vector<CursorPrivate> build_tree_array(const std::vector<std::string>& battery, const bool n_izer_switch) {
        std::vector<CursorPrivate> result;
        // I agree this line is weird, but CLion suggested it, and it doesn't seem to hurt, so I'm leaving it.
        result.reserve(battery.size());
        for (const auto& exp : battery) { result.push_back(build_tree(exp, n_izer_switch)); }
        return result;
    }

    // This may look weird, but the reason I decided to make this function is that
    // I want to keep the test file as clean and as short as possible.
    // I feel like a for-loop at the end would be ugly.
    bool are_tree_arrays_equal(const std::vector<CursorPrivate>& battery, const std::vector<CursorPrivate>& answers) {
        if (battery.size() != answers.size()) {
            std::cout << "[x] Sizes don't match! Check your commas, maybe.\n";
            return false;
        }

        bool result = true;

        for (size_t i = 0; i < battery.size(); i++) {
            if (battery[i].are_trees_equal(answers[i])) {
                std::cout << "[+] Expression " << i+1 << " passed.\n";
            } else {
                std::cout << "[-] Expression " << i+1 << " failed.\n";
                result = false;
            }
        }
        return result;
    }
}
