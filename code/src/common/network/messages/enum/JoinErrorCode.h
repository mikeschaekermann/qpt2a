#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
class JoinErrorCode 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid error code
		Unknown,					/// Server doesn't want to say why
		GameIsFull,					/// The game is already full
		NameAlreadyTaken,			/// The name in the join request has already been taken
		Last_Value					/// Is the last valid error code
	};
	
	JoinErrorCode(Type value);
	JoinErrorCode(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};