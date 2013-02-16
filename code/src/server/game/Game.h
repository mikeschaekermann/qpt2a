#pragma once

class JoinRequest;
class CreateCellRequest;
struct CreatePolyPeptideRequest;

class Game
{
public:
	Game();
	void join(JoinRequest & request);
	void createCell(CreateCellRequest & request);
	void createPolypetide(CreatePolyPeptideRequest & request);
};