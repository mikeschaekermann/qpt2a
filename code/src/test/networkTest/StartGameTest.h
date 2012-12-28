#include <cassert>
#include <iostream>
#include <string>

#include "../../common/network/messages/game/outgame/StartGame.h"
using namespace std;

void testStartGame()
{
	cout << "StartGame Test[Start]" << endl;

	StartGame *message = new StartGame();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;

	message->worldRadius = 532.1f;

	for (unsigned i = 0; i < 3; ++i)
	{
		Player p;
		p.playerId = i;

		for (unsigned j = 0; j <  1 + i * 2; ++j)
		{
			p.playerName = p.playerName.append("ABC");
		}

		p.startCellId = i;

		p.startPosition = ci::Vec3f(3.33f, 6.66f, 1.f * i);
		message->players.push_back(p);
	}


	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	StartGame *received = new StartGame(data, index);

	cout << "StartGame Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "StartGame Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "StartGame Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);
	cout << "StartGame Test[Assert 4]" << endl;
	assert(message->players.size() == received->players.size());
	cout << "StartGame Test[Assert 5]" << endl;
	for (unsigned i = 0; i < message->players.size(); ++i)
	{
		assert(message->players[i].playerId == received->players[i].playerId);
	}

	cout << "StartGame Test[Assert 6]" << endl;
	for (unsigned i = 0; i < message->players.size(); ++i)
	{
		assert(message->players[i].startPosition == received->players[i].startPosition);
	}

	cout << "StartGame Test[Assert 7]" << endl;
	assert(message->worldRadius == received->worldRadius);

	cout << "StartGame Test[Assert 8]" << endl;
	for (unsigned i = 0; i < message->players.size(); ++i)
	{
		assert(message->players[i].startCellId == received->players[i].startCellId);
	}

	cout << "StartGame Test[Assert 9]" << endl;
	for (unsigned i = 0; i < message->players.size(); ++i)
	{
		assert(message->players[i].playerName == received->players[i].playerName);
	}

	delete message;
	delete received;
	delete data;
	cout << "StartGame Test[End]" << endl;
}