#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>

#include <chrono>
#include <thread>

#include <omp.h>
#include "btree/BTree.h"

#define keys_number 1000

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

	int nthreads, tid;
	/* Fork a team of threads with each thread having a private tid variable */
#pragma omp parallel private(tid)
	{
		/* Obtain and print thread id */
		tid = omp_get_thread_num();
		std::cout << "Thread " << tid << " active" << std::endl;

		/* Only master thread does this */
		if (tid == 0) {
			std::cout << "Number of threads = " << omp_get_num_threads()
					<< std::endl;

			std::random_shuffle(std::begin(keys), std::end(keys)); // the first shuffle
			std::for_each(keys.begin(), keys.end(), [&](int key) { // add
						tree4.insert(key);
						std::this_thread::sleep_for(
								std::chrono::milliseconds(distribution(generator) / 16));//sleep
					});

			// I would like to place delete-records-code here,
			// but there is no RWLocak in OpenMP
		} else { // In other parallel threads
			// Get threads number
			nthreads = omp_get_num_threads() - 1;
			if (nthreads < 1)
				nthreads = 1;
#pragma omp parallel for
			for (int i = 0; i < task_size / nthreads; ++i) {
				int key = distribution(generator);
				std::cout << "#" << tid << " Searching for key " << key << "..."
						<< std::endl;
				if (tree4.exist(key))
					std::cout << "Key " << key << " is found!" << std::endl;
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 2));
			}
		}
	} /* All threads join master thread and terminate */
	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shuffle
	std::for_each(keys.begin(), keys.end(), [&](int key) { // remove
				tree4.remove(key);
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 16));//sleep
			});

	return 0;
}
