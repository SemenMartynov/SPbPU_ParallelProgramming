#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <vector>

#include <chrono>
#include <thread>
#include "btree/BTree.h"

#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>

#include "btree/BTree.h"

// The number of child processes
const int thread_num = 6;

// How many requests each child process executes
const int task_num = 20;

// The number of simultaneously active processes.
const int sem_num = 4;

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

	//Actually I do not really care about these variables,
	//but they are needed later.
	pid_t wpid;
	int status = 0;

	// timer for random generator
	typedef std::chrono::high_resolution_clock clock;
	clock::time_point beginning = clock::now();

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 10000);
	BTree tree4(4); // B-Tree with minimum degree 4

	// Create and initialize semaphore
	sem_t mutex;
	if (sem_init(&mutex, 1, sem_num) < 0) {
		std::perror("Semaphore initialization");
		exit(0);
	}

	std::vector<int> keys(10000); // vector with 10000 ints.
	std::iota(keys.begin(), keys.end(), 0); // Fill with 0, 1, ..., 9999.

	std::random_shuffle(keys.begin(), keys.end()); // the first shuffle
	for (auto key : keys) { // add
		tree4.insert(key);
		std::this_thread::sleep_for(
				std::chrono::milliseconds(distribution(generator) / 16));
	}

	// Create and run children
	for (int i = 0; i != thread_num; ++i) {
		if (fork() == 0) { // child process
			// obtain a seed from the timer
			clock::duration seed = clock::now() - beginning;
			generator.seed(static_cast<uint64_t>(seed.count()));
			for (int j = 0; j != task_num; j++) {
				sem_wait(&mutex);
				int key = distribution(generator);
				std::cout << "Searching for key " << key << "..." << std::endl;
				if (tree4.exist(key))
					std::cout << "Key " << key << " is found!" << std::endl;
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 2));
				sem_post(&mutex);
			}
			exit(0);
		}
	}

	// The parent process waits for the semaphore capture on par with everyone.
	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shuffle
	std::for_each(keys.begin(), keys.end(), [&](int key) { // remove
				sem_wait(&mutex);
				tree4.remove(key);
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 16));
				sem_post(&mutex);
			});

	// this way, the father waits for all the child processes
	while ((wpid = wait(&status)) > 0)
		;

	sem_close(&mutex);
	return 0;
}
