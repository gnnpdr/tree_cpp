#pragma once

#include <iostream>

enum Colour     //сделать неймспейс для этого, поднеймспейс неймспейса дерева
{
    BLACK,
    RED
};

template <typename KeyT = int>
struct Node
{
    KeyT key_;
    Node* parent_ = nullptr, left_ = nullptr, right_ = nullptr;
    Colour colour_ = RED;

    Node(KeyT key) : key_(key) {}; 

    ~Node()
    {
        delete(left_);
        delete(right_);
    }
};

template <typename KeyT = int>
class RBTree
{
    Node* root_ = nullptr;
public:
    using KeyT = KeyT;

    RBTree(Node<KeyT>* root) : root_(root) {};
    ~RBTree()
    {
        delete(root_);
    }

    //надо сделать другие элементы биг 5, тк у нас тут указатели

private:

    void left_rotate(Node<KeyT>* root)       //здесь скорее local root
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

public:
    void balance(Node<KeyT>* node)   //укоротить
    {
        Node<KeyT>* uncle;
        if (is_parent_right(node))
            uncle = node->parent_->parent_->left_;
        else if (is_parent_left(node))
            uncle = node->parent_->parent_->right_;

        if (uncle->colour_ == RED)
        {
            recolour(root_);
            return;
        }

        if (is_parent_left(node) && is_node_right(node))
        {
            left_rotate(node->parent_);
            right_rotate(node);
            node->colour_ = BLACK;
            node->left_.colour_ = RED;
            return;
        }
        else if (is_parent_right(node) && is_node_left(node))
        {
            right_rotate(node->parent);
            left_rotate(node);
            node->colour_ = BLACK;
            node->right_.colour_ = RED;
            return;
        }

        if (is_node_left(node))
        {
            right_rotate(node->parent_);
            node->parent_->colour_ = BLACK;
            node->parent_->right_.colour_ = RED;
            return;
        }
        else
        {
            left_rotate(node->parent_);
            node->parent_->colour_ = BLACK;
            node->parent_->left_.colour_ = RED;
            return;
        }
    }

    void add_node(KeyT key)
    {
        Node<KeyT> new_node{key};

        Node<KeyT>* parent = find_parent(new_node);
        if (parent->key_ > key)
            parent->left_ = new_node;
        else if (parennt->key_ < key)
            parent->right_ = new_node;

        new_node.parent_ = parent;
        balance(parent->parent_);        //так ведь? по идее набольшая проблема может быть с дедушкой
            
        //if(!root)      //если до этого не было корня, если это самая первая нода
        //{     //это можно в функцию баланс включить
        //    new_node.colour_ = BLACK;
        //}
    }

    Node<KeyT>* find_parent(Node<KeyT> node)      //ссылка же провиснет, если попытаться ее вернуть
    {
        Node<KeyT>* possible_parent = root_;

        while (possible_parent.right_ || possible_parent.left_)     //если есть место хоть куда нибудь сходить
        {
            //это точно можно сделать красивее, надо подумать
            if (possible_parent.left_ && possible_parent.key_ > node.key_)
                possible_parent = possible_parent.left_;
            else if (!possible_parent.left_ && possible_parent.key_ > node.key_)
                return posiible_parent;
            else if (possible_parent.right_ && possible_parent.key_ < node.key_)
                possible_parent = possible_parent.right_;
            else if (!possible_parent.right_ && possible_parent.key_ < node.key_)
                return possible_parent;
            else
            {
                //может, исключение кинуть, что попалась нода с тем же значением, что пытаются вставить
                //хотя, нет, наверное, легче вернуть нуллптр, но его так долго тянуть, нет?
            }
        }
        
        return nullptr;
    }

    void print_tree()
    {
        //ну, чтобы было ясно что происходит, функция для отладки, нужно написать какой-нибудь из обходов
    }

private:

    bool is_parent_right(Node* node) const
    {
        return node->parent_ == node->parent_->parent_->right_
    }

    bool is_parent_left(Node* node) const 
    {
        return node->parent_ == node->parent_->parent_->left_;
    }

    bool is_node_right(Node* node) const 
    {
        return node == node->parent_->right_;
    }

    bool is_node_left(Node* node) const 
    {
        return node == node->parent_->left_;
    }

};