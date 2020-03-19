/**
 * Copyright 2020, Benjamin Adams.
 * Email: adams.benjamin@protonmail.com
 *
 * Description:
 * Quick showcase of the PThread class abstraction.
 */

#include <pthread.h>
#include <iostream>
#include <vector>
#include "pthread.hpp"


#define NUM_THREADS 5
#define MAX_COUNT 100


using namespace std;


template<typename T>
void print(vector<T> * v) {
	cout << '[';
	for (auto t : *v) {
		cout << t;
		if (!(t == v->back())) {
			cout << ", ";
		}
	}
	cout << ']';
}

/**
 * Given the address of a shared int,
 * work with sibling threads to increment
 * the value until a given threshold is reached.
 * Each thread will keep a record of it's personal
 * updates and return a reference to it upon exit.
 */
vector<int> * incrementor(int * x)
{
	static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	auto records = new vector<int>();

	while (*x < MAX_COUNT) {
		if (pthread_mutex_trylock(&mtx)) {
			(*x) += 1;
			records->push_back(*x);
			pthread_mutex_unlock(&mtx);
		}
	}

	return records;
}

/* Initialize the threads. */
void init_threads(vector<PThread<int, vector<int>> *> * pts) {
	for (int i = 0; i < NUM_THREADS; i++) {
		pts->push_back(
			new PThread<int, vector<int>>(&incrementor)
		);
	}
}

/* Begin thread execution. */
void start_threads(vector<PThread<int, vector<int>> *> * pts) {
	cout << "Created " << pts->size() << " threads.\n";
	for (auto pt : *pts) {
		static auto x = 0;
		pt->start(&x);
	}

}

/* Join the child threads and display the results. */
void join_results(vector<PThread<int, vector<int>> *> * pts) {
	for (auto pt : *pts) {
		cout << pt->id() << " incremented to values:\n";
		auto records = pt->done();
		print(records);
		cout << '\n';
	}
}

int main()
{
	vector<PThread<int, vector<int>> *> pts;

	init_threads(&pts);
	start_threads(&pts);
	join_results(&pts);

	return 0;
}

