/*
 * ListGames.h
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#ifndef LISTGAMES_H_
#define LISTGAMES_H_

#include "Command.h"

#include <sstream>
#include <string>
#include <unistd.h>

class ListGames: public Command {
public:
  ListGames() {};
  virtual void execute(vector<string>& args, map<string, vector<int> >& games,
                     int client_socket);
  virtual ~ListGames() {};
};

#endif /* LISTGAMES_H_ */
