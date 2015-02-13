#ifndef TEST_BTREE_H_
#define TEST_BTREE_H_

#include "BTreeNode.h"

/**
 * BTree
 */
class BTree {
public:
	/**
	 * @brief Constructor (Initializes tree as empty)
	 *
	 * @param t minimum degree
	 */
	BTree(int t);

	/**
	 * @brief Destructor
	 */
	virtual ~BTree();
	BTree(BTree&&) = delete;
	BTree(const BTree&) = delete;
	BTree& operator=(BTree&&) = delete;
	BTree& operator=(const BTree&) = delete;

	/**
	 * A function to traverse all nodes in a subtree
	 *
	 * @return
	 */
	std::string to_str();

	/**
	 * function to search a key in this tree
	 *
	 * @param key
	 * @return
	 */
	bool exist(int key);

	/**
	 * The main function that inserts a new key in this B-Tree
	 *
	 * @param key
	 * @return
	 */
	bool insert(int key);

	/**
	 * The main function that removes a new key in thie B-Tree
	 *
	 * @param key
	 * @return
	 */
	bool remove(int key);

private:
	const int t; /**< Minimum degree*/
	BTreeNode *root; /**< Pointer to root node*/
};

#endif /* TEST_BTREE_H_ */
