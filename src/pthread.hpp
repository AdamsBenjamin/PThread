#ifndef SRC_PTHREAD
/**
 * Copyright 2020, Benjamin Adams.
 * Email: adams.benjamin@protonmail.com
 *
 * Description:
 * A templated, struct-based abstraction of the pthread library.
 * Allows for the use of pthread in a more
 * object-oriented fashion by exposing basic functionality
 * previously provided by the underlying pthread library.
 */

#include <pthread.h>
#include <iostream>

namespace std {

/* A class-like abstraction of the pthread library. */
template<typename U, typename T>
struct PThread {
	public:
		PThread<U, T>(T * (*)(U *));
		~PThread<U, T>();
		void start(U *);
		void stop();
		T * done();
		pthread_t id();

	private:
		pthread_t thread_id;
		T * (*callable)(U *);
		pthread_attr_t attrs;
};

template<typename U, typename T>
PThread<U, T>::PThread(T * (*f)(U *)) {
	this->callable = f;
	pthread_attr_init(&this->attrs);
}

template<typename U, typename T>
PThread<U, T>::~PThread() {
	pthread_attr_destroy(&this->attrs);
}

/* Begin the execution of the thread. */
template<typename U, typename T>
void PThread<U, T>::start(U * arg) {
	// Using a static reference here allows the
	// function to be directly referenced in the lambda
	// function below. This circumvents the need to
	// capture `this`, which would otherwise result
	// in the creation of a closure, which is incompatible
	// with the `pthread_create` function.
	static auto s_func = this->callable;

	// The lambda here is ugly by choice
	// so as to allow for users to remain blind to
	// the underlying use of void pointers. This allows
	// for more immediately type-checkable usage.
	pthread_create(
		&this->thread_id,
		&this->attrs,
		[](void * arg) -> void * {
			auto ret = s_func(reinterpret_cast<U *>(arg));
			pthread_exit(reinterpret_cast<void *>(ret));
		},
		reinterpret_cast<void *>(arg)
	);
	cout << "Started thread: " << this->id() << '\n';

	return;
}

/* Stop the execution of the thread. */
template<typename U, typename T>
void PThread<U, T>::stop() {
	pthread_cancel(this->id());
	return;
}

/**
 * Return the result of executing the thread.
 * Null reference will be returned in cases
 * where nothing returns.
 * Blocks the calling thread until results
 * are rendered.
 */
template<typename U, typename T>
T * PThread<U, T>::done() {
	T * tmp;
	pthread_join(this->id(), reinterpret_cast<void **>(&tmp));
	return tmp;
}

/**
 * Return a thread's unique id.
 * Undefined behavior in cases where
 * called before `start`.
 */
template<typename U, typename T>
pthread_t PThread<U, T>::id() {
	return this->thread_id;
}


} // namespace std
#endif

