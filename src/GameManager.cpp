/*
 * GameManager.cpp
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */

#include <GameManager.h>


void GameManager::RunGame(int clientSocket1 , int clientSocket2, string& gameName, map<string, vector<int> >& games) {
//	while (isGameOnList(gameName)) {
	while (true) {
		//handle first client
		bool isClient1Connected = handleOneClient(clientSocket1, clientSocket2, gameName, games); //return msg
		if (!isClient1Connected) {
			return;
		}
		//handle second client
		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket1, gameName, games);
		if (!isClient2Connected) {
			return;
		}
	}
	return;
}

bool GameManager::handleOneClient(int clientSocket, int waitingClient, string& gameName, map<string, vector<int> >& games) {
	int size, n;
	// recieve command and arguments
	string commandName;
	vector<string> args;
	bool b = readCommand(clientSocket, &commandName, &args);
	if (b == false)
		throw "Error reading from socket";

	if (strcmp(commandName.c_str(), "close") == 0) {
		closeGame(gameName, games, waitingClient);
		return false;
	} else if (strcmp(commandName.c_str(), "play") == 0) {
		bool b = playTurn(args, waitingClient);
		if (b == false)
			return false;
	}
	return true;
}


/*
 * 	// writing message size
	n = write(waitingClient, &size, sizeof(size));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return false;
	}
	//return false if other client disconnected
	if (is_client_closed(waitingClient)) {
	    cout << "Client disconnected" << endl;
	    return false;
	}
	//writing message
	n = write(waitingClient, move.c_str(), strlen(move.c_str()));
	if (n == -1) {
		cout << "Error writing move to socket" << endl;
		return false;
	}
 */

void GameManager::closeGame(string& gameName, map<string, vector<int> >& games,  int waitingClient) {
	int row,col;
	row = col = -2;
	if (is_client_closed(waitingClient)) {
		cout << "Client disconnected" << endl;
		return;
	}
	// writing row to waitingClient socket
	int n = write(waitingClient, &row, sizeof(row));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return;
	}
	if (is_client_closed(waitingClient)) {
		cout << "Client disconnected" << endl;
		return;
	}
	// writing col to waitingClient socket
	n = write(waitingClient, &col, sizeof(col));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return;
	}

	 map<string, vector<int> >::iterator iter;
	 iter = games.find(gameName);
	 vector<int> clients = iter->second;
	 int firstClient = clients[0];
	 int secondClient = clients[1];

	 // Close communication with the client
	 close(firstClient);
	 close(secondClient);

	 //delete gameName from games
	 games.erase (gameName);
}

bool GameManager::playTurn(vector<string>& args, int waitingClient) {
	int row = atoi(args[0].c_str());
	int col = atoi(args[1].c_str());

	if (is_client_closed(waitingClient)) {
		cout << "Client disconnected" << endl;
		return false;
	}
	// writing row to waitingClient socket
	int n = write(waitingClient, &row, sizeof(row));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return false;
	}
	if (is_client_closed(waitingClient)) {
		cout << "Client disconnected" << endl;
		return false;
	}
	// writing col to waitingClient socket
	n = write(waitingClient, &col, sizeof(col));
	if (n == -1) {
		cout << "Error writing size to socket" << endl;
		return false;
	}
	return true;
}


bool GameManager::is_client_closed(int cs) {
	pollfd pfd;
	pfd.fd = cs;
	pfd.events = POLLIN | POLLHUP | POLLRDNORM;
	pfd.revents = 0;

	// call poll with a timeout of 100 ms
	if(poll(&pfd, 1, 100) > 0) {
		// if result > 0, this means that there is either data available on the
		// socket, or the socket has been closed
		char buffer[32];
		if(recv(cs, buffer, sizeof(buffer), MSG_PEEK | MSG_DONTWAIT) == 0) {
			// if recv returns zero, that means the connection has been closed:
			return true;
		}
	}
	return false;
}



//NEED TO WRITE
bool GameManager::isGameOnList(string& comgameName) {
	return true;
}

