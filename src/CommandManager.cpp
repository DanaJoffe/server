/*
 * CommandManager.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "CommandManager.h"

CommandManager::CommandManager() {
  commands_["start"] = new StartGame();
  commands_["list_games"] = new ListGames();
  commands_["join"] = new JoinGame();
}

void CommandManager::executeCommand(string command, vector<string>& args, int client_socket) {
  Command *cmd = commands_[command];
  cmd->execute(args, client_socket);
}

CommandManager::~CommandManager() {
  map<string, Command*>::iterator it;
  for (it = commands_.begin(); it != commands_.end(); it++) {
    delete it->second;
  }
}
