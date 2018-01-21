/*
 * Task.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "Task.h"

Task::Task(void* (*func)(void*arg), void* arg) : func(func), arg(arg) {}

void Task::execute() {
  func(arg);
}

