#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <future>

#include <chrono>
#include <thread>

#include "btree/BTree.h"

#define keys_number 1000

// std::shared_mutex will be part of the C++14 Standard Library
int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Too few parameters!" << std::endl;
		exit(1);
	}
	std::istringstream ss(argv[1]);
	int task_size;
	if (!(ss >> task_size)) {
		std::cout << "Invalid number " << argv[1] << std::endl;
		exit(1);
	}

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, keys_number);
	BTree tree4(4); // B-Tree with minimum degree 4

	std::vector<int> keys(keys_number); // vector with keys_number ints.
	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 9999.

	std::random_shuffle(std::begin(keys), std::end(keys)); // the first shuffle
	std::for_each(keys.begin(), keys.end(), [&](int key) { // add
				tree4.insert(key);
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 16));//sleep
			});

	std::cout << "Main thread id: " << std::this_thread::get_id() << std::endl;
	std::vector<std::future<void>> futures;

	for (int i = 0; i != task_size; ++i) {
		auto fut = std::async(std::launch::async, [&]
		{
			int key = distribution(generator);
			std::cout << "Searching for key " << key << "..." << std::endl;
			if (tree4.exist(key))
			std::cout << "Key " << key << " is found!" << std::endl;
			std::this_thread::sleep_for(
					std::chrono::milliseconds(distribution(generator) / 2)); //sleep
			});
		futures.push_back(std::move(fut));
	}
	std::for_each(futures.begin(), futures.end(), [](std::future<void> &fut)
	{
		fut.wait();
	});

	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shuffle
	std::for_each(keys.begin(), keys.end(), [&](int key) { // remove
				tree4.remove(key);
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 16));//sleep
			});

	return 0;
}
