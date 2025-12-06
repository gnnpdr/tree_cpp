#pragma once
#include "tester_base.hpp"
#include "tree.hpp"


class RBTreeTester : public ITester
{
    void test_def_ctor();
    void test_param_ctor();
    void test_copy_ctor();
    void test_assign();
    void test_dtor();
    void test_add_node();

public:
    void run_all_tests() override
    {
        std::cout << "Testing RBTree" << std::endl;

        run_test("def_ctor", [this]() { test_def_ctor(); });
        run_test("param ctor", [this]() { test_param_ctor(); });
        run_test("copy_ctor", [this]() {test_copy_ctor(); });
        run_test("assign", [this]() { test_assign(); });
        run_test("dtor", [this]() { test_dtor(); });
        run_test("add_node", [this]() {test_add_node(); });
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

void RBTreeTester::test_param_ctor()
{
    Node<int>* root = new Node<int>(5);

    RBTree<int> root_tree(root);

    ASSERT(root_tree.root_ == root, "ptrs should be equal");
    
    Node<int>* left = new Node<int>(3);
    Node<int>* right = new Node<int>(8);

    root->left_ = left;
    root->right_ = right;

    RBTree<int> tree(root);

    ASSERT(tree.root_->left_ == left, "left child should be");
    ASSERT(tree.root_->right_ == right, "right child should be");
}

void RBTreeTester::test_copy_ctor()
{
    RBTree<int> original;
    original.add_node(3);
    original.add_node(5);
    original.add_node(7);

    RBTree<int> tree(original);

    ASSERT(tree.root_ != nullptr, "Copied tree should not be empty");
    ASSERT(tree.root_ != original.root_, "Should be deep copy (different roots)");
    ASSERT(tree.root_->colour_ == original.root_->colour_, "colour should be copied");
    ASSERT(tree.root_->key_ == original.root_->key_, "key should be copied");

    ASSERT(tree.root_->left_->key_ = original.root_->left_->key_, "left key should be copied");

    ASSERT(tree.root_->left_ != original.root_->left_, "diff ptrs");

    original.root_->key_ = 7;

    ASSERT(tree.root_->key_ == 7, "copy should be independent");
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
        
    //tree1 = tree1;
    //ASSERT(check_rb_properties(tree1), "Self-assignment should keep tree valid");
        
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
        
    // Вставка первого узла (становится корнем)
    tree.add_node(50);
    ASSERT(tree.root_ != nullptr, "Root should exist");
    ASSERT(tree.root_->key_ == 50, "Root key should be 50");
    ASSERT(tree.root_->colour_ == BLACK, "Root must be black");
        
    // Вставка второго узла
    tree.add_node(30);
    ASSERT(tree.root_->left_ != nullptr, "Left child should exist");
    ASSERT(tree.root_->left_->key_ == 30, "Left child key should be 30");
        
    // Вставка третьего узла
    tree.add_node(70);
    ASSERT(tree.root_->right_ != nullptr, "Right child should exist");
        
    // Попытка вставить дубликат
    try
    {
        tree.add_node(50);  // Дубликат
        THROW_TEST_FAIL("Should handle duplicate insertion");
    }
    catch (const std::exception&)
    {
        // Ожидаемое поведение - либо игнорировать, либо бросать исключение
    }
}
    
    // Тест 7: Проверка свойств красно-черного дерева
/*    void test_properties()
    {
        RBTree<int> tree;
        
        // Заполняем дерево
        std::vector<int> values = {50, 30, 70, 20, 40, 60, 80, 10, 90};
        for (int val : values)
        {
            tree.add_node(val);
        }
        
        // Проверяем свойства
        ASSERT(check_rb_properties(tree), "Tree should satisfy all RB properties");
        
        // Специфические проверки:
        // 1. Корень черный
        ASSERT(tree.root_->colour_ == BLACK, "Root must be black");
        
        // 2. Нет двух красных узлов подряд
        ASSERT(check_no_double_red(tree.root_), "No double red nodes");
        
        // 3. Все листья (nullptr) считаются черными
        // 4. Все пути содержат одинаковое количество черных узлов
        int black_height = -1;
        ASSERT(check_black_height(tree.root_, 0, black_height), 
               "Black height should be consistent");
    }
    
    // Тест 8: Балансировка
    void test_balance()
    {
        // Тест 1: Случай когда дядя красный
        RBTree<int> tree1;
        tree1.add_node(50);  // Корень черный
        tree1.add_node(30);  // Красный
        tree1.add_node(70);  // Красный (дядя для следующих)
        tree1.add_node(20);  // Красный, вызывает перекрашивание
        
        // После вставки 20:
        // - 30 и 70 должны стать черными
        // - 50 должен стать красным, но потом стать черным (корень)
        ASSERT(tree1.root_->colour_ == BLACK, "Root should be black");
        if (tree1.root_->left_)
            ASSERT(tree1.root_->left_->colour_ == BLACK, "Parent should be black after recoloring");
        
        // Тест 2: Лево-правый случай (left-right)
        RBTree<int> tree2;
        tree2.add_node(50);
        tree2.add_node(30);
        tree2.add_node(40);  // Вызывает левый поворот, затем правый
        
        ASSERT(check_rb_properties(tree2), "Tree should be balanced after left-right case");
        
        // Тест 3: Право-левый случай (right-left)
        RBTree<int> tree3;
        tree3.add_node(50);
        tree3.add_node(70);
        tree3.add_node(60);  // Вызывает правый поворот, затем левый
        
        ASSERT(check_rb_properties(tree3), "Tree should be balanced after right-left case");
    }
    
private:
    // Вспомогательные функции проверки свойств
    
    bool check_rb_properties(RBTree<int>& tree)
    {
        if (!tree.root_) return true;
        
        // Корень черный
        if (tree.root_->colour_ != BLACK) return false;
        
        // Нет двух красных подряд
        if (!check_no_double_red(tree.root_)) return false;
        
        // Одинаковая черная высота
        int black_height = -1;
        return check_black_height(tree.root_, 0, black_height);
    }
    
    bool check_no_double_red(Node<int>* node)
    {
        if (!node) return true;
        
        if (node->colour_ == RED)
        {
            if (node->left_ && node->left_->colour_ == RED) return false;
            if (node->right_ && node->right_->colour_ == RED) return false;
        }
        
        return check_no_double_red(node->left_) && 
               check_no_double_red(node->right_);
    }
    
    bool check_black_height(Node<int>* node, int current, int& first_height)
    {
        if (!node)
        {
            current++;  // nullptr считается черным
            if (first_height == -1)
            {
                first_height = current;
                return true;
            }
            return current == first_height;
        }
        
        int new_height = current + (node->colour_ == BLACK ? 1 : 0);
        
        return check_black_height(node->left_, new_height, first_height) &&
               check_black_height(node->right_, new_height, first_height);
    }
};*/