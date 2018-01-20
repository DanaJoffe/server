/*
 * Task.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: chaviva
 */

#include "Task.h"

Task::Task(void* (*func)(void*arg), void* arg) : func(func), arg(arg) {}

void Task::execute() {
  func(arg);
}

