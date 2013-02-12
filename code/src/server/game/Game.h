#pragma once

class JoinRequest;
class CreateCellRequest;

class Game
{
public:
	Game();
	void join(JoinRequest &request);
	void createCell(CreateCellRequest &request);
};