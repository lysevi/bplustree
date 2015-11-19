#include "btree.h"
#include <cassert>
using namespace trees;


BTree::BTree(size_t N) :n(N) {
	m_root = BTree::make_root();
};

BTree::Node::Ptr BTree::make_root() {
	return std::make_shared<BTree::Node>();
}

int BTree::find(int key)const {
	Node::Ptr node=nullptr;
	int res;
	if (this->iner_find(key, node, res)) {
		assert(node != nullptr);
		return res;
	} else {
		assert(node != nullptr);
		return 0;
	}
}

bool BTree::iner_find(int key, Node::Ptr&out_ptr, int &out_res)const {

	Node::Ptr cur_node = m_root;
	Node::Ptr next_node = nullptr;
	Node::Ptr last_node = cur_node;
	while (cur_node != nullptr) {
		if (cur_node->vals.size() != 0) {
			if (cur_node != nullptr) {
				if (key > cur_node->vals.back()) {
					next_node = cur_node->childs.back();
				}
			} else {
				if (cur_node != nullptr) {
					if (key > cur_node->vals.front()) {
						next_node = cur_node->childs.front();
					}
				}
			}
		} else {
			for (size_t i = 0; i < cur_node->vals.size(); ++i) {
				if (cur_node->vals[i] == key) {
					out_ptr = cur_node;
					out_res = cur_node->vals[i];
					return true;
				}

				if ((i != cur_node->childs.size()) && (cur_node->vals[i - 1] < key) && (cur_node->vals[i] > key)) {
					if (cur_node->childs.size() < i) {
						next_node = nullptr;
					} else {
						next_node = cur_node->childs[i];
					}
					break;
				}
			}
			last_node = cur_node;
			cur_node = next_node;
			next_node = nullptr;
		}
	}
	out_ptr = last_node;
	out_res = 0;
	return false;
	
}

bool BTree::insert(int key) {
	return false;
}