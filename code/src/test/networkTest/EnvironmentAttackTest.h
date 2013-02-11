#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/environment/EnvironmentAttack.h"

using namespace std;

void testEnvironmentAttack()
{
	cout << "EnvironmentAttack Test[Start]" << endl;

	EnvironmentAttack *message = new EnvironmentAttack();
	message->messageId = 32;
	message->attackedCellId = 8;
	message->modifierId = 235;
	message->damage = 12.345f;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	EnvironmentAttack *received = new EnvironmentAttack(data, index);

	cout << "EnvironmentAttack Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "EnvironmentAttack Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "EnvironmentAttack Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "EnvironmentAttack Test[Assert 4]" << endl;
	assert(message->attackedCellId == received->attackedCellId);

	
	cout << "EnvironmentAttack Test[Assert 5]" << endl;
	assert(message->modifierId == received->modifierId);
	
	cout << "EnvironmentAttack Test[Assert 6]" << endl;
	assert(message->damage == received->damage);

	delete message;
	delete received;
	delete data;
	cout << "EnvironmentAttack Test[End]" << endl;
}