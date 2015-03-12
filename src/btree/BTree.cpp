#include <iostream>
#include "BTree.h"

BTree::BTree(int t) :
		t(t), root(nullptr) {
}

BTree::~BTree() {
	if (root)
		delete root;
}

std::string BTree::to_str() {
	return (root) ? root->to_str() : std::string(" [ ]");
}

bool BTree::exist(int key) {
	return (root) ? root->exist(key) : false;
}

bool BTree::insert(int key) {
	// If tree is empty
	if (!root) {
		// Allocate memory for root
		root = new BTreeNode(t, true);
		root->keys[0] = key; // Insert key
		root->keysnum = 1; // Update number of keys in root
		return true;
	}

	// If root is not full, call insertNonFull for root
	if (root->keysnum != 2 * t - 1) {
		root->insertNonFull(key);
		return true;
	}

	// If root is full, then tree grows in height
	// Allocate memory for new root
	BTreeNode *newroot = new BTreeNode(t, false);

	// Make old root as child of new root
	newroot->children[0] = root;

	// Split the old root and move 1 key to the new root
	newroot->splitChild(0, root);

	// New root has two children now.  Decide which of the
	// two children is going to have new key
	int i = 0;
	if (newroot->keys[0] < key)
		i++;
	newroot->children[i]->insertNonFull(key);

	// Change root
	root = newroot;
	return true;
}

bool BTree::remove(int key) {
	if (!root) {
		return false;
	}

	// Call the remove function for root
	root->remove(key);

	// If the root node has 0 keys, make its first child as the new root
	//  if it has a child, otherwise set root as NULL
	if (root->keysnum == 0) {
		BTreeNode *tmp = root;

		if (root->leaf)
			root = nullptr;
		else {
			root = root->children[0];
			tmp->children[0] = nullptr;
		}

		// Free the old root
		delete tmp;
	}
	return true;
}
