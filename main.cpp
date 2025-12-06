#include "tester_node.hpp"
#include "tester_tree.hpp"

int main()
{
    NodeTester node_tester;
    node_tester.run_all_tests();

    RBTreeTester tree_tester;
    tree_tester.run_all_tests();

    return 0;
}