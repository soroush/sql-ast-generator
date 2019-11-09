#include "naive-lexer.hpp"
#include "state-machine.hpp"

int main(int argc, char const* argv[]) {
    //std::string sample = "INSERT INTO user_notes (id, user_id, note, created) VALUES (1, 1, \"Note 1\", NOW());";
    std::string sample = "SELECT id, name, address FROM users WHERE is_customer IS NOT NULL ORDER BY created;";
    naive_lexer l;
    state_machine m;
    auto all_tokens = l.tokenize(sample);
    for(const auto& t : all_tokens) {
        m.move(t);
    }
    std::cout << m.is_accepted() << '\n';
    m.get_ast().save("insert-ast.dot");
}
