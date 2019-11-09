#ifndef RIPS_STATE_MACHINE
#define RIPS_STATE_MACHINE

#include <utility>
#include <string>
#include "naive-lexer.hpp"
#include "tree.hpp"

class state_machine {
public:
    enum class state {
        initial,
        // INSERT INTO ...
        q1,
        q2,
        q3,
        q4,
        q5,
        q6,
        q7,
        q8,
        q9,
        q10,
        q11,
        // SELECT FROM ...
        p1,
        p2,
        p3,
        p4,
        p5,
        p6,
        p7,
        p8,
        p9,
        p10,
        p11,
        accept, // because `final' is a keyword
    };
    state_machine();
    ~state_machine() = default;
    void move(const std::pair<token, std::string>& inut);
    bool is_accepted() const;
    static std::string to_string(state);
    const tree& get_ast() const;
private:
    state m_current;
    tree m_ast;
};

#endif // RIPS_STATE_MACHINE_INSERT
