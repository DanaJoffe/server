/*
 * ThreadPool.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: chaviva
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
}

void ThreadPool::addTask(Task *task) {
  tasksQueue.push(task);
}
