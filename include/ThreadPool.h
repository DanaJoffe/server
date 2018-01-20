/*
 * ThreadPool.h
 *
 *  Created on: Jan 18, 2018
 *      Author: chaviva
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Task.h"
#include <queue>
#include <pthread.h>

class ThreadPool {
public:
  ThreadPool(int threadsNum);
  void addTask(Task *task);
  void terminate();
  virtual ~ThreadPool();

private:
  queue<Task *> tasksQueue;
  pthread_t* threads;
  void executeTasks();
  bool stopped;
  pthread_mutex_t lock;
  static void *execute(void* arg);
};

#endif /* THREADPOOL_H_ */
