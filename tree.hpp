#pragma once

#include <iostream>
#include <queue>

enum Colour     //сделать неймспейс для этого, поднеймспейс неймспейса дерева
{
    BLACK,
    RED
};

template <typename KeyT = int>
struct Node
{
    KeyT key_;
    Node<KeyT>* parent_ = nullptr;
    Node<KeyT>* left_ = nullptr;
    Node<KeyT>* right_ = nullptr;
    Colour colour_ = RED;

    Node() : key_{} {};
    explicit Node(const KeyT& key) : key_(key) {};                              //!!не Node<KeyT>
    Node(const Node& other) : key_(other.key_), colour_(other.colour_) {};

    Node& operator=(const Node& other)
    {
        if (this != &other)         //!!
        {
            key_ = other.key_;
            colour_ = other.colour_;
        }
        return *this;
    }

    ~Node()
    {
        left_ = nullptr;
        right_ = nullptr;
    }

    void print_node()
    {
        printf("%d %d", key_, colour_); 
    }
};

template <typename KeyT = int>
class RBTree
{
public:
    Node<KeyT>* root_ = nullptr;

    RBTree() {};

    //параметрический конструктор от узла не имеет смысла
    RBTree(const RBTree& other)
    {
        if (other.root_)
            root_ = copy_tree(other.root_, nullptr);
        else
            root_ = nullptr;
    }

    RBTree& operator=(const RBTree& other)
    {
        if (this != &other)
        {
            RBTree tmp(other);
            std::swap(root_, tmp.root_);        //!copy_cnd_swap идиома
        }
        return *this;
    }

    ~RBTree()
    {
        safe_delete(root_);
    }
private:

    void safe_delete(Node<KeyT>* root)      //здесь надо <KeyT>
    {
        if (!root)
            return;

        std::queue<Node<KeyT>*> nodes;
        nodes.push(root);

        while (!nodes.empty())
        {
            Node<KeyT>* cur = nodes.front();
            nodes.pop();

            Node<KeyT>* left = cur->left_;
            Node<KeyT>* right = cur->right_;

            delete cur;

            if (left)
            {
                left->parent_ = nullptr;
                nodes.push(left);
            }
            if (right)
            {
                right->parent_ = nullptr;
                nodes.push(right);
            }
        }
    }

    Node<KeyT>* copy_tree(const Node<KeyT>* source, Node<KeyT>* parent)
    {
        if(!source)
            return nullptr;

        Node<KeyT>* new_node = new Node<KeyT>(*source);
        new_node->parent_ = parent;     //тот, что мы передаем

        new_node->left_ = copy_tree(source->left_, new_node);
        new_node->right_ = copy_tree(source->right_, new_node);

        return new_node;
    }

    void left_rotate(Node<KeyT>* root)
    {
        Node<KeyT>* new_root = root->right_;
        root->right_ = new_root->left_;

        if (new_root->left_)
            new_root->left_->parent_ = root;

        new_root->parent_ = root->parent_;

        if (!root->parent_)
            root_ = new_root;
        else if (is_node_left(root))
            root->parent_->left_ = new_root;
        else
            root->parent_->right_ = new_root;
            
        new_root->left_ = root;
        root->parent_ = new_root;
    }

    void right_rotate(Node<KeyT>* root)
    {
        Node<KeyT>* new_root = root->left_;
        root->left_ = new_root->right_;

        if (new_root->right_)
            new_root->right_->parent_ = root;

        new_root->parent_ = root->parent_;

        if (!root->parent_)
            root_ = new_root;
        else if (is_node_left(root))
            root->parent_->left_ = new_root;
        else
            root->parent_->right_ = new_root;
            
        new_root->right_ = root;
        root->parent_ = new_root;
    }

    void balance(Node<KeyT>* node)
    {
        while (node != root_ && node->parent_->colour_ == RED)
        {
            if (is_parent_left(node))
            {
                Node<KeyT>* uncle = node->parent_->parent_->right_;

                if (uncle && uncle->colour_ == RED)
                {
                    node->parent_->colour_ = BLACK;
                    uncle->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    node = node->parent_->parent_;
                }
                else
                {
                    if (node == node->parent_->right_)
                    {
                        node = node->parent_;
                        left_rotate(node);
                    }

                    node->parent_->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    right_rotate(node->parent_->parent_);
                }
            }
            else
            {
                Node<KeyT>* uncle = node->parent_->parent_->left_;

                if (uncle && uncle->colour_ == RED)
                {
                    node->parent_->colour_ = BLACK;
                    uncle->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    node = node->parent_->parent_;
                }
                else
                {
                    if (node == node->parent_->left_)
                    {
                        node = node->parent_;
                        right_rotate(node);
                    }

                    node->parent_->colour_ = BLACK;
                    node->parent_->parent_->colour_ = RED;
                    left_rotate(node->parent_->parent_);
                }
            }
        }

        root_->colour_ = BLACK;
    }

public:
    void add_node(KeyT key)
    {
        //!Node<KeyT> new_node{key};     ! неполучится, создается объект на стеке
        Node<KeyT>* new_node = new Node<KeyT>(key);

        if (!root_)
        {
            //printf("!root\n");
            root_ = new_node;
            root_->colour_ = BLACK;
            return;
        }

        Node<KeyT>* parent = find_parent(new_node->key_);
        if (!parent)
        {
            delete new_node;
            return;
        }

        new_node->parent_ = parent;

        if (parent->key_ > key)
            parent->left_ = new_node;
        else if (parent->key_ < key)
            parent->right_ = new_node;

        
        balance(new_node);
    }

    //!!Node<KeyT>* find_parent(Node<KeyT> node)      //ссылка же провиснет, если попытаться ее вернуть
    Node<KeyT>* find_parent(KeyT key)
    {
        //printf("find parent\n");
        Node<KeyT>* current = root_;
        Node<KeyT>* parent = nullptr;
    
        while (current)
        {
            parent = current;

            if (key == current->key_)
                throw std::invalid_argument("double key");
            else if (key < current->key_)
                current = current->left_;
            else  // key > current->key_
                current = current->right_;
        }
        return parent;
    }

    void print_tree(Node<KeyT>* node)
    {
        if (!node)
            return;
        
        printf("(");
        node->print_node();
        if (node->left_)
            print_tree(node->left_);
        if (node->right_)
            print_tree(node->right_);
        printf(")");
    }

private:

    bool is_parent_right(Node<KeyT>* node) const
    {
        return node->parent_ == node->parent_->parent_->right_;
    }

    bool is_parent_left(Node<KeyT>* node) const 
    {
        return node->parent_ == node->parent_->parent_->left_;
    }

    bool is_node_right(Node<KeyT>* node) const 
    {
        return node == node->parent_->right_;
    }

    bool is_node_left(Node<KeyT>* node) const 
    {
        return node == node->parent_->left_;
    }

};