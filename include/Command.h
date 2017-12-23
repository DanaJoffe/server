/*
 * Command.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Command {
public:
//  Command() {};
  virtual void execute(vector<string>& args, map<string, vector<int> >& games,
                    int client_socket) = 0;
  virtual ~Command() {};
};

#endif /* COMMAND_H_ */
