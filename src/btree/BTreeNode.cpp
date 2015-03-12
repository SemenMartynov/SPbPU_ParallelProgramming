#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include "BTreeNode.h"

/* Copy the given minimum degree and leaf property, allocate memory for
 * maximum number of possible keys and child pointers, initialize the number
 * of keys as 0
 */
BTreeNode::BTreeNode(int t, bool leaf) :
		t(t), children(new BTreeNode*[2 * t]), keys(new int[2 * t - 1]), keysnum(
				0), leaf(leaf) {
}

BTreeNode::~BTreeNode() {
	// travers through keysnum keys and first keysnum children

	for (int i = 0; i <= keysnum && !leaf; i++)
		delete children[i];

	delete[] keys;
	delete[] children;
}

std::string BTreeNode::to_str() {
	// There are keysnum keys and keysnum+1 children, travers through
	// keysnum keys and first keysnum children
	std::string result;
	int i;
	result.append(" [");
	for (i = 0; i < keysnum; i++) {
		// If this is not leaf, then before printing key[i],
		// traverse the subtree rooted with child C[i].
		if (!leaf)
			result.append(children[i]->to_str());
		result.append(" " + std::to_string(keys[i]));
	}

	// Print the subtree rooted with last child
	if (!leaf)
		result.append(children[i]->to_str());
	result.append(" ]");
	return result;
}

bool BTreeNode::exist(int key) {
	// Find the first key greater than or equal to k
	int i = 0;
	while (i < keysnum && key > keys[i])
		i++;

	// If the found key is equal to k, return this node
	if (keys[i] == key)
		return true;

	// If key is not found here and this is a leaf node
	if (leaf == true)
		return false;

	// Go to the appropriate child
	return children[i]->exist(key);
}

int BTreeNode::findKey(int key) {
	int idx = 0;
	while (idx < keysnum && keys[idx] < key)
		++idx;

	return idx;
}

void BTreeNode::insertNonFull(int key) {
	// Initialize index as index of rightmost element
	int i = keysnum - 1;

	// If this is a leaf node
	if (leaf) {
		// The following loop does two things
		// a) Finds the location of new key to be inserted
		// b) Moves all greater keys to one place ahead
		while (i >= 0 && keys[i] > key) {
			keys[i + 1] = keys[i];
			i--;
		}

		// Insert the new key at found location
		keys[i + 1] = key;
		keysnum = keysnum + 1;
		return;
	}
	// Find the child which is going to have the new key
	while (i >= 0 && keys[i] > key)
		i--;

	// See if the found child is full
	if (children[i + 1]->keysnum == 2 * t - 1) {
		// If the child is full, then split it
		splitChild(i + 1, children[i + 1]);

		// After split, the middle key of C[i] goes up and
		// C[i] is splitted into two.  See which of the two
		// is going to have the new key
		if (keys[i + 1] < key)
			i++;
	}
	children[i + 1]->insertNonFull(key);

}

