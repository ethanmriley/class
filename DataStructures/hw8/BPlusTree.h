/* ASSUMPTIONS:
1. When a node must be split and has an odd number of keys, the extra key will go to the new right-hand node.
2. There will never be a duplicate key passed to insert.
*/

#include <vector>
#include <algorithm>
#include <math.h>

#ifndef DS_BPLUSTREE
#define DS_BPLUSTREE

///////////////////////////////////////////////////////////////////////////////
//DO NOT CHANGE THIS CODE
///////////////////////////////////////////////////////////////////////////////
template <class T> class BPlusTree;

template <class T>
class BPlusTreeNode{
public:
	BPlusTreeNode() : parent(NULL) {};
	bool is_leaf();
	bool contains(const T& key);

	//For grading only, this is actually bad to have because
	//it exposes a private member variable.
	BPlusTreeNode* get_parent() { return parent; } 

	//We need this to let BPlusTree access private members
	friend class BPlusTree<T>; 
private:
	bool contains(const T& key,std::size_t low,std::size_t high);
	std::vector<T> keys;
	std::vector<BPlusTreeNode*> children;
	BPlusTreeNode* parent;
};

template <class T>
bool BPlusTreeNode<T>::is_leaf(){
	for(unsigned int i=0; i<children.size(); i++){
		if(children[i]){
			return false;
		}
	}
	return true;
}

template <class T>
bool BPlusTreeNode<T>::contains(const T& key){
	return contains(key,0,keys.size()-1);
}

//Private method takes advantage of the fact keys are sorted
template <class T>
bool BPlusTreeNode<T>::contains(const T& key,std::size_t low,std::size_t high){
	if(low>high){
		return false;
	}
	if(low==high){
		return key == keys[low];
	}
	std::size_t mid = (low+high)/2;
	if(key<=keys[mid]){
		return contains(key,0,mid);
	}
	else{
		return contains(key,mid+1,high);
	}
}

///////////////////////////////////////////////////////////////////////////////////////
//Your BPlusTree implementation goes below this point.
///////////////////////////////////////////////////////////////////////////////////////

//a (shoddy) B+ tree implementation
//ethan riley, 2017

template <class T>
class BPlusTree{
public:
	BPlusTree(int def_b);
	~BPlusTree();
	BPlusTree(const BPlusTree<T>& copy_tree);
	BPlusTreeNode<T>* find(const T& key) const;
	void insert(const T& key);
	unsigned int b;
	void print_sideways(std::ofstream& outfile) const;
	void print_BFS(std::ofstream& outfile) const;

private:
	BPlusTreeNode<T>* root = NULL;
	BPlusTreeNode<T>* find(const T& key, BPlusTreeNode<T>* node) const;
	void recursiveDelete(BPlusTreeNode<T>* node);
	void recursiveCopy(BPlusTreeNode<T>* node, BPlusTreeNode<T>* copy_node);
	void splitLeaf(const T& key, BPlusTreeNode<T>* insert_leaf);
	void recursivePrintSideways(std::ofstream& outfile, BPlusTreeNode<T>* node, int depth) const;

};

//==============
//	destructor
template <class T>
BPlusTree<T>::~BPlusTree() {
	recursiveDelete(root);
}

template <class T>
void BPlusTree<T>::recursiveDelete(BPlusTreeNode<T>* node) {
	//post-order traversal of the tree used to delete it
	if (node) {
		for (unsigned int i = 0; i < node->children.size(); i++) {
			recursiveDelete(node->children[i]);
		}
		delete node;
	}
}

//==============




//==============
//	copy constructor
template <class T>
BPlusTree<T>::BPlusTree(const BPlusTree<T>& copy_tree) {
	b = copy_tree.b;
	root = new BPlusTreeNode<T>;
	recursiveCopy(root, copy_tree.root);
}

//my copy constructor traverses the tree, copying each existing node into a new node
template <class T>
void BPlusTree<T>::recursiveCopy(BPlusTreeNode<T>* node, BPlusTreeNode<T>* copy_node) {
	//copy over keys from the existing node to the new node
	for (unsigned int i = 0; i < copy_node->keys.size(); i++) {
		node->keys.push_back(copy_node->keys[i]);
	}
	//for each existing child, follow it down and copy it
	for (unsigned int j = 0; j < copy_node->children.size(); j++) {
		if (copy_node->children[j]) {
			BPlusTreeNode<T>* new_node = new BPlusTreeNode<T>;
			new_node->parent = node; 
			recursiveCopy(new_node, copy_node->children[j]);
		} else {
			node->children.push_back(NULL);
		}
	}
}
//==============



