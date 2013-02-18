#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
class MovePolypeptideErrorCode 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid error code
		InvalidCells,			/// This position is already occupied by another cell
		DifferentOwner,				/// The given identifier is already used by another cell
		TargetCellFull,
		Last_Value					/// Is the last valid error code
	};
	
	MovePolypeptideErrorCode(Type value);
	MovePolypeptideErrorCode(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};