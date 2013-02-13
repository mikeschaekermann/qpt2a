#pragma once
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif
class CellType 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid cell type
		StemCell,					/// The main cell for the player
		StandardCell,				/// The standard cell
		BoneCell,					/// The bone cell
		Last_Value					/// Is the last valid cell type
	};
	
	CellType(Type value);
	CellType(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};