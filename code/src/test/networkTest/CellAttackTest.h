#include <cassert>
#include <iostream>

#include "../../common/network/messages/game/ingame/cell/combat/CellAttack.h"

using namespace std;

void testCellAttack()
{
	cout << "CellAttack Test[Start]" << endl;

	CellAttack *message = new CellAttack();
	message->messageId = 32;
	message->messageType = MessageType::JoinFailure;
	message->attackedCellId = 8;
	message->attackerCellId= 23;
	message->damage = 12.345f;
	message->messageSize = message->calculateSize();
	
	char *data = new char[message->messageSize];

	message->writeToArray(data, 0);


	unsigned index = 0;
	CellAttack *received = new CellAttack(data, index);

	cout << "CellAttack Test[Assert 1]" << endl;
	assert(index == message->messageSize && index == received->messageSize);
	cout << "CellAttack Test[Assert 2]" << endl;
	assert(message->messageType.getType() == received->messageType.getType());
	cout << "CellAttack Test[Assert 3]" << endl;
	assert(message->messageId == received->messageId);

	cout << "CellAttack Test[Assert 4]" << endl;
	assert(message->attackedCellId == received->attackedCellId);

	
	cout << "CellAttack Test[Assert 5]" << endl;
	assert(message->attackerCellId == received->attackerCellId);
	
	cout << "CellAttack Test[Assert 6]" << endl;
	assert(message->damage == received->damage);

	delete message;
	delete received;
	delete data;
	cout << "CellAttack Test[End]" << endl;
}