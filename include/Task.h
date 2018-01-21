/*
 * Task.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef TASK_H_
#define TASK_H_

/*
 * Task that can be executed.
 */
class Task {
public:
  /*
   * constuct task with function and arguments
   */
  Task(void * (*func)(void *arg), void* arg);
  /*
   * execute task - call function with arguments
   */
  void execute();
  /*
   * destruct task
   */
  ~Task() {};
private:
  void* (*func)(void*arg);
  void* arg;
};

#endif /* TASK_H_ */
