#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/outgame/GameOver.h"

using namespace std;

void testGameOver()
{
	cout << "GameOver Test[Start]" << endl;

	GameOver *message = new GameOver();
	message->messageId = 32;
	message->messageType = MessageType::GameOver;
	message->playerId = 5;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	GameOver *received = new GameOver(data, index);

	cout << "GameOver Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "GameOver Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "GameOver Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);
	cout << "GameOver Test[Assert 4]" << endl;
	assert(message->playerId == received->playerId);

	delete message;
	delete received;
	delete data;
	cout << "GameOver Test[End]" << endl;
}