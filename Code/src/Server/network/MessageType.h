#pragma once
#define _WIN32_WINNT 0x0501
class MessageType 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid message
		JoinRequest,				/// Is sent by the client when he wants to join an open game.
		JoinSuccess,				/// Is sent by the server when the join request of the client has been accepted
		JoinFailure,				/// Is sent by the server when the join request has been refused
		StartGame,					/// Is sent by the server when the game starts
		CreateCellComplete,			/// Tells the client wenn a cell is completed
		CreateCellRequest,			/// Is sent by the client when he wants to create a cell
		CreateCellSuccess,			/// Is sent by the server when the create cell has been accepted
		CreateCellFailure,			/// Is sent by the server if the createCell request is invalid (due position)
		CellNew,					/// Is sent by the server if when a new Cell appears
		CellAttack,					/// Is sent by the server when a cell attacks another
		CellDie,					/// Is sent by the server when a cell dies
		GameOver,					/// Is sent by the server when the game is over


		Last_Value					/// Is the last valid messagetype
	};
	
	MessageType(Type value);
	MessageType(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};