#include "tree.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>

void tree::insert_set_table(const std::string& name) {
    node insert;
    insert.label = "table";
    insert.value = "table";
    insert.uid = "node_" + std::to_string(++m_max_uid);
    node table;
    table.label = "table";
    table.value = name;
    table.uid = "node_" + std::to_string(++m_max_uid);
    insert.children.push_back(table);
    m_root.children.push_back(insert);
}

void tree::insert_init() {
    m_root.children.clear();
    m_root.label = "statement";
    m_root.value = "insert";
    m_max_uid = 1;
    m_root.uid = "node_" + std::to_string(m_max_uid);
}

void tree::insert_add_column(const std::string& column) {
    // Find or create columns root node
    auto predicate = [](const node & n)->bool{return n.label == "columns";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node;
        new_node.label = "columns";
        new_node.value = "columns";
        new_node.uid = "node_" + std::to_string(++m_max_uid);
        m_root.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    node column_node;
    column_node.label = "column";
    column_node.value = column;
    column_node.uid = "node_" + std::to_string(++m_max_uid);
    found->children.push_back(column_node);
}

void tree::insert_add_value(const std::string& value) {
    // Find or create values root node
    auto predicate = [](const node & n)->bool{return n.label == "values";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node;
        new_node.label = "values";
        new_node.value = "values";
        new_node.uid = "node_" + std::to_string(++m_max_uid);
        m_root.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    node value_node;
    value_node.label = "value";
    value_node.value = value;
    value_node.uid = "node_" + std::to_string(++m_max_uid);
    found->children.push_back(value_node);
}

void tree::select_init() {
    m_root.children.clear();
    m_root.label = "statement";
    m_root.value = "select";
    m_max_uid = 1;
    m_root.uid = "node_" + std::to_string(m_max_uid);
}

void tree::select_set_table(const std::string& name) {
    node from_node;
    from_node.label = "from";
    from_node.uid = "node_" + std::to_string(++m_max_uid);
    m_root.children.push_back(from_node);
    node table_node;
    table_node.label = "table";
    table_node.value = "name";
    table_node.uid = "node_" + std::to_string(++m_max_uid);
    table_node.children.push_back(table_node);
}

void tree::select_add_column(const std::string& column) {
    // Find or create columns root node
    auto predicate = [](const node & n)->bool{return n.label == "columns";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node;
        new_node.label = "columns";
        new_node.uid = "node_" + std::to_string(++m_max_uid);
        m_root.children.push_back(new_node);
        found = std::prev(m_root.children.end());
    }
    node value_node;
    value_node.label = "value";
    value_node.value = column;
    value_node.uid = "node_" + std::to_string(++m_max_uid);
    found->children.push_back(value_node);
}

void tree::push_stack(const std::string& label, const std::string& value) {
    node new_node;
    new_node.label = label;
    new_node.value = value;
    new_node.uid = "node_" + std::to_string(++m_max_uid);
    m_stack.push(new_node);
}

void tree::select_add_order_by_statement() {
    // Find or create order by root node
    auto predicate = [](const node & n)->bool{return n.label == "order_by";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node;
        new_node.label = "order_by";
        new_node.value = "order_by";
        new_node.uid = "node_" + std::to_string(++m_max_uid);
        m_root.children.push_back(new_node);
    }
}

void tree::select_add_order_by(const std::string& field_name) {
    select_add_order_by_statement();
    auto predicate = [](const node & n)->bool{return n.label == "order_by";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    node new_node;
    new_node.label = "id";
    new_node.value = field_name;
    new_node.uid = "node_" + std::to_string(++m_max_uid);
    found->children.push_back(new_node);
}

void tree::select_add_order_by_direction(const std::string& direction) {
    select_add_order_by_statement();
    auto predicate = [](const node & n)->bool{return n.label == "order_by";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    node new_node;
    new_node.label = "direction";
    new_node.value = direction;
    new_node.uid = "node_" + std::to_string(++m_max_uid);
    found->children.push_back(new_node);
}

void tree::select_add_where_clause() {
    // Find or create "where" root node
    auto predicate = [](const node & n)->bool{return n.label == "where";};
    auto found = std::find_if(m_root.children.begin(),
                              m_root.children.end(), predicate);
    if(found == m_root.children.end()) {
        node new_node;
        new_node.label = "where";
        new_node.value = "where";
        new_node.uid = "node_" + std::to_string(++m_max_uid);
        m_root.children.push_back(new_node);
    }
}

// void tree::select_add_where_field(const std::string& field_name) {
//     // Find "where" root node
//     auto predicate = [](const node & n)->bool{return n.label == "where";};
//     auto found = std::find_if(m_root.children.begin(),
//                               m_root.children.end(), predicate);
//     node new_node;
//     new_node.label = "where_field";
//     new_node.value = field_name;
//     new_node.uid = "node_" + std::to_string(++m_max_uid);
//     found->children.push_back(new_node);
// }

// void tree::select_add_where_operator(const std::string& op) {
//     // Find "where" root node
//     auto predicate = [](const node & n)->bool{return n.label == "where";};
//     auto found = std::find_if(m_root.children.begin(),
//                               m_root.children.end(), predicate);
//     // Because of infix notation, there will be an id or value before operator:
//     node new_node;
//     new_node.label = "where_operator";
//     new_node.value = op;
//     new_node.uid = "node_" + std::to_string(++m_max_uid);
//     if(!found->children.empty()) {
//         new_node.children = found->children;
//         found->children.clear();
//     }
//     found->children.push_back(new_node);
// }

// void tree::select_add_where_value(const std::string& val) {
//     // Find "where" root node
//     auto predicate = [](const node & n)->bool{return n.label == "where";};
//     auto found = std::find_if(m_root.children.begin(),
//                               m_root.children.end(), predicate);
//     node new_node;
//     new_node.label = "where_value";
//     new_node.value = val;
//     new_node.uid = "node_" + std::to_string(++m_max_uid);
//     found->children.push_back(new_node);
// }

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
        if(label=="where_operator") {
            operators.push_back(m_stack.top());
        }
        else if(label=="where_field" || label=="where_value") {
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

void print_nodes(const node& n, std::ofstream& o) {
    std::string n_label = n.value;
    n_label = replace_all(n_label, "\"", "\\\"");
    o << "\"" + n.uid + "\" [label=\"" + n_label + "\"]\n";
    for(const node& c : n.children) {
        print_nodes(c, o);
    }
}

void print_helper(const node& n, std::ofstream& o) {
    for(const node& c : n.children) {
        o << n.uid << " -> " << c.uid << '\n';
    }
    for(const node& c : n.children) {
        print_helper(c, o);
    }
}

void tree::save(const std::string& path) const {
    std::ofstream f{path};
    if(!f.is_open()) {
        std::cerr << "Unable to open file: " << path << "!\n";
        return;
    }
    f << "digraph G {\n";
    print_nodes(m_root, f);
    print_helper(m_root, f);
    f << "}\n";
    f.close();
}