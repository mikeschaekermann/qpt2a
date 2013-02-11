#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/environment/EnvironmentMove.h"

using namespace std;

void testEnvironmentMove()
{
	cout << "EnvironmentMove Test[Start]" << endl;

	EnvironmentMove *message = new EnvironmentMove();
	message->messageId = 32;
	message->modifierId = 8;
	message->moveToPosition = ci::Vec3f(324.f, 45.238f, 0.324f);
	message->travelTime = 12.345f;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	EnvironmentMove *received = new EnvironmentMove(data, index);

	cout << "EnvironmentMove Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "EnvironmentMove Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "EnvironmentMove Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "EnvironmentMove Test[Assert 4]" << endl;
	assert(message->modifierId == received->modifierId);

	
	cout << "EnvironmentMove Test[Assert 5]" << endl;
	assert(message->moveToPosition == received->moveToPosition);
	
	cout << "EnvironmentMove Test[Assert 6]" << endl;
	assert(message->travelTime == received->travelTime);

	delete message;
	delete received;
	delete data;
	cout << "EnvironmentMove Test[End]" << endl;
}