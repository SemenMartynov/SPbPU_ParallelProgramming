#include <iostream>
#include <numeric>
#include <algorithm>
#include <sstream>
#include <vector>

#include <chrono>
#include <thread>

#include <semaphore.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "btree/BTree.h"

#define keys_number 1000

// The number of child processes
const int thread_num = 6;

// How many requests each child process executes
int task_num = 20;

// The number of simultaneously active processes.
const int sem_num = std::thread::hardware_concurrency();

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
	task_num = task_size / thread_num;

	//Actually I do not really care about these variables,
	//but they are needed later.
	pid_t wpid;
	int status = 0;

	// timer for random generator
	typedef std::chrono::high_resolution_clock clock;
	clock::time_point beginning = clock::now();

	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, keys_number);
	BTree tree4(4); // B-Tree with minimum degree 4

	// Create and initialize semaphore
	int shm;
	sem_t * mutex;

	if ((shm = shm_open("/myshm", O_CREAT | O_TRUNC | O_RDWR, 0666)) < 0) {
		std::perror("shm_open");
		exit(1);
	}

	if (ftruncate(shm, sizeof(sem_t)) < 0) {
		std::perror("ftruncate");
		exit(1);
	}

	if ((mutex = (sem_t*)mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED,
			shm, 0)) == MAP_FAILED) {
		std::perror("mmap");
		exit(1);
	}

	if (sem_init(mutex, 1, sem_num) < 0) {
		std::perror("Semaphore initialization");
		exit(0);
	}
	// forked child processes use the same semaphore

	std::vector<int> keys(keys_number); // vector with keys_number ints.
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
				sem_wait(mutex);
				int key = distribution(generator);
				std::cout << "Searching for key " << key << "..." << std::endl;
				if (tree4.exist(key))
					std::cout << "Key " << key << " is found!" << std::endl;
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 2));
				sem_post(mutex);
			}
			exit(0);
		}
	}

	// The parent process waits for the semaphore capture on par with everyone.
	std::random_shuffle(std::begin(keys), std::end(keys)); // the second shuffle
	std::for_each(keys.begin(), keys.end(), [&](int key) { // remove
				sem_wait(mutex);
				tree4.remove(key);
				std::this_thread::sleep_for(
						std::chrono::milliseconds(distribution(generator) / 16));
				sem_post(mutex);
			});

	// this way, the father waits for all the child processes
	while ((wpid = wait(&status)) > 0)
		;

	sem_close(mutex);
	shm_unlink("/myshm");
	return 0;
}
