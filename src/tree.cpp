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
    node& from_node = find_or_create(m_root, "into", "into");
    from_node.children.emplace_back("table", name);
}

void tree::insert_init() {
    m_root = node{"statement", "insert"};
}

void tree::insert_add_column(const std::string& column) {
    node& found = find_or_create(m_root, "columns", "columns");
    found.children.emplace_back("column", column);
}

void tree::insert_add_value(const std::string& value) {
    node& found = find_or_create(m_root, "values", "values");
    found.children.emplace_back("value", value);
}

void tree::select_init() {
    m_root = node {"statement", "select"};
}

void tree::select_set_table(const std::string& name) {
    node& from_node = find_or_create(m_root, "from", "from");
    from_node.children.emplace_back("table", name);
}

void tree::select_add_column(const std::string& column) {
    node& found = find_or_create(m_root, "columns", "column");
    found.children.emplace_back("value", column);
}

void tree::push_stack(const std::string& label, const std::string& value) {
    m_stack.emplace(label, value);
}

void tree::select_add_order_by_statement() {
    find_or_create(m_root, "order_by", "order_by");
}

void tree::select_add_order_by(const std::string& field_name) {
    node& found = find_or_create(m_root, "order_by", "order_by");
    found.children.emplace_back("id", field_name);
}

void tree::select_add_order_by_direction(const std::string& direction) {
    node& found = find_or_create(m_root, "order_by", "order_by");
    found.children.emplace_back("direction", direction);
}

void tree::select_add_where_clause() {
    find_or_create(m_root, "where", "where");
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
    o << "\t\t\"" + n.id() + "\" [label=\"" + n_label + "\"]\n";
    for(const node& c : n.children) {
        print_nodes(c, o);
    }
}

void print_edges(const node& n, std::ostream& o) {
    for(const node& c : n.children) {
        o << "\t\t" << n.id() << " -> " << c.id() << '\n';
    }
    for(const node& c : n.children) {
        print_edges(c, o);
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
        str << "\tgraph [\n";
        str << "\t\tlabel = ";
        str << "\"" << label << "\"\n";
        str << "\t\tlabelloc = t\n";
        str << "\t]\n";
        str << "\tsubgraph {\n";
    }
    print_nodes(m_root, str);
    print_edges(m_root, str);
    str << "\t}\n";
    if(!legend.empty()) {
        str << "}\n";
    }
    return str.str();
}

void tree::reset() {
    // TODO: Check for memory leak (which does not exists)
    m_root.children.clear();
    m_root.label.clear();
    m_root.value.clear();
}

node& tree::find_or_create(node& parent, const std::string& label, const std::string& value) {
    auto predicate = [&](const node & n)->bool{return n.label == label;};
    auto found = std::find_if(parent.children.begin(),
                              parent.children.end(), predicate);
    if(found == parent.children.end()) {
        node new_node{label, value};
        parent.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    return *found;
}