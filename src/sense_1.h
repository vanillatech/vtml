#pragma once
#include "neuron_1.h"
#include "ICallbacks.h"
#include "ISense.h"

namespace Odin
{

class Sense1 : public ISense<unsigned char>
{
	public:
		struct Parameters
		{
			int queueMax;
			int recoveryTime;
			int blockTime;
			float activationBias;
			float drain;
			float minActivation;
			float fireMargin;
			float learnRate;
			//~etc

			Parameters() :
				queueMax(QUEUE_MAX),
				recoveryTime(RECOVERY_TIME),
				blockTime(BLOCK_TIME),
				activationBias(ACTIVATION_BIAS),
				drain(DRAIN),
				minActivation(MIN_ACTIVATION),
				fireMargin(FIRE_MARGIN),
				learnRate(LEARN_RATE)
			{}
		};

		explicit Sense1(ICallbacks& a_callbacks, const Parameters& a_parameters = Parameters()) :
			ISense(a_callbacks),
			m_params(a_parameters)
				// etc
		{}
		~Sense1();

		virtual void input(const unsigned char&);

	private:
		Parameters m_params;


		// No copying so far.
		Sense1(const Sense1&);
		Sense1& operator=(const Sense1&);
};

} //Odin
