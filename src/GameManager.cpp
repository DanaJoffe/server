/*
 * GameManager.cpp
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */

#include <GameManager.h>

// Singleton version
GameManager* GameManager::instance = 0;
GameManager* GameManager::getInstance() {
	if(!instance) {
		instance = new GameManager();
		cout << "getInstance(): First instance\n";
		return instance;
	} else {
		cout << "getInstance(): previous instance\n";
		return instance;
	}
}

bool GameManager::isGameExist(string gameName) {
	 //find game
	map<string, vector<int> >::iterator it;
	it = this->games->find(gameName);
	if (it == this->games->end()) {
		return false;
	}
	return true;
}

// returns -1 if game doesn't exist.
int GameManager::playersAmount(string gameName) {
	if (!this->isGameExist(gameName)) {
		return -1;
	}
	 map<string, vector<int> >::iterator it;
	 it = this->games->find(gameName);
	 return it->second.size();
}


// returns 1 if adding done successfully.
// returns -1 if adding didn't work..
// returns -2 if game already have 2 clients.
int GameManager::addGame(string gameName, int playerSocket) {
	if (!this->isGameExist(gameName)) {
		vector<int> players_sockets;
		players_sockets.push_back(playerSocket);
		pair<map<string, vector<int> >::iterator,bool> result;

		result = this->games->insert(make_pair(gameName, players_sockets));
		if (result.second == false) {
		   return -1;
		}
		return 1;
	}
	if (this->playersAmount(gameName) == 2) {
		return -2;
	}
	//game has 1 player - add the second player
	map<string, vector<int> >::iterator it;
	it = this->games->find(gameName);
	it->second.push_back(playerSocket);
	return 1;
}

map<string, vector<int> >* GameManager::getGames() {
	return this->games;
}


vector<int>& GameManager::getPlayers(string gameName) {
	if (!this->isGameExist(gameName)) {
		throw "Error in GameManager::getPlayers. game does not exist";
	}
	map<string, vector<int> >::iterator it;
	it = this->games->find(gameName);
	return it->second;
}



void GameManager::RunGame(string& gameName) {
	cout << "GameManager::RunGame" << endl;

	vector<int> clients = this->getPlayers(gameName);
	if (clients.size() != 2) {
		throw "Error in GameManager::RunGame. player is missing";
	}
	int clientSocket1 = clients[0];
	int clientSocket2 = clients[1];


//	while (isGameOnList(gameName)) {
	while (true) {

		cout << "GameManager::RunGame: first player" << endl;
		//handle first client
		bool isClient1Connected = handleOneClient(clientSocket1, clientSocket2, gameName, *this->getGames()); //return msg
		if (!isClient1Connected) {
			return;
		}

		cout << "GameManager::RunGame: second player" << endl;
		//handle second client
		bool isClient2Connected = handleOneClient(clientSocket2, clientSocket1, gameName, *this->getGames());
		if (!isClient2Connected) {
			return;
		}
	}
	return;
}

void GameManager::closeGames() {
	map<string, vector<int> >::iterator it;
	int player1Socket;
	int player2Socket;

	for (it = this->games->begin(); it!=this->games->end(); ++it) {
		player1Socket = it->second[0];
		player2Socket = it->second[1];
		// inform players game is closed.
//******************************************

		// close players' sockets.
		close(player1Socket);
		close(player2Socket);
	}
	delete this->games;
}


//////// finish singleton section

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

