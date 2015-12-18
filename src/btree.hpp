
namespace trees{

	template<class Key, class Value>
    BTree<Key,Value>::Node::Node() {
		is_leaf = false;
		vals_size = 0;
        childs_size=0;
        id=0;
        parent=0;
        next=0;
	}

	template<class Key, class Value>
    BTree<Key, Value>::Node::Node(size_t cap){
		is_leaf = false;
        this->childs.resize(cap+1);
        this->vals.resize(cap+1);
        vals_size = 0;
        childs_size=0;
        id=0;
        parent=0;
        next=0;
	}
	
	template<class Key, class Value>
	BTree<Key,Value>::Node::~Node() {
		this->childs.clear();
		this->vals.clear();
	}

	template<class Key, class Value>
	void BTree<Key, Value>::Node::insertValue(Key key, Value val) {
        auto kv = node_data(key,val);
		if (vals_size == 0) {
			vals_size++;
			vals[0]=kv;
			return;
		} else {
			if (vals.data()->first > key) {
				vals_size++;
				insert_to_array(vals.data(), vals_size, 0, kv);
				return;
			}
		}

		auto lb_iter = std::lower_bound(this->vals.data(), this->vals.data() + vals_size, kv,
										[](const std::pair<Key, Value> &l, const std::pair<Key, Value> &r){return l.first < r.first; });
		if (lb_iter != this->vals.data()+vals_size) {
			auto d = std::distance(vals.data(), lb_iter);
			vals_size++;
			insert_to_array(vals.data(), vals_size, d, kv);
			return;
		}

		this->vals[vals_size] = (kv);
		vals_size++;
	}

	template<class Key, class Value>
	void BTree<Key, Value>::Node::insertChild(Key key, typename Node::Ptr C) {
		if (key == this->vals.back().first) {
			//this->childs.resize(this->childs.size() + 1);
            this->childs[childs_size]=C->id;
			childs_size++;
			return;
		}
		auto kv = std::make_pair(key, Value{});
		auto lb_iter = std::lower_bound(this->vals.data(), vals.data() + vals_size, kv,
										[](const std::pair<Key, Value> &l, const std::pair<Key, Value> &r){return l.first < r.first; });
		if (lb_iter != this->vals.data() + vals_size) {
			auto d = std::distance(this->vals.data(), lb_iter);
            insert_to_array(this->childs.data(), childs_size+1, d+1, C->id);
			childs_size++;
			return;
		} else {
            this->childs[childs_size] = C->id;
			childs_size++;
		}
		assert(false);
	}
	
	template<class Key, class Value>
    BTree<Key,Value>::BTree(size_t N):n(N) {
        cache=new std::vector<Node>(100000);
        for(size_t i=0;i<cache->size();i++){
            (*cache)[i]=Node(this->n*2);
        }
        cache_pos=1;
        m_root = BTree<Key,Value>::make_node().first;
		m_root->is_leaf = true;

	}
	
	template<class Key, class Value>
    BTree<Key,Value>::~BTree() {
		m_root = nullptr;
        this->cache->clear();
        delete this->cache;
	}

	template<class Key, class Value>
    std::pair<typename BTree<Key, Value>::Node::Ptr,typename BTree<Key, Value>::Node::Weak> BTree<Key, Value>::make_node() {
        auto ptr=&(*cache)[cache_pos];
        //(*cache)[cache_pos]=ptr;
        ptr->id=cache_pos;
        auto pos=cache_pos;
        cache_pos++;
        return std::make_pair(ptr,pos);
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
    typename BTree<Key, Value>::Node::Ptr  BTree<Key, Value>::find_node(Key key)const {
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

         //case k < k_0
        if (key < cur_node->vals[0].first) {
            return iner_find(key, &(*cache)[cur_node->childs[0]], out_ptr, out_res);
        }

         //case k_d ≤ k
        if ((cur_node->childs_size != 0) && (cur_node->vals_size != 0)) {
            if (key >= cur_node->vals[cur_node->vals_size - 1].first) {
                auto last = cur_node->childs[cur_node->childs_size - 1];
                return iner_find(key, &(*cache)[last], out_ptr, out_res);
            }
        }

        // case k_i ≤ k < k_{i+1}
        auto kv = node_data(key, Value());
        auto low_bound = std::lower_bound(cur_node->vals.data(),
                                          cur_node->vals.data() + cur_node->vals_size,
                                          kv,
                                          [key](const node_data& v, const node_data&v2)
                                            {
                                                return (v.first < v2.first);
                                            });

        auto nxt_it = low_bound + 1;
        if (low_bound->first != kv.first) {
            low_bound--;
            nxt_it--;
        }
        if (key < nxt_it->first) {
            auto d = std::distance(cur_node->vals.data(), low_bound);
            return iner_find(key, &(*cache)[cur_node->childs[d+1]], out_ptr, out_res);
        }

		return false;
	}

	template<class Key, class Value>
	bool BTree<Key, Value>::isFull(const typename BTree<Key, Value>::Node::Ptr node)const {
        return (node->vals_size+1) >= (n*2);
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
        auto C_rec=this->make_node();
        auto C = C_rec.first;
		C->is_leaf = node->is_leaf;

		auto pos_half_index = (node->vals_size / 2);
		auto midle = node->vals[pos_half_index];
		auto vals_begin = pos_half_index;

		if (!C->is_leaf) {
			vals_begin++;
		} 
		
		size_t insert_pos = 0;
		for (size_t i = vals_begin; i < node->vals_size; i++) {
			C->vals[insert_pos] = node->vals[i];
			C->vals_size++;
			insert_pos++;
		}

		node->vals_size = pos_half_index;
		auto tmp = node->next;
        node->next = C_rec.second;
		C->next = tmp;
		
		if (node->childs_size > 0) {
            size_t new_count = node->childs_size/2;
            size_t old_count = node->childs_size/2;

            C->childs_size = new_count;
            size_t pos=0;
            for (size_t i = old_count; i<node->childs_size; i++) {
                auto ch_ind = node->childs[i];
                auto ch = &(*cache)[ch_ind];
                ch->parent = C_rec.second;
                C->childs[pos++]=ch_ind;
			}
            for(size_t i=old_count;i<node->childs_size;i++){
                node->childs[i]=0;
            }

            node->childs_size = old_count;
		}
        typename Node::Ptr node2insert = nullptr;

        auto parent_ind= node->parent;
        if (parent_ind!=0) {
            node2insert = &(*cache)[parent_ind];
			node2insert->insertValue(midle.first,midle.second);

			node2insert->insertChild(midle.first, C);
			C->parent = node->parent;
		} else {
            auto node2insert_rec = this->make_node();
            node2insert = node2insert_rec.first;
            m_root->parent = node2insert_rec.second;
			m_root = node2insert;
          
			node2insert->insertValue(midle.first, midle.second);
            node2insert->childs[node2insert->childs_size]=(node->id);
			node2insert->childs_size++;
            node2insert->childs[node2insert->childs_size] = (C->id);
			node2insert->childs_size++;
            node->parent = m_root->id;
            C->parent = m_root->id;
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
