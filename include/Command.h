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
  /*
   * exectute command from client, with command args.
   * input: vector<string>& args, int client_socket
   */
  virtual void execute(vector<string>& args, int client_socket) = 0;
  /*
   * destructor - no need to delete anything.
   */
  virtual ~Command() {};
};

#endif /* COMMAND_H_ */
