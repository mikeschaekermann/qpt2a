#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
class CreatePolypeptideErrorCode 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid error code
		InvalidPlayer,			/// This position is already occupied by another cell
		CellFull,				/// The given identifier is already used by another cell
		NoPolypeptideCapacities,
		Last_Value					/// Is the last valid error code
	};
	
	CreatePolypeptideErrorCode(Type value);
	CreatePolypeptideErrorCode(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};