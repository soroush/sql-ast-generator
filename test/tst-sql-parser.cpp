#include <src/state-machine.hpp>
#include <string>
#include <sstream>
#include <fstream>

int main(int argc, char* argv[]) {
    // Prepare state machine and lexer
    std::ifstream infile{"queries.sql"};
    if(!infile.is_open()) {
        std::cerr << "Unable to open file!\n";
        return EXIT_FAILURE;
    }
    std::string line;
    int lineno = 0;
    while(std::getline(infile, line)) {
        lineno++;
        naive_lexer l;
        state_machine m;
        m.reset();
        auto all_tokens = l.tokenize(line);
        for(const auto& t : all_tokens) {
            m.move(t);
        }
        if(!m.is_accepted()) {
            std::cerr << "ERROR: Unable to parse: "
                      << line << '\n';
            return EXIT_FAILURE;
        } else {
            m.get_ast().save("statement_" + std::to_string(lineno) + ".dot",
                             line);
        }
    }
    return 0;
}
