/*
 * GameManager.h
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */

#ifndef INCLUDE_GAMEMANAGER_H_
#define INCLUDE_GAMEMANAGER_H_

#include "threads.h"

class GameManager {
public:
  /*
   * constructs.
   */
	GameManager();
	bool is_client_closed(int cs);

	Status PlayDecoder(string message, int* row, int* col);
	int findOtherPlayer(map<string, vector<int> >& games,string& gameName, int clientSocket);
	bool isGameOnList(string& comgameName);

	void RunGame(int clientSocket1 , int clientSocket2, string& gameName, map<string, vector<int> >& games);
	bool handleOneClient(int clientSocket, int waitingclient, string& gameName, map<string, vector<int> >& games);

private:

};



#endif /* INCLUDE_GAMEMANAGER_H_ */
