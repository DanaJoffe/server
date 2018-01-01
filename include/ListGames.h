/*
 * ListGames.h
 *
 * Author1: name & ID: Dana Joffe 312129240
 * Author2: name & ID: Chaviva Moshavi 322082892
 */

#ifndef LISTGAMES_H_
#define LISTGAMES_H_

#include "Command.h"
#include "threads.h"

#include <sstream>
#include <string>
#include <unistd.h>

class ListGames: public Command {
public:
  /*
   * constructor
   */
  ListGames() {};
  /*
   * send client a list of all existing games with one only one player.
   * close client's socket when finished.
   * input: args - none, client_socket - client to send list to.
   */
  virtual void execute(vector<string>& args, int client_socket);
  /*
   * destructor
   */
  virtual ~ListGames() {};
};

#endif /* LISTGAMES_H_ */
