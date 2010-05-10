#pragma once
#include <string>

namespace Odin
{

typedef char TElement;
typedef std::string TPattern;

struct Configuration
{
	float threshold;
	float activationDecay;
	float defaultImpulse;

	Configuration() :
		threshold(0.2f),
		activationDecay(0.95f),
		defaultImpulse(1.0f)
	{}
};

} //Odin
