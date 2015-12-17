#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <iostream>
#include <algorithm>

namespace trees
{

	template<typename  Key, typename  Value>
	class BTree
	{
	public:
		struct Node
		{
			typedef std::shared_ptr<Node> Ptr;
			typedef std::weak_ptr<Node> Weak;
			typedef std::vector<std::pair<Key, Value>> value_vector;
			value_vector vals;       // n >= size < 2*n
			std::vector<Ptr> childs;   // size(vals)+1
			bool is_leaf;

			typename Node::Weak parent;
			typename Node::Weak next;
			Node();
			Node(size_t cap);
			~Node();
			void insertValue(Key key,Value val);
			void insertChild(Key key, typename Node::Ptr C);
		};

	public:
		BTree() = delete;
		BTree(size_t N);
		~BTree();

		Value find(Key key)const;
		typename Node::Weak  find_node(Key key)const;
		bool insert(Key key, Value val); 
		void print()const;
	protected:
		typename Node::Ptr make_node();
		bool iner_find(Key key, typename Node::Ptr cur_node, typename Node::Ptr&out_ptr, Value &out_res)const; // return last_node, false if fail, or cur_node,true;
		bool isFull(const typename Node::Ptr node)const;
		void split_node(typename Node::Ptr node);
		void print(const typename Node::Ptr& root)const;
	private:
		size_t n;
		typename Node::Ptr m_root;
	};
}

#include "btree.hpp"
