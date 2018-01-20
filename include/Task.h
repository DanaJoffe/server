/*
 * Task.h
 *
 *  Created on: Jan 18, 2018
 *      Author: chaviva
 */

#ifndef TASK_H_
#define TASK_H_

class Task {
public:
  Task(void * (*func)(void *arg), void* arg);
  void execute();
  ~Task() {};
private:
  void* (*func)(void*arg);
  void* arg;
};

#endif /* TASK_H_ */
