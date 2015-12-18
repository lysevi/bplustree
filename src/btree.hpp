
namespace trees{

	template<class Key, class Value>
	BTree<Key,Value>::Node::Node() {
		is_leaf = false;
		vals_size = 0;
	}

	template<class Key, class Value>
	BTree<Key, Value>::Node::Node(size_t cap) {
		is_leaf = false;
        this->childs.reserve(cap+1);
        this->vals.reserve(cap+1);
		vals_size = 0;
	}
	
	template<class Key, class Value>
	BTree<Key,Value>::Node::~Node() {
		this->childs.clear();
		this->vals.clear();
	}

	template<class Key, class Value>
	void BTree<Key, Value>::Node::insertValue(Key key, Value val) {
        auto kv = node_data(key,val);
		if (vals.size() == 0) {
			vals.resize(1);
			vals_size++;
			vals[0]=kv;
			return;
		} else {
			if (vals.data()->first > key) {
				vals.resize(vals.size() + 1);
				vals_size++;
				insert_to_array(vals.data(), vals_size, 0, kv);
				return;
			}
		}

		auto lb_iter = std::lower_bound(this->vals.data(), this->vals.data() + vals_size, kv,
										[](const std::pair<Key, Value> &l, const std::pair<Key, Value> &r){return l.first < r.first; });
		if (lb_iter != this->vals.data()+vals_size) {
			auto d = std::distance(vals.data(), lb_iter);
			vals.resize(vals.size() + 1);
			vals_size++;
			insert_to_array(vals.data(), vals.size(), d, kv);
			return;
		}

		vals.resize(vals.size() + 1);
		this->vals[vals_size] = (kv);
		vals_size++;
	}

	template<class Key, class Value>
	void BTree<Key, Value>::Node::insertChild(Key key, typename Node::Ptr C) {
		if (key == this->vals.back().first) {
			this->childs.push_back(C);
			return;
		}
		auto kv = std::make_pair(key, Value{});
		auto lb_iter = std::lower_bound(this->vals.data(), vals.data() + vals_size, kv,
										[](const std::pair<Key, Value> &l, const std::pair<Key, Value> &r){return l.first < r.first; });
		if (lb_iter != this->vals.data() + vals_size) {
			auto pos = this->childs.begin() + std::distance(this->vals.data(), lb_iter);
			this->childs.insert(pos + 1, C);
			return;
		} else {
			this->childs.push_back(C);
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
		Value res;
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
		Key res;
		this->iner_find(key, m_root, node, res);
		return node;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::iner_find(Key key, typename Node::Ptr cur_node, typename Node::Ptr&out_ptr, Value &out_res)const {
		if (cur_node->is_leaf) {
			auto find_res = std::lower_bound(cur_node->vals.data(), cur_node->vals.data() + cur_node->vals_size, std::make_pair(key, Value()),
										   [key](const std::pair<Key, Value> &v, const std::pair<Key, Value> &v2) {
				return (v.first < v2.first);
									});

			out_ptr = cur_node;
			if (find_res != cur_node->vals.data() + cur_node->vals_size) {
				out_res = find_res->second;
				return true;
			}
			else{
				return false;
			}
		}

        if (key < cur_node->vals[0].first) {
            return iner_find(key, cur_node->childs[0], out_ptr, out_res);
        }

		if ((cur_node->childs.size() != 0) && (cur_node->vals.size() != 0)) {
			if (key >= cur_node->vals[cur_node->vals.size() - 1].first) {
				auto last = cur_node->childs[cur_node->childs.size() - 1];
				return iner_find(key, last, out_ptr, out_res);
			}
		}

		
   /*     for (size_t i = 0; i < cur_node->vals.size()-1; i++) {
            auto cur=cur_node->vals[i].first;
            auto nxt=cur_node->vals[i+1].first;*/

            //if((cur<=key) && ((key<nxt)))
			{
				auto kv = std::make_pair(key, Value());
				auto low_bound = std::lower_bound(cur_node->vals.data(), cur_node->vals.data() + cur_node->vals_size, kv,
												  [key](const std::pair<Key, Value> &v, const std::pair<Key, Value> &v2) {
					return (v.first < v2.first);
				});
				
				
				//if ((low_bound != cur_node->vals.end())) 
				{
					auto nxt_it = low_bound + 1;
					if (low_bound->first != kv.first) {
						low_bound--;
						nxt_it--;
					}
					if (key < nxt_it->first) {
						auto d = std::distance(cur_node->vals.data(), low_bound);
						return iner_find(key, cur_node->childs[d+1], out_ptr, out_res);
					}
					
				}
                  /*return iner_find(key, cur_node->childs[i+1], out_ptr, out_res);*/
            }
		/*}*/

		

		return false;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::isFull(const typename BTree<Key, Value>::Node::Ptr node)const {
        return (node->vals.size()+1) >= (2 * n);
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::insert(Key key, Value val) {
        typename Node::Ptr node = nullptr;
		Value res;
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
		C->is_leaf = node->is_leaf;

		auto pos_half_index = (node->vals.size() / 2);
		auto midle = node->vals[pos_half_index];
		auto pos_half = node->vals.begin() + pos_half_index;

		if (C->is_leaf) {
			C->vals = typename Node::value_vector(pos_half, node->vals.begin() + node->vals_size);
		} else {
			C->vals = typename Node::value_vector(pos_half + 1, node->vals.begin() + node->vals_size);
		}
				
		node->vals.erase(pos_half, node->vals.begin() + node->vals_size);
		auto tmp = node->next;
		node->next = C;
		C->next = tmp;
		C->vals_size = C->vals.size();
		node->vals_size = node->vals.size();
		if (node->childs.size() > 0) {
			size_t new_count = 0;
			size_t old_count = 0;
			for (auto c : node->childs) {
				if (c->vals[0].first >= midle.first) {
					new_count++;
				} else {
					old_count++;
				}
			}
			std::vector<typename Node::Ptr> new_childs{ new_count };
			std::vector<typename Node::Ptr> old_childs{ old_count };
			new_count = old_count = 0;
			for (auto c : node->childs) {
                if (c->vals[0].first >= midle.first) {
					new_childs[new_count++]=c;
				} else {
					old_childs[old_count++] = c;
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
