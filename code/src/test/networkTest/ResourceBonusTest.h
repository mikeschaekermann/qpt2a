#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/environment/ResourceBonus.h"

using namespace std;

void testResourceBonus()
{
	cout << "ResourceBonus Test[Start]" << endl;

	ResourceBonus *message = new ResourceBonus();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->modifierId = 8;
	message->amount= 23.23f;
	message->duration = 12.345f;
	message->cellId = 38;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	ResourceBonus *received = new ResourceBonus(data, index);

	cout << "ResourceBonus Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "ResourceBonus Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "ResourceBonus Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "ResourceBonus Test[Assert 4]" << endl;
	assert(message->modifierId == received->modifierId);

	cout << "ResourceBonus Test[Assert 5]" << endl;
	assert(message->cellId == received->cellId);
	
	cout << "ResourceBonus Test[Assert 6]" << endl;
	assert(message->amount == received->amount);
	
	cout << "ResourceBonus Test[Assert 7]" << endl;
	assert(message->duration == received->duration);

	delete message;
	delete received;
	delete data;
	cout << "ResourceBonus Test[End]" << endl;
}