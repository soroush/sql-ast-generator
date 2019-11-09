#ifndef RIPS_TREE_HPP
#define RIPS_TREE_HPP

#include <vector>
#include <string>
#include <stack>

struct node {
    std::string label;
    std::string value;
    std::vector<node> children;
    std::string uid;
};

class tree {
public:
    tree() = default;
    ~tree() = default;
    void insert_init();
    void insert_set_table(const std::string& id);
    void insert_add_column(const std::string& column);
    void insert_add_value(const std::string& value);
    void select_init();
    void select_set_table(const std::string& id);
    void select_add_column(const std::string& column);
    void select_add_where_clause();
    void push_stack(const std::string& label, const std::string& value);
    // 
    void select_add_order_by_statement();
    void select_add_order_by(const std::string& field_name);
    void select_add_order_by_direction(const std::string& direction);
    void clause_to_postfix();
    // 
    void delete_init();
    void delete_set_table(const std::string& name);
    //
    void use_init();
    void use_set_database(const std::string& table_name);
    //
    void save(const std::string& path) const;
    std::string to_dot_graph() const;
private:
    node m_root;
    int m_max_uid;
    std::stack<node> m_stack;
};

#endif // RIPS_TREE_HPP
