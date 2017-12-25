/*
 * JoinGame.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef JOINGAME_H_
#define JOINGAME_H_

#include "Command.h"
#include "threads.h"
#include "GameManager.h"


#include <unistd.h>

class JoinGame: public Command {
public:
  JoinGame() {};
  virtual void execute(vector<string>& args, map<string, vector<int> >& games,
                     int client_socket);
  virtual ~JoinGame() {};
};

#endif /* JOINGAME_H_ */
