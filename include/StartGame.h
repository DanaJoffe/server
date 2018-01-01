/*
 * StartGame.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef STARTGAME_H_
#define STARTGAME_H_

#include "Command.h"
#include "threads.h"

#include <unistd.h>

class StartGame: public Command {
public:
  /*
   * constructor
   */
  StartGame() {};
  /*
   * add new game to list of games with client as first player.
   * send client 1 if succeeded, else send -1
   * if didn't succeed - close client's socket
   * input: args - name of game to add, client_socket - client to add as player
   */
  virtual void execute(vector<string>& args, int client_socket);
  /*
   * destuctor
   */
  virtual ~StartGame() {};
};

#endif /* STARTGAME_H_ */
