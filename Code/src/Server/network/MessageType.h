#pragma once

enum MessageType 
{
	JoinRequest,				/// Is sent by the client when he wants to join an open game.
	JoinSuccess,
	JoinFailure,
	StartGame,
	CreateCellRequest,
	CreateCellSuccess,
	CreateCellFailure,
	CellAttack,
	CellReceiveDamage,
	CellDie,
	PlayerWin,
	PlayerLoses
};