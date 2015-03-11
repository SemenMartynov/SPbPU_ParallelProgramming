#include <iostream>
#include <vector>
#include <future>
#include <algorithm>
#include <chrono>
#include <thread>
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

	std::vector<int> keys(tree_size); // vector with 10000 ints.
	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 9999.

	std::random_shuffle(std::begin(keys), std::end(keys)); // the first shufle
	std::for_each(keys.begin(), keys.end(), [&tree4](int key) { // add
				tree4.insert(key);
			});

	std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
	std::vector<std::future<void>> futures;
	for (int i = 0; i < 20; ++i) {
		auto fut = std::async([&]
		{
			int key = distribution(generator);
			std::cout << "Searching for key " << key << "..." << std::endl;
			if (tree4.exist(key))
			std::cout << "Key " << key << " is found!" << std::endl;
		});
		futures.push_back(std::move(fut));
	}
	std::for_each(futures.begin(), futures.end(), [](std::future<void> &fut)
	{
		fut.wait();
	});

	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shufle
	std::for_each(keys.begin(), keys.end(), [&tree4](int key) { // remove
				tree4.remove(key);
			});

	return 0;
}
