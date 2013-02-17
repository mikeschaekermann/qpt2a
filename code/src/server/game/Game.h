#pragma once

class JoinRequest;
class CreateCellRequest;
class PlayerServer;
class CellServer;

struct CreatePolipeptideRequest
{
	unsigned int requestId;
	unsigned int playerId;
};

struct MovePolipeptideRequest
{
	unsigned int requestId;
	unsigned int fromCellId;
	unsigned int toCellId;
	unsigned int amount;
};

class Game
{
public:
	Game();
	void join(JoinRequest & request);
	void createCell(CreateCellRequest & request);
	void createPolypetide(CreatePolipeptideRequest & request);
	void movePolypetide(MovePolipeptideRequest & request);
private:
	PlayerServer* testPlayer(unsigned int playerId);
	CellServer* testCell(unsigned int cellId);
};