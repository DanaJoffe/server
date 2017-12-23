/*
 * StartGame.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef STARTGAME_H_
#define STARTGAME_H_

#include "Command.h"

#include <unistd.h>

class StartGame: public Command {
public:
  StartGame() {};
  virtual void execute(vector<string>& args, map<string, vector<int> >& games,
                     int client_socket);
  virtual ~StartGame() {};
};

#endif /* STARTGAME_H_ */
