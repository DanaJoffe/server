/*
 * CommandManager.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef COMMANDMANAGER_H_
#define COMMANDMANAGER_H_

#include "Command.h"
#include "CloseGame.h"
#include "JoinGame.h"
#include "ListGames.h"
#include "PlayTurn.h"
#include "StartGame.h"

#include <iostream>
#include <map>
#include <vector>
using namespace std;

class CommandManager {
public:
  CommandManager();
  void executeCommand(string command, vector<string>& args, int client_socket);
  ~CommandManager();

private:
  map<string, Command*> commands_;
};

#endif /* COMMANDMANAGER_H_ */
