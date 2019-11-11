#include "tree.hpp"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

node::node():
    label{""},
    value{""} {
    uid = next_id();
}

node::node(const std::string& label_, const std::string& value_):
    label{label_},
    value{value_} {
    uid = next_id();
}

std::string node::id() const {
    return uid;
}

std::string node::next_id() {
    static int max_id = 0;
    return "node_" + std::to_string(++max_id);
}

void tree::insert_set_table(const std::string& name) {
    node insert{"table", "table"};
    node table{"table", name};
    insert.children.push_back(table);
    m_root.children.push_back(insert);
}

void tree::insert_init() {
    m_root = node{"statement", "insert"};
}

void tree::insert_add_column(const std::string& column) {
    // Find or create columns root node
    auto predicate = [](const node & n)->bool{return n.label == "columns";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node{"columns", "columns"};
        m_root.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    node column_node {"column", column};
    found->children.push_back(column_node);
}

void tree::insert_add_value(const std::string& value) {
    // Find or create values root node
    auto predicate = [](const node & n)->bool{return n.label == "values";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node {"values", "values"};
        m_root.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    node value_node {"value", value};
    found->children.push_back(value_node);
}

void tree::select_init() {
    m_root = node {"statement", "select"};
}

void tree::select_set_table(const std::string& name) {
    node from_node {"from", "from"};
    m_root.children.push_back(from_node);
    node table_node{"table", name};
    table_node.children.push_back(table_node);
}

void tree::select_add_column(const std::string& column) {
    // Find or create columns root node
    auto predicate = [](const node & n)->bool{return n.label == "columns";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node{"columns", "columns"};
        m_root.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    node value_node {"value", column};
    found->children.push_back(value_node);
}

void tree::push_stack(const std::string& label, const std::string& value) {
    node new_node {label, value};
    m_stack.push(new_node);
}

void tree::select_add_order_by_statement() {
    // Find or create order by root node
    auto predicate = [](const node & n)->bool{return n.label == "order_by";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node {"order_by", "order_by"};
        m_root.children.push_back(new_node);
    }
}

void tree::select_add_order_by(const std::string& field_name) {
    select_add_order_by_statement();
    auto predicate = [](const node & n)->bool{return n.label == "order_by";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    node new_node{"id", field_name};
    found->children.push_back(new_node);
}

void tree::select_add_order_by_direction(const std::string& direction) {
    select_add_order_by_statement();
    auto predicate = [](const node & n)->bool{return n.label == "order_by";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    node new_node{"direction", direction};
    found->children.push_back(new_node);
}

void tree::select_add_where_clause() {
    // Find or create "where" root node
    auto predicate = [](const node & n)->bool{return n.label == "where";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node {"where", "where"};
        m_root.children.push_back(new_node);
    }
}

void tree::clause_to_postfix() {
    // Find "where" root node
    auto predicate = [](const node & n)->bool{return n.label == "where";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    // A mini shunting yard play, single statement,
    // no parenthesis, no predecences,
    std::list<node> outputs;
    std::list<node> operators;
    while(!m_stack.empty()) {
        const std::string& label = m_stack.top().label;
        if(label == "where_operator") {
            operators.push_back(m_stack.top());
        } else if(label == "where_field" || label == "where_value") {
            outputs.push_back(m_stack.top());
        }
        m_stack.pop();
    }
    while(!operators.empty()) {
        node op = operators.back();
        operators.pop_back();
        // operand #1
        // CAUTION: Pay attetion to directions. Must be stack-like
        node op1 = outputs.back();
        outputs.pop_back();
        // operand #2
        node op2 = outputs.back();
        outputs.pop_back();
        // Add operands
        op.children.push_back(op1);
        op.children.push_back(op2);
        found->children.push_back(op);
    }
}

void tree::delete_init() {
    m_root = node{"statement", "delete"};
}

void tree::delete_set_table(const std::string& name) {
    node delete_node {"table", "table"};
    node table{"table", name};
    delete_node.children.push_back(table);
    m_root.children.push_back(delete_node);
}

void tree::use_init() {
    m_root = node{"statement", "use"};
}

void tree::use_set_database(const std::string& dbname) {
    node use_node{"database", dbname};
    m_root.children.push_back(use_node);
}

std::string replace_all(std::string& input,
                        const std::string& find,
                        const std::string& replace) {
    std::size_t start_pos = 0;
    while((start_pos = input.find(find, start_pos)) != std::string::npos) {
        input.replace(start_pos, find.length(), replace);
        start_pos += replace.length();
    }
    return input;
}

void print_nodes(const node& n, std::ostream& o) {
    std::string n_label = n.value;
    n_label = replace_all(n_label, "\"", "\\\"");
    o << "\"" + n.id() + "\" [label=\"" + n_label + "\"]\n";
    for(const node& c : n.children) {
        print_nodes(c, o);
    }
}

void print_helper(const node& n, std::ostream& o) {
    for(const node& c : n.children) {
        o << n.id() << " -> " << c.id() << '\n';
    }
    for(const node& c : n.children) {
        print_helper(c, o);
    }
}

void tree::save(const std::string& path, const std::string& legend) const {
    std::ofstream f{path};
    if(!f.is_open()) {
        std::cerr << "Unable to open file: " << path << "!\n";
        return;
    }
    f << to_dot_graph(legend);
    f.close();
}

std::string tree::to_dot_graph(const std::string& legend) const {
    std::stringstream str;
    str << "digraph G {\n";
    if(!legend.empty()) {
        std::string label = legend;
        replace_all(label, "\"", "\\\"");
        str << "\tgraph [\n\tlabel = ";
        str << "\"" << label << "\"\n";
        str << "\tlabelloc = t\n";
        str << "\t]\n";
        str << "\tsubgraph {\n";
    }
    print_nodes(m_root, str);
    print_helper(m_root, str);
    str << "}\n";
    if(!legend.empty()) {
        str << "}\n";
    }
    return str.str();
}

void tree::reset() {
    m_root.children.clear();
    m_root.label.clear();
    m_root.value.clear();
}