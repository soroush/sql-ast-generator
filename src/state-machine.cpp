#include "state-machine.hpp"

state_machine::state_machine() :
    m_current{state_machine::state::initial} {
}

void state_machine::move(const std::pair<token, std::string>& input) {
    const token t = input.first;
    const std::string v = input.second;
    std::cout << "STATE: " << to_string(m_current)
              << "\nTOKEN : " << naive_lexer::to_string(t) << "\n\n";
    switch(m_current) {
    case state::initial:
        if(t == token::insert_into) {
            m_ast.insert_init();
            m_current = state::q1;
        }
        else if(t == token::select) {
            m_ast.select_init();
            m_current = state::p1;
        }
        break;
    case state::q1:
        if(t == token::identifier) {
            m_ast.insert_set_table(v);
            m_current = state::q2;
        }
        break;
    case state::q2:
        if(t == token::left_paren) {
            m_current = state::q3;
        }
        break;
    case state::q3:
        if(t == token::identifier) {
            m_ast.insert_add_column(v);
            m_current = state::q4;
        }
        break;
    case state::q4:
        if(t == token::comma) {
            m_current = state::q3;
        }
        else if(t == token::right_paren) {
            m_current = state::q5;
        }
        break;
    case state::q5:
        if(t == token::values) {
            m_current = state::q6;
        }
        break;
    case state::q6:
        if(t == token::left_paren) {
            m_current = state::q7;
        }
        break;
    case state::q7:
        if(t == token::value) {
            m_ast.insert_add_value(v);
            m_current = state::q10;
        }
        else if(t == token::identifier) {
            // Function name
            m_ast.insert_add_value(v + "()");
            m_current = state::q8;
        }
        break;
    case state::q8:
        if(t == token::left_paren) {
            m_current = state::q9;
        }
        break;
    case state::q9:
        if(t == token::right_paren) {
            m_current = state::q10;
        }
        break;
    case state::q10:
        if(t == token::right_paren) {
            m_current = state::q11;
        }
        else if(t == token::comma) {
            m_current = state::q7;
        }
        break;
    case state::q11:
        if(t == token::semicolon) {
            m_current = state::accept;
        }
        break;
    case state::p1:
        if(t == token::identifier) {
            m_ast.insert_add_column(v);
            m_current = state::p2;
        }
        break;
    case state::p2:
        if(t == token::comma) {
            m_current = state::p1;
        }
        else if(t == token::from) {
            m_current = state::p3;
        }
        break;
    case state::p3:
        if(t == token::identifier) {
            m_ast.insert_set_table(v);
            m_current = state::p4;
        }
        break;
    case state::p4:
        if(t == token::where) {
            m_ast.select_add_where_clause();
            m_current = state::p5;
        }
        else if(t == token::from) {
            m_current = state::p3;
        }
        break;
    case state::p5:
        if(t == token::identifier) {
            m_ast.push_stack("where_field", v);
            m_current = state::p6;
        }
        break;
    case state::p6:
        switch(t) {
        case token::is:
        case token::is_not:
        case token::greater:
        case token::lesser:
        case token::equal:
            m_ast.push_stack("where_operator", v);
            m_current = state::p7;
            break;
        default:
            break;
        }
        break;
    case state::p7:
        if(t == token::value) {
            m_ast.push_stack("where_value", v);
            m_ast.clause_to_postfix();
            m_current = state::p8;
        }
        break;
    case state::p8:
        switch(t) {
        case token::semicolon:
            m_current = state::accept;
            break;
        case token::order_by:
            m_ast.select_add_order_by_statement();
            m_current = state::p9;
            break;
        default:
            break;
        }
        break;
    case state::p9:
        if(t == token::identifier) {
            m_ast.select_add_order_by(v);
            m_current = state::p10;
        }
        break;
    case state::p10:
        switch(t) {
        case token::semicolon:
            m_current = state::accept;
            break;
        case token::desc:
        case token::asc:
            m_ast.select_add_order_by_direction(v);
            m_current = state::p11;
            break;
        default:
            break;
        }
        break;
    case state::p11:
        if(t == token::semicolon) {
            m_current = state::accept;
        }
        break;
    case state::accept:
        break;
    default:
        break;
    }
}

bool state_machine::is_accepted() const {
    return m_current == state::accept;
}

std::string state_machine::to_string(state s) {
    switch(s) {
    case state::accept:
        return "accept";
    case state::initial :
        return "initial";
    case state::q1 :
        return "q1";
    case state::q2 :
        return "q2";
    case state::q3 :
        return "q3";
    case state::q4 :
        return "q4";
    case state::q5 :
        return "q5";
    case state::q6 :
        return "q6";
    case state::q7 :
        return "q7";
    case state::q8 :
        return "q8";
    case state::q9 :
        return "q9";
    case state::q10 :
        return "q10";
    case state::q11 :
        return "q11";
    case state::p1 :
        return "p1";
    case state::p2 :
        return "p2";
    case state::p3 :
        return "p3";
    case state::p4 :
        return "p4";
    case state::p5 :
        return "p5";
    case state::p6 :
        return "p6";
    case state::p7 :
        return "p7";
    case state::p8 :
        return "p8";
    case state::p9 :
        return "p9";
    case state::p10 :
        return "p10";
    case state::p11 :
        return "p11";
    default:
        return "INVALID";
    }
}

const tree& state_machine::get_ast() const {
    return m_ast;
}