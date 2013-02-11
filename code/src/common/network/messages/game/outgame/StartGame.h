#pragma once
#include <vector>

#include "../../NetworkMessage.h"

#include "NetworkPlayer.h"
#include "NetworkBarrier.h"
#include "NetworkStaticModifier.h"
#include "NetworkDynamicModifier.h"

class StartGame : public NetworkMessage
{
public:
	/* Fields */
	float worldRadius;

	std::vector<NetworkPlayer> players;
	std::vector<NetworkBarrier> barriers;
	std::vector<NetworkStaticModifier> staticModifiers;
	std::vector<NetworkDynamicModifier> dynamicModifiers;

	/* Constructor / Destructor */
	StartGame();														/// Default constructor
	StartGame(char *data, unsigned &index);								/// Construct from buffer starting by the given index
	StartGame(const NetworkMessage &other);								/// Copy constructor which copies the network address
	StartGame(const StartGame &other);									/// Copy constructor
	virtual ~StartGame();												/// Default Destructor

	/* Methods */
	virtual unsigned writeToArray(char* data, unsigned start = 0);		/// Serializes the class to the array
	virtual unsigned calculateSize();									/// Calculates and returns the size of this message
};