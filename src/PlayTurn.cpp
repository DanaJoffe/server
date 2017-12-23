/*
 * PlayTurn.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: chaviva
 */

#include "PlayTurn.h"

void PlayTurn::execute(vector<string>& args, map<string, vector<int> >& games,
    int client_socket) {
  int other_player;
  //find other player in client's game
  map<string, vector<int> >::iterator it;
  for (it = games.begin(); it != games.end(); it++) {
    if (it->second.size() == 2) {
      if (it->second[0] == client_socket) {
        other_player = it->second[1];
        break;
      } else if (it->second[1] == client_socket) {
        other_player = it->second[0];
        break;
      }
    }
  }
  //send play coordinates to other player
  for (int i = 0; i < args.size(); i++) {
    int c;
    stringstream coordinate(args[i]);
    coordinate >> c;
    int n = write(other_player, &c, sizeof(c));
    if (n == -1) {
      cout << "Error writing coordinate to socket" << endl;
    }
    coordinate.clear();
  }
}
