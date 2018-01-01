/*
 * CommandManager.cpp
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#include "CommandManager.h"

CommandManager::CommandManager() {
  commands_["start"] = new StartGame();
  commands_["list_games"] = new ListGames();
  commands_["join"] = new JoinGame();
}

void CommandManager::executeCommand(string command, vector<string>& args,
                                  int client_socket) {
  Command *cmd = commands_[command];
  cmd->execute(args, client_socket);
}

CommandManager::~CommandManager() {
  map<string, Command*>::iterator it;
  for (it = commands_.begin(); it != commands_.end(); it++) {
    delete it->second;
  }
}