void BTreeNode::splitChild(int i, BTreeNode* y) {

	// Create a new node which is going to store (t-1) keys
	// of y
	BTreeNode *z = new BTreeNode(y->t, y->leaf);
	z->keysnum = t - 1;

	// Copy the last (t-1) keys of y to z
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// Copy the last t children of y to z
	if (y->leaf == false) {
		for (int j = 0; j < t; j++)
			z->children[j] = y->children[j + t];
	}

	// Reduce the number of keys in y
	y->keysnum = t - 1;

	// Since this node is going to have a new child,
	// create space of new child
	for (int j = keysnum; j >= i + 1; j--)
		children[j + 1] = children[j];

	// Link the new child to this node
	children[i + 1] = z;

	// A key of y will move to this node. Find location of
	// new key and move all greater keys one space ahead
	for (int j = keysnum - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// Copy the middle key of y to this node
	keys[i] = y->keys[t - 1];

	// Increment count of keys in this node
	keysnum = keysnum + 1;
}

void BTreeNode::remove(int key) {
	int idx = findKey(key);

	// The key to be removed is present in this node
	if (idx < keysnum && keys[idx] == key) {
		// If the node is a leaf node - removeFromLeaf is called
		// Otherwise, removeFromNonLeaf function is called
		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	} else {
		// If this node is a leaf node, then the key is not present in tree
		if (leaf) {
			std::cout << "The key " << key
					<< " is does not exist in the tree\n";
			return;
		}

		// The key to be removed is present in the sub-tree rooted with this node
		// The flag indicates whether the key is present in the sub-tree rooted
		// with the last child of this node
		bool flag = ((idx == keysnum) ? true : false);

		// If the child where the key is supposed to exist has less that t keys,
		// we fill that child
		if (children[idx]->keysnum < t)
			fill(idx);

		// If the last child has been merged, it must have merged with the previous
		// child and so we recurse on the (idx-1)th child. Else, we recurse on the
		// (idx)th child which now has atleast t keys
		if (flag && idx > keysnum)
			children[idx - 1]->remove(key);
		else
			children[idx]->remove(key);
	}
	return;
}

void BTreeNode::removeFromLeaf(int idx) {
	// Move all the keys after the idx-th pos one place backward
	for (int i = idx + 1; i < keysnum; ++i)
		keys[i - 1] = keys[i];

	// Reduce the count of keys
	keysnum--;

	return;
}

void BTreeNode::removeFromNonLeaf(int idx) {
	int k = keys[idx];

// If the child that precedes k (C[idx]) has atleast t keys,
// find the predecessor 'pred' of k in the subtree rooted at
// C[idx]. Replace k by pred. Recursively delete pred
// in C[idx]
	if (children[idx]->keysnum >= t) {
		int pred = getPred(idx);
		keys[idx] = pred;
		children[idx]->remove(pred);
	}

// If the child C[idx] has less that t keys, examine C[idx+1].
// If C[idx+1] has atleast t keys, find the successor 'succ' of k in
// the subtree rooted at C[idx+1]
// Replace k by succ
// Recursively delete succ in C[idx+1]
	else if (children[idx + 1]->keysnum >= t) {
		int succ = getSucc(idx);
		keys[idx] = succ;
		children[idx + 1]->remove(succ);
	}

// If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
// into C[idx]
// Now C[idx] contains 2t-1 keys
// Free C[idx+1] and recursively delete k from C[idx]
	else {
		merge(idx);
		children[idx]->remove(k);
	}
	return;
}

int BTreeNode::getPred(int idx) {
// Keep moving to the right most node until we reach a leaf
	BTreeNode *cur = children[idx];
	while (!cur->leaf)
		cur = cur->children[cur->keysnum];

// Return the last key of the leaf
	return cur->keys[cur->keysnum - 1];
}

int BTreeNode::getSucc(int idx) {
// Keep moving the left most node starting from C[idx+1] until we reach a leaf
	BTreeNode *cur = children[idx + 1];
	while (!cur->leaf)
		cur = cur->children[0];

// Return the first key of the leaf
	return cur->keys[0];
}

void BTreeNode::fill(int idx) {
	// If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
	// from that child

	if (idx != 0 && children[idx - 1]->keysnum >= t)
		borrowFromPrev(idx);
	// If the next child(C[idx+1]) has more than t-1 keys, borrow a key
	// from that child
	else if (idx != keysnum && children[idx + 1]->keysnum >= t)
		borrowFromNext(idx);

	// Merge C[idx] with its sibling
	// If C[idx] is the last child, merge it with with its previous sibling
	// Otherwise merge it with its next sibling
	else {
		if (idx != keysnum)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

void BTreeNode::borrowFromPrev(int idx) {
	BTreeNode *child = children[idx];
	BTreeNode *sibling = children[idx - 1];

// The last key from C[idx-1] goes up to the parent and key[idx-1]
// from parent is inserted as the first key in C[idx]. Thus, the  loses
// sibling one key and child gains one key

// Moving all key in C[idx] one step ahead
	for (int i = child->keysnum - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

// If C[idx] is not a leaf, move all its child pointers one step ahead
	if (!child->leaf) {
		for (int i = child->keysnum; i >= 0; --i)
			child->children[i + 1] = child->children[i];
	}

// Setting child's first key equal to keys[idx-1] from the current node
	child->keys[0] = keys[idx - 1];

// Moving sibling's last child as C[idx]'s first child
	if (!leaf)
		child->children[0] = sibling->children[sibling->keysnum];

// Moving the key from the sibling to the parent
// This reduces the number of keys in the sibling
	keys[idx - 1] = sibling->keys[sibling->keysnum - 1];

	child->keysnum += 1;
	sibling->keysnum -= 1;

	return;
}

void BTreeNode::borrowFromNext(int idx) {
	BTreeNode *child = children[idx];
	BTreeNode *sibling = children[idx + 1];

// keys[idx] is inserted as the last key in C[idx]
	child->keys[(child->keysnum)] = keys[idx];

// Sibling's first child is inserted as the last child
// into C[idx]
	if (!(child->leaf))
		child->children[(child->keysnum) + 1] = sibling->children[0];

//The first key from sibling is inserted into keys[idx]
	keys[idx] = sibling->keys[0];

// Moving all keys in sibling one step behind
	for (int i = 1; i < sibling->keysnum; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

// Moving the child pointers one step behind
	if (!sibling->leaf) {
		for (int i = 1; i <= sibling->keysnum; ++i)
			sibling->children[i - 1] = sibling->children[i];
	}

// Increasing and decreasing the key count of C[idx] and C[idx+1]
// respectively
	child->keysnum += 1;
	sibling->keysnum -= 1;

	return;
}

void BTreeNode::merge(int idx) {
	BTreeNode *child = children[idx];
	BTreeNode *sibling = children[idx + 1];

	// Pulling a key from the current node and inserting it into (t-1)th
	// position of C[idx]
	child->keys[t - 1] = keys[idx];

	// Copying the keys from C[idx+1] to C[idx] at the end
	for (int i = 0; i < sibling->keysnum; ++i)
		child->keys[i + t] = sibling->keys[i];

	// Copying the child pointers from C[idx+1] to C[idx]
	if (!child->leaf) {
		for (int i = 0; i <= sibling->keysnum; ++i)
			child->children[i + t] = sibling->children[i];
	}

	// Moving all keys after idx in the current node one step before -
	// to fill the gap created by moving keys[idx] to C[idx]
	for (int i = idx + 1; i < keysnum; ++i)
		keys[i - 1] = keys[i];

	// Moving the child pointers after (idx+1) in the current node one
	// step before
	for (int i = idx + 2; i <= keysnum; ++i)
		children[i - 1] = children[i];

	// Updating the key count of child and the current node
	child->keysnum += sibling->keysnum + 1;
	keysnum--;

	// Freeing the memory occupied by sibling
	sibling->leaf = true;
	delete (sibling);
	return;
}
