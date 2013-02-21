#include <iostream>

#include "NetworkMessageTest.h"
#include "ConnectionMessageTest.h"
#include "JoinSuccessTest.h"
#include "JoinFailureTest.h"
#include "JoinRequestTest.h"
#include "GameOverTest.h"
#include "StartGameTest.h"
#include "CellDieTest.h"
#include "CellAttackTest.h"
#include "CellNewTest.h"
#include "CreateCellCompleteTest.h"
#include "CreateCellFailureTest.h"
#include "CreateCellRequestTest.h"
#include "CreateCellSuccessTest.h"
#include "EnvironmentAttackTest.h"
#include "EnvironmentMoveTest.h"
#include "ResourceBonusTest.h"
#include "PolypeptideFightTest.h"

using namespace std;

int main()
{
	testNetworkMessage();
	cout << endl;

	testConnectionMessage();
	cout << endl;

	testJoinSuccess();
	cout << endl;

	testJoinFailure();
	cout << endl;

	testJoinRequest();
	cout << endl;

	testGameOver();
	cout << endl;

	testStartGame();
	cout << endl;

	testCellAttack();
	cout << endl;

	testCellDie();
	cout << endl;

	testCellNew();
	cout << endl;

	testCreateCellComplete();
	cout << endl;

	testCreateCellFailure();
	cout << endl;

	testCreateCellRequest();
	cout << endl;

	testCreateCellSuccess();
	cout << endl;

	testEnvironmentAttack();
	cout << endl;

	testEnvironmentMove();
	cout << endl;

	testResourceBonus();
	cout << endl;

	testPolypeptideFight();
	cout << endl;

	cout << "All Test ran through! (Have you checked for new fields in the messages?)" << endl;

	char read;
	cin >> read;
	return 0;
}
