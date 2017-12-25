/*
 * GameManager.cpp
 *
 *  Created on: Dec 25, 2017
 *      Author: djoffe
 */

#include <GameManager.h>


void GameManager::RunGame(int clientSocket1 , int clientSocket2, string& gameName, map<string, vector<int> >& games) {
//	struct ThreadClientArgs *args = (struct ThreadClientArgs *)clientArgs;
//	int clientSocket1 = args->clientSocketFirst;
//	int clientSocket2 = args->clientSocketSecond;
//	map<string, vector<int> > games = *(args->games);
//	string gameName = args->gameName;


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

bool GameManager::handleOneClient(int clientSocket, int waitingclient, string& gameName, map<string, vector<int> >& games) {
	int size, n;

	// Read length
	n = read(clientSocket, &size, sizeof(size));
	if (n == -1) {
		cout << "Error reading size" << endl;
		return false;
	}
	if (n == 0) {
		cout << "Client disconnected" << endl;
		return false;
	}

	// Read message
	string move = readStringFromSocket(size, clientSocket);

	//return false if other client disconnected
//	if (is_client_closed(findOtherPlayer(games,gameName, clientSocket))) {
	if (is_client_closed(waitingclient)) {
		cout << "Client disconnected" << endl;
	    return false;
	}

	// Interpret
	string buf; // Have a buffer string
	stringstream msg(move); // Insert the string into a stream
	msg >> buf;
	string comName = buf;
	vector<string> args;

	//add game's name to args
//	args.push_back(gameName);

	//add all arguments
	while (msg >> buf)
		args.push_back(buf);

//	CommandManager comManager;
//	comManager.executeCommand(comName, args, games, clientSocket);

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


//NEED TO WRITE!!!
int GameManager::findOtherPlayer(map<string, vector<int> >& games, string& gameName, int clientSocket) {
	return 0;
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



Status GameManager::PlayDecoder(string message, int* row, int* col) {
	string buf; // Have a buffer string
	stringstream msg(message); // Insert the string into a stream
	vector<string> tokens; // Create vector to hold our words

	while (msg >> buf)
		   tokens.push_back(buf);

	int num1 = atoi(tokens[1].c_str());
	int num2 = atoi(tokens[2].c_str());
	*row = num1;
	*col = num2;

	if(num1 == -1 && num2 == -1) {
		return NO_MOVES;
	} else if(num1 == -2 && num2 == -2) {
		return END;
	} else {
		return HAS_MOVE;
	}
}

//NEED TO WRITE
bool GameManager::isGameOnList(string& comgameName) {
	return true;
}

