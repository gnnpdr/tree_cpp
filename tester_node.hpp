#pragma once
#include "tester_base.hpp"
#include "tree.hpp"

class NodeTester : public ITester
{
    void test_def_ctor();
    void test_param_ctor();
    void test_copy_ctor();
    void test_assign_op();
    void test_dtor();
public:
    void run_all_tests() override
    {
        std::cout << "\n=== Testing Node ===" << std::endl;
        
        run_test("Default constructor", [this]() { test_def_ctor(); });
        run_test("Parameter constructor", [this]() { test_param_ctor(); });
        run_test("Copy constructor", [this]() { test_copy_ctor(); });
        run_test("Assignment operator", [this]() { test_assign_op(); });
        run_test("Destructor", [this]() { test_dtor(); });
    }
    
    void print_results() const override
    {
        int passed = 0;
        for (const auto& result : test_results_)
            if (result.passed) passed++;
            
        std::cout << "\nNode Tests: " << passed << "/" << test_results_.size() 
                  << " passed" << std::endl;
    }
};


void NodeTester::test_def_ctor()
{
    Node<int> node;
    
    ASSERT(node.key_ == 0, "Default key should be 0 for int");
    ASSERT(node.parent_ == nullptr, "Parent should be nullptr");
    ASSERT(node.left_ == nullptr, "Left child should be nullptr");
    ASSERT(node.right_ == nullptr, "Right child should be nullptr");
    ASSERT(node.colour_ == RED, "New node should be RED");
    
    Node<double> node_double;
    ASSERT(node_double.key_ == 0.0, "Default key should be 0.0 for double");
    
    Node<std::string> node_string;
    ASSERT(node_string.key_ == "", "Default key should be empty string");
}

void NodeTester::test_param_ctor()
{
    Node<int> node_int(42);
    ASSERT(node_int.key_ == 42, "Key should be 42");
    ASSERT(node_int.colour_ == RED, "New node should be RED");
    ASSERT(node_int.parent_ == nullptr, "Parent should be nullptr");
    
    Node<int> node_negative(-100);
    ASSERT(node_negative.key_ == -100, "Key should be -100");
}

void NodeTester::test_copy_ctor()
{
    Node<int> original(100);
    original.colour_ = BLACK;
    
    Node<int> copy(original);
    
    ASSERT(copy.key_ == 100, "Copied key should be 100");
    ASSERT(copy.colour_ == BLACK, "Colour should be copied");
    
    ASSERT(copy.parent_ == nullptr, "Parent should not be copied");
    ASSERT(copy.left_ == nullptr, "Left child should not be copied");
    ASSERT(copy.right_ == nullptr, "Right child should not be copied");
    
    original.key_ = 200;
    ASSERT(copy.key_ == 100, "Copy should be independent");
}
    
void NodeTester::test_assign_op()
{
    Node<int> node1(10);
    node1.colour_ = BLACK;
    
    Node<int> node2(20);
    
    node2 = node1;
    
    ASSERT(node2.key_ == 10, "Assigned key should be 10");
    ASSERT(node2.colour_ == BLACK, "Colour should be assigned");
    
    node1.key_ = 30;
    ASSERT(node2.key_ == 10, "Assigned node should be independent");
    
    node1 = node1;
    ASSERT(node1.key_ == 30, "Self-assignment should not change key");

    Node<int> node3(40);
    node3 = node2 = node1;
    ASSERT(node3.key_ == 30, "Chain assignment should work");
}
    
void NodeTester::test_dtor()
{
    {
        Node<int> node(123);
    }
    
    {
        Node<int>* parent = new Node<int>(1);
        Node<int>* left_child = new Node<int>(2);
        Node<int>* right_child = new Node<int>(3);
        
        parent->left_ = left_child;
        parent->right_ = right_child;
        left_child->parent_ = parent;
        right_child->parent_ = parent;
        
        delete parent;
        
        ASSERT(left_child->key_ == 2, "Left child should still exist");
        ASSERT(right_child->key_ == 3, "Right child should still exist");
        
        delete left_child;
        delete right_child;
    }
}