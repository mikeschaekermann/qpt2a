#pragma once

class JoinRequest;
class CreateCellRequest;
class CreatePolypeptideRequest;
class MovePolypeptideRequest;
class PlayerServer;
class CellServer;

class Game
{
public:
	Game();
	void join(JoinRequest & request);
	void createCell(CreateCellRequest & request);
	void createPolypetide(CreatePolypeptideRequest & request);
	void movePolypetide(MovePolypeptideRequest & request);
private:
	PlayerServer* testPlayer(unsigned int playerId);
	CellServer* testCell(unsigned int cellId);
};