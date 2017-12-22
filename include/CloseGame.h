/*
 * CloseGame.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef CLOSEGAME_H_
#define CLOSEGAME_H_

#include "Command.h"

#include <unistd.h>

class CloseGame: public Command {
public:
  CloseGame() {};
  virtual void execute(vector<string>& args, map<string, vector<int> >& games,
                        int client_socket);
  virtual ~CloseGame() {};
};

#endif /* CLOSE_H_ */