//==============
//	insert()
template <class T>
//splits a leaf and divides its keys
void BPlusTree<T>::splitLeaf(const T& key, BPlusTreeNode<T>* insert_leaf) {
	unsigned int mid = b/2;
	BPlusTreeNode<T>* new_node1 = new BPlusTreeNode<T>;
	BPlusTreeNode<T>* new_node2 = new BPlusTreeNode<T>;
	new_node1->parent = insert_leaf;
	new_node2->parent = insert_leaf;

	for (unsigned int k = 0; k < b; k++) {
		new_node1->children.push_back(NULL);
		new_node2->children.push_back(NULL);
	}

	assert(mid < insert_leaf->keys.size()); //sanity check

	for(unsigned int i = 0; i < mid; i++) { //put the lower half of the keys into node 1
		new_node1->keys.push_back(insert_leaf->keys[i]);
	}
	for (unsigned int j = mid; j < insert_leaf->keys.size(); j++){ //put the upper half of the keys into node 2
		new_node2->keys.push_back(insert_leaf->keys[j]);
	}

	//ideally, this reassigns the middle value to the new parent (what used to be insert_leaf)
	T tmp_val = insert_leaf->keys[mid];
	insert_leaf->keys.clear();
	insert_leaf->keys.push_back(tmp_val);

	insert_leaf->children[0] = new_node1;
	insert_leaf->children[1] = new_node2;
}

template <class T> 
//main insert function
//it should work better
void BPlusTree<T>::insert(const T& key) {
	BPlusTreeNode<T>* insert_leaf;
	if (root) {
		insert_leaf = find(key); //finds the right place to insert the new value
		if (insert_leaf->keys.size() + 1 == b) { //if the insertion would make the leaf too big, split it
			splitLeaf(key, insert_leaf);
		} else if (insert_leaf->keys.size() + 1 > b){ //little check to make sure I didn't overrun the bounds somehow
			std::cerr << "stuff is really messed up" << std::endl;
		} else { //most insertions will just be placed into a leaf without any splitting, which is what this does
			insert_leaf->keys.push_back(key);
		}
	} else { //if the root is null, create and populate it
		insert_leaf = root = new BPlusTreeNode<T>;
		root->keys.push_back(key);
		for (unsigned int i = 0; i < b; i++) {
			root->children.push_back(NULL);
		}
	}
	//my attempt to sort keys (probably the source of a bug)
	std::sort( (insert_leaf->keys).begin(), (insert_leaf->keys).end());
}
//==============



//==============
//	default constructor
template <class T>
BPlusTree<T>::BPlusTree(int def_b) {
	b = def_b;
}
//==============



//==============
//	find()
template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& key) const {
	if (root) { //if the root exists, search the tree
		return find(key, root);
	} else {    //otherwise, return NULL
		return NULL;	
	}

}

template <class T>
BPlusTreeNode<T>* BPlusTree<T>::find(const T& key, BPlusTreeNode<T>* node) const {
	if (node->is_leaf()) {
		return node;
	}

	//for each key that the search term is larger than, increment the child index
	//i'm sure this is bugged
	unsigned int child_index = 0;
	for (unsigned int i = 0; i < node->keys.size(); i++) {
		if (key >= node->keys[i]) {
			child_index++;
		}
	}

	return find(key, node->children[child_index]);
}
//==============


//==============
//	print
template <class T>
void BPlusTree<T>::recursivePrintSideways(std::ofstream& outfile, BPlusTreeNode<T>* node, int depth) const{
	//printSideways is an in-order traversal, which should ideally work with variable-sized trees

	//go down the "left" subtree
	unsigned int mid = b/2;
	for (unsigned int i = 0; i < mid; i++) {
		if (node->children[i]) {
			recursivePrintSideways(outfile, node->children[i], depth+1);
		}
	}

	//print data
	std::string indent;
	for (int l = 0; l < depth; l++) {
		indent+='\t';
	}

	outfile << indent;
	for (unsigned int j = 0; j < node->keys.size(); j++) {
		outfile << node->keys[j];
		if (j < node->keys.size()-1) {
			outfile << ',';
		}
	}
	outfile << std::endl;


	//go down the "right" subtree
	for (unsigned int k = mid; k < node->keys.size(); k++) {
		if (node->children[k]) {
			recursivePrintSideways(outfile, node->children[k], depth+1);
		}
	}	
}


//helper function for sideways print
template <class T>
void BPlusTree<T>::print_sideways(std::ofstream& outfile) const {
	if (root) {
		recursivePrintSideways(outfile, root, 0);
	}else {
		outfile << "Tree is empty." << std::endl;
	}
}

//breadth-first print
//implemented iteratively, with a vector of vectors of nodes
template <class T>
void BPlusTree<T>::print_BFS(std::ofstream& outfile) const {
	if (root) {
		std::vector<std::vector<BPlusTreeNode<T>* > > q;
		unsigned int position = 0;
		
		std::vector<BPlusTreeNode<T>* > layer;

		layer.push_back(root);
		q.push_back(layer);

		while (position < q.size()) { //go through one "layer" of the tree
			layer.clear();
			//for each layer in the tree, print the keys
			//additionally, add the children to the next layer
			for (unsigned int k = 0; k < q[position].size(); k++) {
				for (unsigned int i = 0; i < q[position][k]->keys.size(); i++) {
					outfile << q[position][k]->keys[i];
					if (i < q[position][k]->keys.size() - 1) {
						outfile << ',';
					}
				}
				outfile << '\t';

				for (unsigned int j = 0; j < q[position][k]->children.size(); j++) {
					if (q[position][k]->children[j]) {
						layer.push_back(q[position][k]->children[j]);
					}
				}
			}

			//increment the layer and add the next set of children to the queue (if they exist)
			position++;
			if (layer.size() > 0) {
				q.push_back(layer);
			}
			outfile << std::endl;

		}

	}else {
		outfile << "Tree is empty." << std::endl;
	}
}
//==============

#endif