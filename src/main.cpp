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


vector<int> * incrementor(int * x)
{
	static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
	auto records = new vector<int>();

	while (*x < MAX_COUNT) {
		if (pthread_mutex_trylock(&mtx)) {
			(*x)++;
			records->push_back(*x);
			pthread_mutex_unlock(&mtx);
		}
	}

	return records;
}

int main()
{
	vector<PThread<int, vector<int>> *> pts;
	for (int i = 0; i < NUM_THREADS; i++) {
		pts.push_back(
			new PThread<int, vector<int>>(&incrementor)
		);
	}

	cout << "Created " << pts.size() << " threads.\n";
	for (auto pt : pts) {
		static auto x = 0;
		pt->start(&x);
	}

	for (auto pt : pts) {
		cout << pt->id() << " incremented to values:\n [";
		auto records = pt->done();
		for (auto record : *records) {
			cout << record;
			if (!(record == records->back())) {
				cout << ", ";
			}
		}

		cout << "]\n";
	}

	return 0;
}

