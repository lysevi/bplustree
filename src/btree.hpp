
namespace trees{

	template<class Key, class Value>
	BTree<Key,Value>::Node::Node() {
		is_leaf = false;
	}

	template<class Key, class Value>
	BTree<Key,Value>::Node::Node(int cap) {
		is_leaf = false;
		this->childs.reserve(cap);
		this->vals.reserve(cap);
	}
	
	template<class Key, class Value>
	BTree<Key,Value>::Node::~Node() {
		this->childs.clear();
		this->vals.clear();
	}

	template<class Key, class Value>
	void BTree<Key,Value>::Node::insertValue(int key) {

		if (vals.size() == 0) {
			vals.push_back(key);
			return;
		} else {
			if (*vals.begin() > key) {
				vals.insert(vals.begin(), key);
				return;
			}
		}

		int step = 0;
		std::vector<int>::iterator prev_it;
		for (auto it = this->vals.begin(); it != this->vals.end(); ++it) {
			if (step != 0) {
				auto cur = *it;
				auto prev = *prev_it;
				if ((key < cur) && (key > prev)) {
					this->vals.insert(it, key);
					return;
				}
			}
			prev_it = it;
			step += 1;
		}

		this->vals.push_back(key);
	}

	template<class Key, class Value>
	void BTree<Key, Value>::Node::insertChild(int key, typename Node::Ptr C) {
		if (key == this->vals.back()) {
			this->childs.push_back(C);
			return;
		}
		for (size_t i = 0; i < this->vals.size(); ++i) {
			if (this->vals[i] == key) {
				auto pos = this->childs.begin() + i;
				if (pos == this->childs.end()) {
					this->childs.push_back(C);
				} else {
					this->childs.insert(pos + 1, C);
				}
				return;
			}
		}
		assert(false);
	}
	
	template<class Key, class Value>
	BTree<Key,Value>::BTree<Key,Value>(size_t N) :n(N) {
		m_root = BTree<Key,Value>::make_node();
		m_root->is_leaf = true;
	}
	
	template<class Key, class Value>
	BTree<Key,Value>::~BTree<Key,Value>() {
		m_root = nullptr;
	}

	template<class Key, class Value>
	typename BTree<Key, Value>::Node::Ptr BTree<Key, Value>::make_node() {
		return std::make_shared<typename BTree<Key, Value>::Node>(this->n);
	}

	template<class Key, class Value>
	int BTree<Key,Value>::find(int key)const {
		Node::Ptr node = m_root;
		int res;
		if (this->iner_find(key, m_root, node, res)) {
			assert(node != nullptr);
			return res;
		} else {
			assert(node != nullptr);
			return 0;
		}
	}

	template<class Key, class Value>
	typename BTree<Key, Value>::Node::Weak  BTree<Key, Value>::find_node(int key)const {
		Node::Ptr node = m_root;
		int res;
		this->iner_find(key, m_root, node, res);
		return node;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::iner_find(int key, typename Node::Ptr cur_node, typename Node::Ptr&out_ptr, int &out_res)const {
		if (cur_node->is_leaf) {
			out_res = key;
			out_ptr = cur_node;
			if (cur_node->vals.size() != 0)
				return true;
			else
				return false;
		}

		if (cur_node->vals.size() == 0) {
			out_res = 0;
			out_ptr = cur_node;
			return false;
		}

		if (key < cur_node->vals.front()) {
			if (cur_node->childs.size() == 0) {
				out_res = 0;
				out_ptr = cur_node;
				return false;
			} else {
				return iner_find(key, cur_node->childs.front(), out_ptr, out_res);
			}
		}
		for (size_t i = 0; i < cur_node->vals.size(); i++) {
			/*if (cur_node->vals[i] == key) {
				out_res = cur_node->vals[i];
				out_ptr = cur_node;
				return true;
				}*/
			if (cur_node->vals[i] > key) {
				if ((cur_node->childs.size() > 0) && (i != 0)) {
					return iner_find(key, cur_node->childs[i - 1], out_ptr, out_res);
				}
			}
		}
		if ((cur_node->childs.size() != 0) && (cur_node->vals.size() != 0)) {
			if (key <= cur_node->vals.front()) {
				return iner_find(key, cur_node->childs.front(), out_ptr, out_res);
			}
			if (key >= cur_node->vals.back()) {
				return iner_find(key, cur_node->childs.back(), out_ptr, out_res);
			}
		} else {
			out_res = 0;
			out_ptr = cur_node;
			return false;
		}
		return false;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::isFull(const typename BTree<Key, Value>::Node::Ptr node)const {
		return node->vals.size() >= (2 * n);
	}

	template<class Key, class Value>
	bool BTree<Key,Value>::insert(int key) {
		Node::Ptr node = nullptr;
		int res;
		this->iner_find(key, m_root, node, res);
		assert(node != nullptr);
		if (!isFull(node)) {
			node->insertValue(key);
			return true;
		} else { //split
			node->insertValue(key);
			this->split_node(node);
			return true;
		}
	}

	template<class Key, class Value>
	void BTree<Key, Value>::split_node(typename BTree<Key, Value>::Node::Ptr node) {
		auto C = this->make_node();
		C->vals = node->vals;

		auto midle = C->vals[(C->vals.size() / 2)];
		auto pos_half = C->vals.begin() + (C->vals.size() / 2);
		C->vals.erase(C->vals.begin(), pos_half);
		C->is_leaf = node->is_leaf;

		pos_half = node->vals.begin() + (node->vals.size() / 2);
		node->vals.erase(pos_half - 1, node->vals.end());
		auto tmp = node->next;
		node->next = C;
		C->next = tmp;
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

			node2insert->insertChild(midle, C);
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

		if (node->next.lock() == nullptr) {
			int a = 3;
		}
		if (isFull(node2insert)) {
			split_node(node2insert);
		}
	}

	template<class Key, class Value>
	void BTree<Key,Value>::print()const {
		print(m_root);
	}

	template<class Key, class Value>
	void BTree<Key, Value>::print(const typename Node::Ptr& root)const {
		std::cout << "{  [";
		for (auto v : root->vals) {
			std::cout << " " << v;
		}
		std::cout << "], ";
		for (auto c : root->childs) {
			print(c);
		}
		std::cout << "}" << std::endl;
	}
}