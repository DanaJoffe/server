/*
 * CloseGame.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "CloseGame.h"

void CloseGame::execute(vector<string>& args, map<string, vector<int> >& games,
    int client_socket) {
  map<string, vector<int> >::iterator it;
  for (it = games.begin(); it != games.end(); it++) {
    if (it->second.size() == 2) {
      if (it->second[0] == client_socket || it->second[1] == client_socket) {
        close(it->second[0]);
        close(it->second[1]);
        games.erase(it);
        break;
      }
    }
  }
}

/*  SOME CODE I SAVED FOR CloseGame FUNC
 * 	// Close communication with the client
	close(clientSocket1);
	close(clientSocket2);
	return NULL;
 */
