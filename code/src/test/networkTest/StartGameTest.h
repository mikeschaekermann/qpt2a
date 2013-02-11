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
		NetworkPlayer p;
		p.playerId = i;

		for (unsigned j = 0; j <  1 + i * 2; ++j)
		{
			p.playerName = p.playerName.append("ABC");
		}

		p.startCellId = i;

		p.startPosition = ci::Vec3f(3.33f, 6.66f, 1.f * i);
		message->players.push_back(p);
	}

	for (unsigned i = 0; i < 3; ++i)
	{
		NetworkBarrier p;
		p.modifierId = i;
		p.position = ci::Vec3f(4.33f, 5.66f, 2.f * i);
		p.rotation = ci::Vec3f(5.33f, 6.66f, 3.f * i);
		p.scale = ci::Vec3f(6.33f, 7.66f, 4.f * i);
		p.radius = i * 2.4f;
		message->barriers.push_back(p);
	}

	for (unsigned i = 0; i < 3; ++i)
	{
		NetworkDynamicModifier p;
		p.modifierId = i;
		p.position = ci::Vec3f(9.33f, 15.66f, 232.f * i);
		p.rotation = ci::Vec3f(523.33f, 2346.66f, 3.f * i);
		p.scale = ci::Vec3f(6.33f, 7.23466f, 4.f * i);
		p.radius = i * 2.4f;
		p.type = DynamicModifierType::VirusSwarm;
		message->dynamicModifiers.push_back(p);
	}

	for (unsigned i = 0; i < 3; ++i)
	{
		NetworkStaticModifier p;
		p.modifierId = i;
		p.position = ci::Vec3f(4.33f, 5.66f, 2234.f * i);
		p.rotation = ci::Vec3f(5234.33f, 6.23466f, 3.f * i);
		p.scale = ci::Vec3f(6.33f, 7.62346f, 4.f * i);
		p.radius = i * 2.2344f;
		p.type = StaticModifierType::RadioActivity;
		message->staticModifiers.push_back(p);
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

	// Barrier
	cout << "StartGame Test[Assert 10]" << endl;
	for (unsigned i = 0; i < message->barriers.size(); ++i)
	{
		assert(message->barriers[i].modifierId == received->barriers[i].modifierId);
	}

	cout << "StartGame Test[Assert 11]" << endl;
	for (unsigned i = 0; i < message->barriers.size(); ++i)
	{
		assert(message->barriers[i].position == received->barriers[i].position);
	}

	cout << "StartGame Test[Assert 12]" << endl;
	for (unsigned i = 0; i < message->barriers.size(); ++i)
	{
		assert(message->barriers[i].rotation == received->barriers[i].rotation);
	}

	cout << "StartGame Test[Assert 13]" << endl;
	for (unsigned i = 0; i < message->barriers.size(); ++i)
	{
		assert(message->barriers[i].scale == received->barriers[i].scale);
	}

	cout << "StartGame Test[Assert 14]" << endl;
	for (unsigned i = 0; i < message->barriers.size(); ++i)
	{
		assert(message->barriers[i].radius == received->barriers[i].radius);
	}


	// Static
	cout << "StartGame Test[Assert 15]" << endl;
	for (unsigned i = 0; i < message->staticModifiers.size(); ++i)
	{
		assert(message->staticModifiers[i].modifierId == received->staticModifiers[i].modifierId);
	}

	cout << "StartGame Test[Assert 16]" << endl;
	for (unsigned i = 0; i < message->staticModifiers.size(); ++i)
	{
		assert(message->staticModifiers[i].position == received->staticModifiers[i].position);
	}

	cout << "StartGame Test[Assert 17]" << endl;
	for (unsigned i = 0; i < message->staticModifiers.size(); ++i)
	{
		assert(message->staticModifiers[i].rotation == received->staticModifiers[i].rotation);
	}

	cout << "StartGame Test[Assert 18]" << endl;
	for (unsigned i = 0; i < message->staticModifiers.size(); ++i)
	{
		assert(message->staticModifiers[i].scale == received->staticModifiers[i].scale);
	}

	cout << "StartGame Test[Assert 19]" << endl;
	for (unsigned i = 0; i < message->staticModifiers.size(); ++i)
	{
		assert(message->staticModifiers[i].radius == received->staticModifiers[i].radius);
	}

	cout << "StartGame Test[Assert 20]" << endl;
	for (unsigned i = 0; i < message->staticModifiers.size(); ++i)
	{
		assert(message->staticModifiers[i].type.getType() == received->staticModifiers[i].type.getType());
	}


	// Dynamic
	cout << "StartGame Test[Assert 21]" << endl;
	for (unsigned i = 0; i < message->dynamicModifiers.size(); ++i)
	{
		assert(message->dynamicModifiers[i].modifierId == received->dynamicModifiers[i].modifierId);
	}

	cout << "StartGame Test[Assert 22]" << endl;
	for (unsigned i = 0; i < message->dynamicModifiers.size(); ++i)
	{
		assert(message->dynamicModifiers[i].position == received->dynamicModifiers[i].position);
	}

	cout << "StartGame Test[Assert 23]" << endl;
	for (unsigned i = 0; i < message->dynamicModifiers.size(); ++i)
	{
		assert(message->dynamicModifiers[i].rotation == received->dynamicModifiers[i].rotation);
	}

	cout << "StartGame Test[Assert 24]" << endl;
	for (unsigned i = 0; i < message->dynamicModifiers.size(); ++i)
	{
		assert(message->dynamicModifiers[i].scale == received->dynamicModifiers[i].scale);
	}

	cout << "StartGame Test[Assert 25]" << endl;
	for (unsigned i = 0; i < message->dynamicModifiers.size(); ++i)
	{
		assert(message->dynamicModifiers[i].radius == received->dynamicModifiers[i].radius);
	}

	cout << "StartGame Test[Assert 26]" << endl;
	for (unsigned i = 0; i < message->dynamicModifiers.size(); ++i)
	{
		assert(message->dynamicModifiers[i].type.getType() == received->dynamicModifiers[i].type.getType());
	}

	delete message;
	delete received;
	delete data;
	cout << "StartGame Test[End]" << endl;
}