/*
 * CommandManager.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "Command.h"
#include "JoinGame.h"
#include "ListGames.h"
#include "StartGame.h"

#include <iostream>
#include <map>
#include <vector>
using namespace std;

class CommandManager {
public:
  /*
   * constructor
   */
  CommandManager();
  /*
   * execute command from client, with command args
   */
  void executeCommand(string command, vector<string>& args, int client_socket);
  /*
   * destructor - delete command map data member.
   */
  ~CommandManager();

private:
  map<string, Command*> commands_;
};

#endif /* COMMANDMANAGER_H_ */
