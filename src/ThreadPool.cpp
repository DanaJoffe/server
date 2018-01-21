/*
 * ThreadPool.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "ThreadPool.h"
#include <unistd.h>

ThreadPool::ThreadPool(int threadsNum) : stopped(false) {
  threads = new pthread_t[threadsNum];
  for(int i = 0; i < threadsNum; i++) {
    pthread_create(threads + i, NULL, execute, this);
  }
  pthread_mutex_init (&lock, NULL);
}

void* ThreadPool::execute(void* arg) {
  ThreadPool* pool = (ThreadPool*)arg;
  pool->executeTasks();
  return NULL;
}

void ThreadPool::addTask(Task *task) {
  tasksQueue.push(task);
}

void ThreadPool::executeTasks() {
	while (!stopped) {
		pthread_mutex_lock(&lock);
		if (!tasksQueue.empty()) {
			Task* task = tasksQueue.front();
			tasksQueue.pop();
			pthread_mutex_unlock(&lock);
			task->execute();
      delete task;
		} else {
			pthread_mutex_unlock(&lock);
			sleep(1);
		}
	}
}

void ThreadPool::terminate() {
	stopped = true;
}
ThreadPool::~ThreadPool() {
  pthread_mutex_lock(&lock);
  while (!tasksQueue.empty()) {
    Task* task = tasksQueue.front();
    tasksQueue.pop();
    delete task;
  }
  pthread_mutex_unlock(&lock);
  pthread_mutex_destroy(&lock);
	delete[] threads;
}


