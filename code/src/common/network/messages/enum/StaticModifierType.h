#pragma once
#define _WIN32_WINNT 0x0501
class StaticModifierType 
{
private:
	unsigned int value;
public:
	enum Type {
		Invalid,					/// Determines that this is an invalid error code
		VirusSwarm,					/// Damages Cells in the area
		NutrientSoil,				/// Accellerates build time
		RadioActivity,				/// Modifies attacks and the cells that are built
		Last_Value					/// Is the last valid error code
	};
	
	StaticModifierType(Type value);
	StaticModifierType(unsigned int networkValue);
	Type getType();
	unsigned int getNetworkType();
};