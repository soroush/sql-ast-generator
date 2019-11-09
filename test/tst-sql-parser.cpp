#include <src/state-machine.hpp>
#include <string>
#include <sstream>
#include <fstream>

int main(int argc, char* argv[]) {
    // Prepare state machine and lexer
    naive_lexer l;
    state_machine m;
    std::ifstream infile{"queries.sql"};
    if(!infile.is_open()) {
        std::cerr << "Unable to open file!\n";
        return EXIT_FAILURE;
    }
    std::string line;
    while (std::getline(infile, line)) {
        auto all_tokens = l.tokenize(line);
        for(const auto& t : all_tokens) {
            m.move(t);
        }
        if(!m.is_accepted()) {
            std::cerr << "ERROR: Unable to parse: " 
            << line << '\n';
            return EXIT_FAILURE;
        }
    }
    return 0;
}
