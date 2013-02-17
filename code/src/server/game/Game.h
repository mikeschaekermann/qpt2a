#pragma once

class JoinRequest;
class CreateCellRequest;
struct CreatePolypeptideRequest;
struct MovePolypetideRequest;

class Game
{
public:
	Game();
	void join(JoinRequest & request);
	void createCell(CreateCellRequest & request);
	void createPolypetide(CreatePolypeptideRequest & request);
	void movePolypetide(MovePolypetideRequest & request);
private:
	bool testPlayerAndCell(unsigned int playerId, unsigned int cellId);
};