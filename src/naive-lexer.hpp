#ifndef RIPS_NAIVE_LEXER_HPP
#define RIPS_NAIVE_LEXER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iterator>
#include <regex>

enum class token {
    insert_into,    // ^\s*INSERT\s+INTO\s*
    select,         // ^\s*SELECT\s*
    from,           // ^\s*FROM\s*
    where,          // ^\s*WHERE\s*
    delete_,        // ^\s*DETELE\s*
    use,            // ^\s*USE\s*
    is_not,         // ^\s*IS\s*NOT\s*
    is,             // ^\s*IS\s*
    greater,        // ^\s*>\s*
    lesser,         // ^\s*<\s*
    equal,          // ^\s*=\s*
    order_by,       // ^\s*ORDER\s*BY\s*
    desc,           // ^\s*DESC\s*
    asc,            // ^\s*ASC\s*
    identifier,     // ^\s*[[:alpha:]_][[:alnum:]_]*\s*
    left_paren,     // ^\s*\(\s*
    right_paren,    // ^\s*\)\s*
    comma,          // ^\s*,\s*
    values,         // ^\s*VALUES\s*
    value,          // ^\s*([[:alpha:]_][[:alnum:]_]*(\(\))|[[:digit:]]+|\".*\")\s*
    semicolon,      // ^\s*;\s*
    invalid,
};

class naive_lexer {
public:
    naive_lexer();
    static std::string to_string(token t);
    std::vector<std::pair<token, std::string>> tokenize(std::string input);
    std::pair<token, std::string> next_token(std::string& input);
private:
    std::vector<std::pair<token, std::regex>> acceptors;
};

#endif // RIPS_NAIVE_LEXER_HPP