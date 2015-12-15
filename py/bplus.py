after=False
class Node:
    def __init__(self, is_leaf=False):
        self.keys=[]
        self.childs=[]
        self.is_leaf=is_leaf
        self.next=None
        self.parent=None

    def add_value(self,key):
        if self.is_leaf==True:
            self.keys.append(key)

    def find_key(self,key):
        for i in range(len(self.keys)):
            if self.keys[i]==key:
                return key

    def size(self):
        return len(self.keys)
    
    def add_child(self,key,node):
        if key==self.keys[len(self.keys)-1]:
            self.childs.append(key)
        else:
            for i in range(len(self.keys)):
                if self.keys[i]==key:
                    if len(self.childs)<(i-1):
                        self.childs.append(node)
                    else:
                        self.childs.insert(i+1,node)
                    return

class Tree:
    def __init__(self):
        self.T=3
        self.root=None
        self.nodes=[]
        self.leafs=[]

    def search(self,key):
        #print "search"
        if self.root==None:
            #print "root is None"
            return None
        else:
            return self.tree_search(key,self.root)
            
    def tree_search(self,key, node):
        if node is None:
            #print "search isNone"
            return None

        if node.is_leaf:
            #print "search isLeaf"
            return node

        if key < node.keys[0]:
            return self.tree_search(key,node.childs[0])

        if key<= node.keys[len(node.keys)-1] and  key<= node.keys[len(node.keys)-2]:
            return self.tree_search(key,node.childs[len(node.keys)-1])

        for i in range(len(node.keys)):
            #print i, node.keys
            if len(node.keys)==1:
                return self.tree_search(key,node.childs[0])

            if node.keys[i] <=key and key > node.keys[i+1]:
                return self.tree_search(key,node.childs[i+1])

    def add(self, key):
        #print "add",key
        finded=self.tree_search(key,self.root)
        if finded==None:
            print "find None"
            l=Node(is_leaf=True)
            l.add_value(key)
            self.root=l
            self.nodes.append(l)
            self.leafs.append(l)
        else:
            print "finded"
            finded.add_value(key)
            if finded.size()>=self.T:
                self.split_node(finded)

    def split_node(self,node):
        print "split"
        new_node=Node(is_leaf=node.is_leaf)
        new_node.keys=node.keys[len(node.keys)/2:]
        new_node.childs=node.childs[len(node.childs)/2:]
        new_node.parent=node.parent
        node.next=new_node

        middle=node.keys[len(node.keys)/2]
        
        node.keys=node.keys[:len(node.keys)/2]
        node.childs=node.childs[:len(node.keys)/2]

        if node.parent is None: #is root
            print "split isRoot"
            newroot=Node()
            newroot.is_leaf=False
            newroot.keys.append(middle)
            newroot.childs.append(node)
            newroot.childs.append(new_node)
            self.root=newroot
            print "root:", self.root.keys
            for c in self.root.childs:
                print c.keys
        else:
            parent=node.parent
            parent.keys.append(middle)
            parent.add_child(module,new_node)
            if parent.size()>=self.T:
                self.split_node(parent)

    def print_tree(self):
        self.inner_print(self.root)
    def inner_print(self,node):
        print "{ [",
        for i in node.keys:
            print " ",i,
        print "], ",
        for i in node.childs:
            self.inner_print(i)
        print "}"

t=Tree()
inserted=[]
def test_range(start,stop):
    for i in range(start,stop):
        print "* insert", i
        t.add(i)
        t.print_tree()
        inserted.append(i)
        n=t.search(i)
        if n.find_key(i)!=i:
            print "not found:",i
            break

test_range(10,20)
# test_range(0,9)
# test_range(25,100)
# test_range(21,24)


# after=True
# print "*"*20
# for i in inserted:
#     n=t.search(i)
#     if n.find_key(i)!=i:
#         t.print_tree()
#         print "inserted not found:",i

print("done")
