#pragma once

// TODO: turn \\ into a single backslash

namespace eloquent::liblogic::symbols {
    constexpr char SYMB_NOT[] =  R"(\\neg)";
    constexpr char SYMB_AND[] = R"(\\wedge)";
    constexpr char SYMB_OR[] = R"(\\vee)";
    constexpr char SYMB_IMPL[] = R"(\\implies)";
    constexpr char SYMB_IFF[] = R"(\\iff)";
    constexpr char SYMB_LEQUI[] = R"(\\models)";

    constexpr char SYMB_TAUTOLOGY[] = R"(\\top)";
    constexpr char SYMB_CONTRADICTION[] = R"(\\bot)";



}
#define ELOQUENT_LIBLOGIC_SYMB_NOT     eloquent::liblogic::symbols::SYMB_NOT
#define ELOQUENT_LIBLOGIC_SYMB_AND     eloquent::liblogic::symbols::SYMB_AND
#define ELOQUENT_LIBLOGIC_SYMB_OR      eloquent::liblogic::symbols::SYMB_OR
#define ELOQUENT_LIBLOGIC_SYMB_IMPL    eloquent::liblogic::symbols::SYMB_IMPL
#define ELOQUENT_LIBLOGIC_SYMB_IFF     eloquent::liblogic::symbols::SYMB_IFF

#define ELOQUENT_LIBLOGIC_SYMB_TAUTOLOGY        eloquent::liblogic::symbols::SYMB_TAUTOLOGY
#define ELOQUENT_LIBLOGIC_SYMB_CONTRADICTION    eloquent::liblogic::symbols::SYMB_CONTRADICTION