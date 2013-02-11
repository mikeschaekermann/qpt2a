#pragma once
#define _WIN32_WINNT 0x0501
class DynamicModifierType 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid error code
		VirusSwarm,					/// Attacks nearby cells and wanders around the map
		VitaminGusher,				/// Perodically gives resources to nearby cells 
		FreeFatCell,				/// FatCell that can be aquired by a player
		Last_Value					/// Is the last valid error code
	};
	
	DynamicModifierType(Type value);
	DynamicModifierType(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};