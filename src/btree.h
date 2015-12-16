#pragma once

#include <vector>
#include <memory>
#include <cassert>
#include <iostream>

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
			std::vector<int> vals;       // n >= size < 2*n
			std::vector<Ptr> childs;   // size(vals)+1
			bool is_leaf;

			typename Node::Weak parent;
			typename Node::Weak next;
			Node();
			Node(int cap);
			~Node();
			void insertValue(int key);
            void insertChild(int key, typename Node::Ptr C);
		};

	public:
		BTree() = delete;
		BTree(size_t N);
		~BTree();

		int find(int key)const;
		typename Node::Weak  find_node(int key)const;
		bool insert(int key); //true if key was inserted. false, if not (maybe she alrady exists)
		void print()const;
	protected:
		typename Node::Ptr make_node();
		bool iner_find(int key, typename Node::Ptr cur_node, typename Node::Ptr&out_ptr, int &out_res)const; // return last_node, false if fail, or cur_node,true;
		bool isFull(const typename Node::Ptr node)const;
		void split_node(typename Node::Ptr node);
		void print(const typename Node::Ptr& root)const;
	private:
		size_t n;
		typename Node::Ptr m_root;
	};
};

#include "btree.hpp"