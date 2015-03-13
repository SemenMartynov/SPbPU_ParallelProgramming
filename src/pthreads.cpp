#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>

#include <chrono>
#include <thread>

#include <pthread.h>
#include "btree/BTree.h"

#define keys_number 1000

pthread_rwlock_t rwlock;
BTree tree4(4); // B-Tree with minimum degree 4

std::vector<int> keys(keys_number); // vector with keys_number ints.

// random generator
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0, keys_number);

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
		std::this_thread::sleep_for(
				std::chrono::milliseconds(distribution(generator) / 2)); //sleep
		pthread_rwlock_unlock(&rwlock);
	}
	return nullptr;
}

void* insert(void* param) {
	// feel vector
	std::for_each(keys.begin(), keys.end(), [&](int key) {
		pthread_rwlock_wrlock(&rwlock);
		tree4.insert(key);
		std::this_thread::sleep_for(
				std::chrono::milliseconds(distribution(generator) / 16)); //sleep
			pthread_rwlock_unlock(&rwlock);
		});

	return nullptr;
}

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

	pthread_rwlock_init(&rwlock, nullptr);

	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 9999.
	std::random_shuffle(std::begin(keys), std::end(keys)); // the first shuffle

	// Number of processor cores
	unsigned concurentThreads = std::thread::hardware_concurrency() - 1;
	if (concurentThreads < 1)
		concurentThreads = 1;
	// Threads
	std::vector<pthread_t> threads(concurentThreads);
	pthread_t inserter;
	int tasks = task_size / concurentThreads;

	// starts threads
	pthread_create(&inserter, NULL, insert, NULL);
	for (auto thread : threads)
		pthread_create(&thread, NULL, check, &tasks);

	// Wait for the filling of the vector, and then run the cleanup.
	pthread_join(inserter, NULL);
	std::for_each(keys.begin(), keys.end(), [&](int key) {
		pthread_rwlock_wrlock(&rwlock);
		tree4.remove(distribution(generator));
		std::this_thread::sleep_for(
				std::chrono::milliseconds(distribution(generator) / 16)); //sleep
			pthread_rwlock_unlock(&rwlock);
		});

	// wait for all threads to complete.
	for (auto thread : threads)
		pthread_join(thread, NULL);

	pthread_rwlock_destroy(&rwlock);
	return 0;
}
