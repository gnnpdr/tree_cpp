#pragma once
#include "tester_base.hpp"
#include "tree.hpp"


class RBTreeTester : public ITester
{
    void test_def_ctor();
    void test_copy_ctor();
    void test_assign();
    void test_dtor();
    void test_add_node();
    void test_balance();

public:
    void run_all_tests() override
    {
        std::cout << "Testing RBTree" << std::endl;

        run_test("def_ctor", [this]() { test_def_ctor(); });
        run_test("copy_ctor", [this]() {test_copy_ctor(); });
        run_test("assign", [this]() { test_assign(); });
        run_test("dtor", [this]() { test_dtor(); });
        run_test("add_node", [this]() {test_add_node(); });
        run_test("balance", [this] () { test_balance(); });
    }

    void print_results() const override
    {
        int passed = 0;
        for (const auto& result : test_results_)
            if (result.passed) passed++;
            
        std::cout << "\nRBTree Tests: " << passed << "/" << test_results_.size() 
                  << " passed" << std::endl;
    }
};

void RBTreeTester::test_def_ctor()
{
    RBTree<int> tree;

    ASSERT(tree.root_ == nullptr, "Default root should be nullptr");

    tree.add_node(42);
    ASSERT(tree.root_ != nullptr, "Root should exist after insert");
    ASSERT(tree.root_->key_ == 42, "Root key should be 42");
}

void RBTreeTester::test_copy_ctor()
{
    RBTree<int> original;
    original.add_node(3);
    original.add_node(5);
    original.add_node(7);

    RBTree<int> copy(original);
    ASSERT(original.root_ != nullptr, "Original tree should not be empty");
    ASSERT(copy.root_ != nullptr, "Copied tree should not be empty");
    ASSERT(copy.root_ != nullptr, "Copied tree should not be empty");
    ASSERT(copy.root_ != original.root_, "Should be deep copy (different roots)");
    ASSERT(copy.root_->colour_ == original.root_->colour_, "colour should be copied");
    ASSERT(copy.root_->key_ == original.root_->key_, "key should be copied");
    original.root_->key_ = 7;
    ASSERT(copy.root_->key_ != 7, "copy should be independent");
}


void RBTreeTester::test_assign()
{
    RBTree<int> tree1;
    tree1.add_node(10);
    tree1.add_node(5);
    tree1.add_node(15);
        
    RBTree<int> tree2;
    tree2.add_node(999);
        
    tree2 = tree1;
        
    ASSERT(tree2.root_ != nullptr, "Assigned tree should not be empty");
    ASSERT(tree2.root_ != tree1.root_, "Should be deep copy");

        
    RBTree<int> tree3;
    tree3.add_node(100);
    tree3 = RBTree<int>();
    ASSERT(tree3.root_ == nullptr, "Should clear old content");
}
    
void RBTreeTester::test_dtor()
{
    {
        RBTree<int> empty_tree;
    }
        
    {
        RBTree<int> tree;
        for (int i = 0; i < 100; ++i)
        {
            tree.add_node(i);
        }
    }

    {
        RBTree<int> tree1;
        tree1.add_node(50);
        tree1.add_node(25);
        tree1.add_node(75);
            
        {
            RBTree<int> tree2 = tree1;
        }
        
        ASSERT(tree1.root_ != nullptr, "Original tree should still exist");
    }
}
    
void RBTreeTester::test_add_node()
{
    RBTree<int> tree;
        
    tree.add_node(50);
    ASSERT(tree.root_ != nullptr, "Root should exist");
    ASSERT(tree.root_->key_ == 50, "Root key should be 50");
    ASSERT(tree.root_->colour_ == BLACK, "Root must be black");
        
    tree.add_node(30);
    ASSERT(tree.root_->left_ != nullptr, "Left child should exist");
    ASSERT(tree.root_->left_->key_ == 30, "Left child key should be 30");
        
    tree.add_node(70);
    ASSERT(tree.root_->right_ != nullptr, "Right child should exist");
}

void RBTreeTester::test_balance()
{
    RBTree<int> tree;
    tree.add_node(1);
    ASSERT(tree.root_->colour_ == BLACK, "1 root should be black");
    tree.add_node(2);
    ASSERT(tree.root_->colour_ == BLACK, "2 root should be black");
    ASSERT(tree.root_->right_->key_ == 2, "sh 2");
    ASSERT(tree.root_->right_->colour_ == RED, "sh RED");
    tree.add_node(3);
    ASSERT(tree.root_->colour_ == BLACK, "3 root should be black");
    ASSERT(tree.root_->key_ == 2, "root sh 2");
    ASSERT(tree.root_->right_->colour_ == RED, "right sh RED");
    ASSERT(tree.root_->right_->key_ == 3, "right sh 3");
    ASSERT(tree.root_->left_->colour_ == RED, "left sh RED");
    ASSERT(tree.root_->left_->key_ == 1, "left sh 1");
    tree.add_node(4);
    ASSERT(tree.root_->colour_ == BLACK, "4 root should be black");
    ASSERT(tree.root_->key_ == 2, "root sh 2");
    ASSERT(tree.root_->right_->colour_ == BLACK, "right sh BLACK");
    ASSERT(tree.root_->right_->key_ == 3, "right sh 3");
    ASSERT(tree.root_->right_->right_->colour_ == RED, "right right sh RED");
    ASSERT(tree.root_->right_->right_->key_ == 4, "right right sh 4");
    ASSERT(tree.root_->left_->colour_ == BLACK, "left sh BLACK");
    ASSERT(tree.root_->left_->key_ == 1, "left sh 1");
    tree.add_node(5);
    ASSERT(tree.root_->colour_ == BLACK, "5 root should be black");
    ASSERT(tree.root_->key_ == 2, "root sh 2");
    ASSERT(tree.root_->right_->colour_ == BLACK, "right sh BLACK");
    ASSERT(tree.root_->right_->key_ == 4, "right sh 4");
    ASSERT(tree.root_->right_->right_->colour_ == RED, "right right sh RED");
    ASSERT(tree.root_->right_->right_->key_ == 5, "right right sh 5");
    ASSERT(tree.root_->right_->left_->colour_ == RED, "right left sh RED");
    ASSERT(tree.root_->right_->left_->key_ == 3, "right left sh 3");
    ASSERT(tree.root_->left_->colour_ == BLACK, "left sh BLACK");
    ASSERT(tree.root_->left_->key_ == 1, "left sh 1");

    tree.print_tree(tree.root_);

}