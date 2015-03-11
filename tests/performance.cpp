#include <iostream>
#include <numeric>
#include <algorithm>
#include "BTree.h"

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Too few parameters.!" << std::endl;
		exit(1);
	}
	std::istringstream ss(argv[1]);
	int tree_size;
	if (!(ss >> tree_size)) {
		std::cout << "Invalid number " << argv[1] << std::endl;
		exit(1);
	}

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, tree_size);
	BTree tree4(4); // B-Tree with minimum degree 4

	std::vector<int> keys(tree_size); // vector with tree_size ints.
	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 9999.

	std::random_shuffle(std::begin(keys), std::end(keys)); // the first shufle
	std::for_each(keys.begin(), keys.end(), [&tree4](int key) { // add
				tree4.insert(key);
			});

	for (int i = 0; i != 100; ++i) {
		int key = distribution(generator);
		std::cout << "Searching for key " << key << "..." << std::endl;
		if (tree4.exist(key))
			std::cout << "Key " << key << " is found!" << std::endl;
	}

	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shufle
	std::for_each(keys.begin(), keys.end(), [&tree4](int key) { // remove
				tree4.remove(key);
			});

	exit(0);
}
