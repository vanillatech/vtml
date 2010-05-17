#pragma once
#include <string>

namespace Odin
{

typedef char TElement;
typedef std::string TPattern;

struct Configuration
{
	int queueMax;
	int recoveryTime;
	int blockTime;
	float activationBias;
	float drain;
	float minActivation;
	float fireMargin;
	float learnRate;
	float defaultImpulse;

	Configuration() :
		queueMax(9),
		recoveryTime(1),
		blockTime(1),
		activationBias(0.5f),
		drain(0.9f),
		minActivation(0.01f),
		fireMargin(0.1f),
		learnRate(0.5f),
		defaultImpulse(1.0f)
	{}
};

} //Odin
