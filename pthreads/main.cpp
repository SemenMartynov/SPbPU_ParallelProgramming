#include <iostream>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <thread>
#include <pthread.h>
#include "BTree.h"

pthread_rwlock_t rwlock;
BTree tree4(4); // B-Tree with minimum degree 4

std::vector<int> keys(1000); // vector with 10000 ints.

// random generator
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, 1000);

void* check(void* param) {
	int tasks = *(int*) param;

	for (int i = 0; i != tasks; ++i) {
		pthread_rwlock_rdlock(&rwlock);
		int key = distribution(generator);
		std::cout << "Searching for key " << key << "..." << std::endl;
		if (tree4.exist(key)) {
			std::cout << "Key " << key << " is found!" << std::endl;
			std::cout.flush();
		}
		pthread_rwlock_unlock(&rwlock);
		std::this_thread::sleep_for(
				std::chrono::milliseconds(distribution(generator) / 2));
	}
	return nullptr;
}

void* insert(void* param) {
	// feel vector
	std::for_each(keys.begin(), keys.end(), [&](int key) {
		pthread_rwlock_wrlock(&rwlock);
		tree4.insert(key);
		pthread_rwlock_unlock(&rwlock);
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	});

	return nullptr;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cout << "Too few parameters.!" << std::endl;
		exit(1);
	}
	std::istringstream ss(argv[1]);
	int task_size;
	if (!(ss >> task_size)) {
		std::cout << "Invalid number " << argv[1] << std::endl;
		exit(1);
	}

	pthread_rwlock_init(&rwlock, nullptr);

	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 9999.
	std::random_shuffle(std::begin(keys), std::end(keys)); // the first shufle

	// threads
	std::vector<pthread_t> threads(4);
	pthread_t inserter;
	int tasks = 10;

	// starts threads
	pthread_create(&inserter, NULL, insert, NULL);
	std::this_thread::sleep_for(std::chrono::seconds(1));
	for (auto thread : threads)
		pthread_create(&thread, NULL, check, &tasks);

	// clean vector
	std::this_thread::sleep_for(std::chrono::seconds(2));
	std::for_each(keys.begin(), keys.end(), [&](int key) {
		pthread_rwlock_wrlock(&rwlock);
		tree4.remove(distribution(generator));
		pthread_rwlock_unlock(&rwlock);
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	});

	// wait for all threads to complete.
	for (auto thread : threads)
		pthread_join(thread, NULL);
	pthread_join(inserter, NULL);

	pthread_rwlock_destroy(&rwlock);
	return 0;
}
