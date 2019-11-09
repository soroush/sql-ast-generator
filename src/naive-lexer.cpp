#include "naive-lexer.hpp"

naive_lexer::naive_lexer() {
    acceptors.emplace_back(token::insert_into, std::regex{"^\\s*INSERT\\s+INTO\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::values, std::regex{"^\\s*VALUES\\s*", std::regex_constants::icase});
    acceptors.emplace_back(token::select, std::regex{"^\\s*SELECT\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::from, std::regex{"^\\s*FROM\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::where, std::regex{"^\\s*WHERE\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::delete_, std::regex{"^\\s*DELETE\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::use, std::regex{"^\\s*USE\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::is_not, std::regex{"^\\s*IS\\s*NOT\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::is, std::regex{"^\\s*IS\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::order_by, std::regex{"^\\s*ORDER\\s*BY\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::asc, std::regex{"^\\s*ASC\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::desc, std::regex{"^\\s*DESC\\s+", std::regex_constants::icase});
    acceptors.emplace_back(token::greater, std::regex{"^\\s*>\\s*"});
    acceptors.emplace_back(token::lesser, std::regex{"^\\s*<\\s*"});
    acceptors.emplace_back(token::equal, std::regex{"^\\s*=\\s*"});
    acceptors.emplace_back(token::left_paren, std::regex{"^\\s*\\(\\s*"});
    acceptors.emplace_back(token::right_paren, std::regex{"^\\s*\\)\\s*"});
    acceptors.emplace_back(token::comma, std::regex{"^\\s*,\\s*"});
    acceptors.emplace_back(token::value, std::regex{"^\\s*([[:alpha:]_][[:alnum:]_]*(\\(\\))|[[:digit:]]+|\\\".*\\\"|NULL)\\s*",
                           std::regex_constants::icase});
    acceptors.emplace_back(token::identifier, std::regex{"^\\s*[[:alpha:]_][[:alnum:]_]*\\s*",
                           std::regex_constants::icase});
    acceptors.emplace_back(token::semicolon, std::regex{"^\\s*;\\s*"});
}

std::string naive_lexer::to_string(token t) {
    switch(t) {
    case token::insert_into:
        return "insert_into";
    case token::identifier:
        return "[identifier]";
    case token::left_paren:
        return "left_paren";
    case token::right_paren:
        return "right_paren";
    case token::semicolon:
        return "semicolon";
    case token::values:
        return "values";
    case token::value:
        return "[value]";
    case token::comma:
        return "comma";
    case token::select:
        return "select";
    case token::from:
        return "from";
    case token::where:
        return "where";
    case token::is_not:
        return "is_not";
    case token::is:
        return "is";
    case token::order_by:
        return "order_by";
    case token::asc:
        return "asc";
    case token::desc:
        return "desc";
    case token::greater:
        return "greater";
    case token::lesser:
        return "lesser";
    case token::equal:
        return "equal";
    case token::delete_:
        return "delete";
    case token::use:
        return "use";
    default:
        return "[invalid]";
        break;
    }
}

std::vector<std::pair<token, std::string>> naive_lexer::tokenize(std::string& input) {
    std::vector<std::pair<token, std::string>> tokens;
    bool found;
    do {
        found = false;
        for(const auto& item : acceptors) {
            const std::regex acceptor = item.second;
            std::smatch base_match;
            if(std::regex_search(input, base_match, acceptor)) {
                // consume and push token
                tokens.push_back(std::make_pair(item.first, base_match[0].str()));
                input = std::regex_replace(input, acceptor, "");
                // std::cout << " REST: " << input << '\n';
                found = true;
                break;
            }
        }
    }
    while(found);
    return tokens;
}

std::pair<token, std::string> naive_lexer::next_token(std::string& input) {
    for(const auto& item : acceptors) {
        const std::regex acceptor = item.second;
        std::smatch base_match;
        if(std::regex_search(input, base_match, acceptor)) {
            // consume and push token
            auto token = std::make_pair(item.first, base_match[0].str());
            input = std::regex_replace(input, acceptor, "");
            return token;
        }
    }
    // If not found:
    return std::make_pair(token::invalid, "");
}
