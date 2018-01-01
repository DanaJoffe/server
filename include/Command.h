/*
 * Command.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
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
