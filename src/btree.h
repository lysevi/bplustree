#pragma once

#include <vector>
#include <memory>

namespace trees
{

	class BTree
	{
		struct Node
		{
			typedef std::shared_ptr<Node> Ptr;
			typedef std::weak_ptr<Node> Weak;
			std::vector<int> vals;       // n >= size < 2*n
			std::vector<Ptr> childs;   // size(vals)+1
			Node::Weak parent;
			Node();
			~Node();
			void insertValue(int key);
            void insertChild(Node::Ptr C);
		};

	public:
		BTree() = delete;
		BTree(size_t N);
		~BTree();

		int find(int key)const;
		bool insert(int key); //true if key was inserted. false, if not (maybe she alrady exists)
		void print()const;
	protected:
		static Node::Ptr make_node();
		bool iner_find(int key,Node::Ptr cur_node, Node::Ptr&out_ptr, int &out_res)const; // return last_node, false if fail, or cur_node,true;
		bool isFull(const Node::Ptr node)const;
		void split_node(Node::Ptr node);
		void print(const Node::Ptr& root)const;
	private:
		size_t n;
		Node::Ptr m_root;
	};
}
