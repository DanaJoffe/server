/*
 * JoinGame.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef JOINGAME_H_
#define JOINGAME_H_

#include "Command.h"
#include "threads.h"
#include "GameManager.h"


#include <unistd.h>

class JoinGame: public Command {
public:
  /*
   * constructor
   */
  JoinGame() {};
  /*
   * add player to requested game.
   * send player 1 if succeeded in adding him to game. else send -1.
   * send both players of game their colors, and run the game.
   * if didn't succeed - close client's socket
   * input: args - name of game, client_socket - player to add.
   */
  virtual void execute(vector<string>& args, int client_socket);
  /*
   * destructor
   */
  virtual ~JoinGame() {};
};

#endif /* JOINGAME_H_ */
