#include "btree.h"
#include <cassert>
#include <iostream>

using namespace trees;

BTree::Node::Node() {
}

BTree::Node::~Node() {
    this->childs.clear();
    this->vals.clear();
}

void BTree::Node::insertValue(int key) {

    if (vals.size() == 0) {
        vals.push_back(key);
        return;
    } else {
        if (*vals.begin() > key) {
            vals.insert(vals.begin(), key);
            return;
        }
    }

    for (auto it = this->vals.begin(); it != this->vals.end(); ++it) {
        if (it != this->vals.begin()) {
            auto cur=*it;
            auto prev=*(it-1);
            if ((key < cur) && (key > prev)) {
                this->vals.insert(it, key);
                return;
            }
        }
    }

    this->vals.push_back(key);
}

void BTree::Node::insertChild(int key, Node::Ptr C){
    if(key==this->vals.back()){
        this->childs.push_back(C);
        return;
    }
    for(size_t i=0;i<this->vals.size();++i){
        if(this->vals[i]==key){
            auto pos=this->childs.begin()+i;
            if(pos==this->childs.end()){
                this->childs.push_back(C);
            }else{
                this->childs.insert(pos+1,C);
            }
            return;
        }
    }
    assert(false);
}
BTree::BTree(size_t N) :n(N) {
    m_root = BTree::make_node();
}

BTree::~BTree() {
    m_root = nullptr;
}

BTree::Node::Ptr BTree::make_node() {
    return std::make_shared<BTree::Node>();
}

int BTree::find(int key)const {
    Node::Ptr node = m_root;
    int res;
    if (this->iner_find(key,m_root, node, res)) {
        assert(node != nullptr);
        return res;
    } else {
        assert(node != nullptr);
        return 0;
    }
}

bool BTree::iner_find(int key, Node::Ptr cur_node, Node::Ptr&out_ptr, int &out_res)const {
    if(cur_node->vals.size()==0){
        out_res = 0;
        out_ptr=cur_node;
        return false;
    }

    if(key< cur_node->vals.front()){
        if(cur_node->childs.size()==0){
            out_res = 0;
            out_ptr=cur_node;
            return false;
        }else{
            return iner_find(key, cur_node->childs.front(), out_ptr, out_res);
        }
    }
    for (size_t i = 0; i<cur_node->vals.size();i++){
        if (cur_node->vals[i] == key) {
            out_res = cur_node->vals[i];
            out_ptr = cur_node;
            return true;
        }
        if (cur_node->vals[i] > key) {
            if ((cur_node->childs.size() > 0) && (i!=0)) {
                return iner_find(key, cur_node->childs[i], out_ptr, out_res);
            }
        }
    }
    if ((cur_node->childs.size() != 0) && (cur_node->vals.size() != 0)) {
        if (key< cur_node->vals.front()) {
            return iner_find(key, cur_node->childs.front(), out_ptr, out_res);
        }
        if (key> cur_node->vals.back()) {
            return iner_find(key, cur_node->childs.back(), out_ptr, out_res);
        }
    } else {
        out_res = 0;
        out_ptr = cur_node;
        return false;
    }
    return false;
}

bool BTree::isFull(const BTree::Node::Ptr node)const {
    return node->vals.size() >= (2 * n);
}

bool BTree::insert(int key) {
    Node::Ptr node = nullptr;
    int res;
    if (this->iner_find(key,m_root, node, res)) {
        return false;
    } else {
        assert(node != nullptr);
        if (!isFull(node)) {
            node->insertValue(key);
            return true;
        } else { //split
            node->insertValue(key);
            this->split_node(node);
            return true;
        }
        return false;
    }
}

void BTree::split_node(BTree::Node::Ptr node) {
    auto C = this->make_node();
    C->vals = node->vals;

    auto midle = C->vals[(C->vals.size() / 2)];
    auto pos_half = C->vals.begin() + (C->vals.size() / 2 + 1);
    C->vals.erase(C->vals.begin(), pos_half);

    pos_half = node->vals.begin() + (node->vals.size() / 2);
    node->vals.erase(pos_half, node->vals.end());

    if (node->childs.size() > 0) {
        std::vector<Node::Ptr> new_childs;
        std::vector<Node::Ptr> old_childs;
        for (auto c : node->childs) {
            if (c->vals.front() > midle) {
                new_childs.push_back(c);
            } else {
                old_childs.push_back(c);
            }
        }
        node->childs = old_childs;
        for (auto c : node->childs) {
            c->parent = node;
        }
        C->childs = new_childs;
        for (auto c : C->childs) {
            c->parent = C;
        }

    }
    Node::Ptr node2insert = nullptr;

    if (auto parent = node->parent.lock()) {
        node2insert = parent;
        node2insert->insertValue(midle);

        node2insert->insertChild(midle,C);
        C->parent = node->parent;
    } else {
        node2insert = this->make_node();
        m_root->parent = node2insert;
        m_root = node2insert;
        node2insert->insertValue(midle);
        node2insert->childs.push_back(node);
        node2insert->childs.push_back(C);
        node->parent = m_root;
        C->parent = m_root;
    }


    if (isFull(node2insert)) {
        split_node(node2insert);
    }
}

void BTree::print()const {
    print(m_root);
}

void BTree::print(const Node::Ptr& root)const {
    std::cout << "{  [";
    for (auto v : root->vals) {
        std::cout << " " << v;
    }
    std::cout << "], ";
    for (auto c : root->childs) {
        print(c);
    }
    std::cout << "}"<<std::endl;
}
