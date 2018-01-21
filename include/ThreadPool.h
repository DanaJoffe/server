/*
 * ThreadPool.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include "Task.h"
#include <queue>
#include <pthread.h>

using namespace std;

/*
 * pool of threads that execute tasks.
 */
class ThreadPool {
public:
  /*
   * construct thread pool with given number of threads
   */
  ThreadPool(int threadsNum);
  /*
   * add task to task queue
   */
  void addTask(Task *task);
  /*
   * termonate thread pool, stop executing tasks
   */
  void terminate();
  /*
   * destructor
   */
  virtual ~ThreadPool();

private:
  /*
   * call execute tasks
   */
  static void *execute(void* arg);
  /*
   * every 1 second, execute current front task in queue, till it's told to stop
   */
  void executeTasks();

  //data members
  queue<Task *> tasksQueue;
  pthread_t* threads;
  bool stopped;
  pthread_mutex_t lock;
};

#endif /* THREADPOOL_H_ */
