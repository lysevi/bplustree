#pragma once

#include <vector>
#include <memory>

namespace trees{

	class BTree
	{
		struct Node
		{
			typedef std::shared_ptr<Node> Ptr;
			std::vector<int> vals;       // n >= size < 2*n
			std::vector<Ptr> childs;   // size(vals)+1
		};
		
	public:
		BTree() = delete;
		BTree(size_t N);

		int find(int key)const;
		bool insert(int key);
	protected:
		bool iner_find(int key, Node::Ptr&out_ptr, int &out_res)const; // return last_node, false if fail, or cur_node,true;
		static Node::Ptr make_root();
	private:
		size_t n;
		Node::Ptr m_root;
	};
}
