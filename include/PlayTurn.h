/*
 * PlayTurn.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef PLAYTURN_H_
#define PLAYTURN_H_

#include "Command.h"

#include <sstream>
#include <unistd.h>

class PlayTurn: public Command {
public:
  PlayTurn() {};
  virtual void execute(vector<string>& args, int client_socket);
  virtual ~PlayTurn() {};
};

#endif /* PLAYTURN_H_ */
