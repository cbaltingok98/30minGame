#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include "windows.h"
#include <thread>
#include <chrono>

using namespace std;

class Game {
private:
	vector<string> map;
	ifstream file;
	string mystr;

	bool alienAlive = true;
	bool playerAlive = true;
	bool bulletAlive = false;
	bool gameGame = false;

	char alienSymbol = 'A';
	char wallSymbol = '#';
	char obstacleSymbol = 'I';
	char playerSymbol = '^';
	char emptySpace = ' ';
	char bulletSymbol = '!';

	int alienX = 1;
	int alienY = 1;
	int alienDirection = 1;
	int playerX = 12;
	int playerY = 6;
	int playerDirectionY = 1;
	int playerDirectionX = 1;
	int alienEndX = 25;
	int alienEndY = 7;
	int alienMoveCount = 0;
	int bulletY;
	int bulletX;

public:
	void uploadFile();
	void updateGame();
	void gameMove(bool , int, int, int, bool &bulletAlive);
	bool checkIfEnd();
	bool checkIfAlive();
	void initializeObj();
	void setBullet();
};


void Game::uploadFile() {

	file.open("map.txt");

	if (file) {
		while (getline(file, mystr)) {
			map.push_back(mystr);
		}
	}
	else {
		cout << "Error Opening File!" << endl;
	}
	file.close();
}

void Game::updateGame() {
	for (int i = 0; i < map.size(); i++) {
		cout << map.at(i) << endl;
	}
}

void Game::gameMove(bool playerMove, int direcY, int direcX, int bullDirecY, bool &bulletAlive) {

	if (!gameGame) {
		if (alienAlive) {
			if (map.at(alienY).at(alienX + alienDirection) == emptySpace) {
				map.at(alienY).at(alienX) = emptySpace;
				alienX += alienDirection;
				map.at(alienY).at(alienX) = alienSymbol;
				alienMoveCount++;
			}
			else if (map.at(alienY).at(alienX + alienDirection) == wallSymbol) {
				alienDirection *= -1;
				map.at(alienY).at(alienX) = emptySpace;
				map.at(++alienY).at(alienX) = alienSymbol;
				alienMoveCount++;
			}
			else if (map.at(alienY).at(alienX + alienDirection) == playerSymbol) {
				gameGame = true;
			}
			else if (map.at(alienY).at(alienX + alienDirection) == bulletSymbol) {
				alienAlive = false;
			}
		}

		if (playerMove && !(alienMoveCount % 4)) {
			playerDirectionY = direcY;
			playerDirectionX = direcX;
			if (map.at(playerY + playerDirectionY).at(playerX + playerDirectionX) == emptySpace) {
				map.at(playerY).at(playerX) = emptySpace;
				playerY += playerDirectionY;
				playerX += playerDirectionX;
				map.at(playerY).at(playerX) = playerSymbol;
				playerMove = false;
				direcY = 0;
				direcX = 0;
			}
		}
		if (bulletAlive && !(alienMoveCount % 2)) {

			map.at(bulletY).at(bulletX) = bulletSymbol;

			if (map.at(bulletY - 1).at(bulletX) == wallSymbol) {
				map.at(bulletY).at(bulletX) = emptySpace;
				bulletAlive = false;
			}
			else if (map.at(bulletY - 1).at(bulletX) == emptySpace) {				
				map.at(bulletY).at(bulletX) = emptySpace;
				bulletY += bullDirecY;
				map.at(bulletY).at(bulletX) = bulletSymbol;
			}
			else if (map.at(bulletY - 1).at(bulletX) == alienSymbol) {
				map.at(alienY).at(alienX) = emptySpace;
				map.at(bulletY).at(bulletX) = emptySpace;
				bulletAlive = false;
				playerAlive = false;
			}
		}
	}
}

bool Game::checkIfAlive() {
	return playerAlive && alienAlive;
}

bool Game::checkIfEnd() {
	bool work = false;
	map.at(alienEndY).at(alienEndX) == alienSymbol || gameGame ? work = true : work = false;
	return work;
}

void Game::initializeObj() {
	map.at(alienY).at(alienX) = alienSymbol;
	map.at(playerY).at(playerX) = playerSymbol;
}

void Game::setBullet() {
	bulletX = playerX;
	bulletY = playerY - 1;
}

int main() {

	Game controlGame;

	bool work = true;
	bool playerMove = false;
	bool bulletMove = false;

	int direcY = 0;
	int direcX = 0;
	int bulletDirecY = 0;

	controlGame.uploadFile();
	controlGame.initializeObj();

	do {
		if (controlGame.checkIfEnd()) {
			cout << "End Game" << endl;
			break;
		}
  		else if(controlGame.checkIfAlive()) {
			system("CLS");

			if (GetAsyncKeyState(VK_LEFT)) {
				direcY = 0;
				direcX = -1;
				playerMove = true;
			}
			if (GetAsyncKeyState(VK_RIGHT)) {
				direcY = 0;
				direcX = 1;
				playerMove = true;
			}
			if (GetAsyncKeyState(VK_UP)) {
				direcY = -1;
				direcX = 0;
				playerMove = true;
			}
			if (GetAsyncKeyState(VK_DOWN)) {
				direcY = 1;
				direcX = 0;
				playerMove = true;
			}
			if (GetAsyncKeyState(VK_SPACE)) {
				controlGame.setBullet();
				bulletDirecY = -1;
				bulletMove = true;
			}
			
			controlGame.gameMove(playerMove, direcY, direcX, bulletDirecY, bulletMove);
			controlGame.updateGame();
			this_thread::sleep_for(chrono::milliseconds(1));
		}
	} while (work);


	return 0;
}
