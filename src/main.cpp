#include "naive-lexer.hpp"
#include "state-machine.hpp"

int main(int argc, char const* argv[]) {
    //std::string sample = "INSERT INTO user_notes (id, user_id, note, created) VALUES (1, 1, \"Note 1\", NOW());";
    // std::string sample = "SELECT id, name, address FROM users WHERE is_customer IS NOT NULL ORDER BY created;";
    //std::string sample = "DELETE FROM database2 WHERE id < 1000;";
    // std::string sample = "USE db1;";
    // Read input
    std::string input;
    while(std::getline(std::cin, input)) {
        naive_lexer l;
        state_machine m;
        m.reset();
        auto all_tokens = l.tokenize(input);
        for(const auto& t : all_tokens) {
            m.move(t);
        }
        if(m.is_accepted()) {
            std::cout << m.get_ast().to_dot_graph(input);
        } else {
            std::cerr << "Input string is not valid!\n";
        }
    }
}
