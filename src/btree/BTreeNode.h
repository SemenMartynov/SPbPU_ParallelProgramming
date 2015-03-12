#ifndef TEST_BTREENODE_H_
#define TEST_BTREENODE_H_

/**
 * BTreeNode
 */
class BTreeNode {
public:
	/**
	 * @brief Constructor
	 *
	 * @param t minimum degree
	 * @param leaf
	 */
	BTreeNode(int t, bool leaf);

	/**
	 * @brief Destructor
	 */
	virtual ~BTreeNode();
	//virtual ~BTreeNode() = default;
	BTreeNode(BTreeNode&&) = delete;
	BTreeNode(const BTreeNode&) = delete;
	BTreeNode& operator=(BTreeNode&&) = delete;
	BTreeNode& operator=(const BTreeNode&) = delete;

	/**
	 * A function to traverse all nodes in a subtree rooted with this node
	 *
	 * @return
	 */
	std::string to_str();

	/**
	 * A function to search a key in subtree rooted with this node.
	 *
	 * @param key
	 * @return NULL if key is not present
	 */
	bool exist(int key);

	/**
	 * A function that returns the index of the first key that is greater
	 * or equal to key
	 *
	 * @param key
	 * @return
	 */
	int findKey(int key);

	/**
	 * A utility function to insert a new key in the subtree rooted with
	 * this node. The assumption is, the node must be non-full when this
	 * function is called
	 *
	 * @param key
	 */
	void insertNonFull(int key);

	/**
	 * A utility function to split the child y of this node. i is index
	 * of y in child array C[].  The Child y must be full when this
	 * function is called
	 *
	 * @param i
	 * @param y
	 */
	void splitChild(int i, BTreeNode *y);

	/**
	 * A wrapper function to remove the key key in subtree rooted with
	 * this node.
	 *
	 * @param key
	 */
	void remove(int key);

	/**
	 * A function to remove the key present in idx-th position in
	 * this node which is a leaf
	 *
	 * @param idx
	 */
	void removeFromLeaf(int idx);

	/**
	 * A function to remove the key present in idx-th position in
	 * this node which is a non-leaf node
	 *
	 * @param idx
	 */
	void removeFromNonLeaf(int idx);

	/**
	 * A function to get the predecessor of the key- where the key
	 * is present in the idx-th position in the node
	 *
	 * @param idx
	 * @return
	 */
	int getPred(int idx);

	/**
	 * A function to get the successor of the key- where the key
	 * is present in the idx-th position in the node
	 *
	 * @param idx
	 * @return
	 */
	int getSucc(int idx);

	/**
	 * A function to fill up the child node present in the idx-th
	 * position in the C[] array if that child has less than t-1 keys
	 * @param idx
	 */
	void fill(int idx);

	/**
	 * A function to borrow a key from the C[idx-1]-th node and place
	 * it in C[idx]th node
	 *
	 * @param idx
	 */
	void borrowFromPrev(int idx);

	/**
	 * A function to borrow a key from the C[idx+1]-th node and place it
	 * in C[idx]th node
	 *
	 * @param idx
	 */
	void borrowFromNext(int idx);

	/**
	 * A function to merge idx-th child of the node with (idx+1)th child of
	 * the node
	 *
	 * @param idx
	 */
	void merge(int idx);

	/**
	 * Make BTree friend of this so that we can access private members of
	 * this class in BTree functions
	 */
	friend class BTree;

private:
	const int t; /**< Minimum degree (defines the range for number of keys)*/
	BTreeNode** children; /**< An array of child pointers*/
	int* keys; /**< An array of keys*/
	int keysnum; /**< Current number of keys*/
	bool leaf; /**< Is true when node is leaf. Otherwise false*/
};

#endif /* TEST_BTREENODE_H_ */
