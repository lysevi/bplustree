
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
	void BTree<Key, Value>::Node::insertValue(Key key, Value val) {
		auto kv = std::make_pair(key,val);
		if (vals.size() == 0) {
			vals.push_back(kv);
			return;
		} else {
			if (vals.begin()->first > key) {
				vals.insert(vals.begin(), kv);
				return;
			}
		}

		int step = 0;
        typename std::vector<std::pair<Key, Value>>::iterator prev_it;
		for (auto it = this->vals.begin(); it != this->vals.end(); ++it) {
			if (step != 0) {
				auto cur = *it;
				auto prev = *prev_it;
				if ((key < cur.first) && (key > prev.first)) {
					this->vals.insert(it, kv);
					return;
				}
			}
			prev_it = it;
			step += 1;
		}

		this->vals.push_back(kv);
	}

	template<class Key, class Value>
	void BTree<Key, Value>::Node::insertChild(Key key, typename Node::Ptr C) {
		if (key == this->vals.back().first) {
			this->childs.push_back(C);
			return;
		}
		for (size_t i = 0; i < this->vals.size(); ++i) {
			if (this->vals[i].first == key) {
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
    BTree<Key,Value>::BTree(size_t N) :n(N) {
		m_root = BTree<Key,Value>::make_node();
		m_root->is_leaf = true;
	}
	
	template<class Key, class Value>
    BTree<Key,Value>::~BTree() {
		m_root = nullptr;
	}

	template<class Key, class Value>
	typename BTree<Key, Value>::Node::Ptr BTree<Key, Value>::make_node() {
		return std::make_shared<typename BTree<Key, Value>::Node>(this->n);
	}

	template<class Key, class Value>
	Value BTree<Key, Value>::find(Key key)const {
        typename Node::Ptr node = m_root;
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
	typename BTree<Key, Value>::Node::Weak  BTree<Key, Value>::find_node(Key key)const {
        typename Node::Ptr node = m_root;
		int res;
		this->iner_find(key, m_root, node, res);
		return node;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::iner_find(Key key, typename Node::Ptr cur_node, typename Node::Ptr&out_ptr, Value &out_res)const {
		if (cur_node->is_leaf) {
			auto find_res=std::find_if(cur_node->vals.begin(), cur_node->vals.end(), 
									   [key](const std::pair<Key, Value> &v) {
				return (v.first == key);
									});

			out_ptr = cur_node;
			if (find_res != cur_node->vals.end()) {
				out_res = find_res->second;
				return true;
			}
			else{
				return false;
			}
		}

        if (key < cur_node->vals.front().first) {
            return iner_find(key, cur_node->childs.front(), out_ptr, out_res);
        }

        for (size_t i = 0; i < cur_node->vals.size()-1; i++) {
            auto cur=cur_node->vals[i].first;
//            if(cur==key){
//                return iner_find(key, cur_node->childs[i], out_ptr, out_res);
//            }
            auto nxt=cur_node->vals[i+1].first;

            if((cur<=key) && ((key<nxt))){
                    return iner_find(key, cur_node->childs[i+1], out_ptr, out_res);
            }
		}

		if ((cur_node->childs.size() != 0) && (cur_node->vals.size() != 0)) {
			if (key >= cur_node->vals.back().first) {
				return iner_find(key, cur_node->childs.back(), out_ptr, out_res);
			}
        }

		return false;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::isFull(const typename BTree<Key, Value>::Node::Ptr node)const {
		return node->vals.size() >= (2 * n);
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::insert(Key key, Value val) {
        typename Node::Ptr node = nullptr;
		int res;
		this->iner_find(key, m_root, node, res);
		assert(node != nullptr);
		if (!isFull(node)) {
			node->insertValue(key, val);
			return true;
		} else { //split
			node->insertValue(key, val);
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
        node->vals.erase(pos_half , node->vals.end());
		auto tmp = node->next;
		node->next = C;
		C->next = tmp;
		if (!C->is_leaf) {
			C->vals.erase(C->vals.begin(), C->vals.begin() + 1);
		}
		if (node->childs.size() > 0) {
            std::vector<typename Node::Ptr> new_childs;
            std::vector<typename Node::Ptr> old_childs;
			for (auto c : node->childs) {
                if (c->vals.front().first >= midle.first) {
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
        typename Node::Ptr node2insert = nullptr;

		if (auto parent = node->parent.lock()) {
			node2insert = parent;
			node2insert->insertValue(midle.first,midle.second);

			node2insert->insertChild(midle.first, C);
			C->parent = node->parent;
		} else {
			node2insert = this->make_node();
			m_root->parent = node2insert;
			m_root = node2insert;
          
			node2insert->insertValue(midle.first, midle.second);
			node2insert->childs.push_back(node);
			node2insert->childs.push_back(C);
			node->parent = m_root;
			C->parent = m_root;
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
