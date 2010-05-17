#include "neuron.h"
#include "layer.h"

#pragma warning(disable: 4355) //'this' in ctor. initializer list

namespace Odin
{
/*
int m_layerNumber;
		const Configuration& m_config;
		TNeurons m_neurons;
		InputMap m_inputs;
		ActivationFunc m_af;
		ActivationQueue<ActivationFunc> m_AQ;
		RecoveryQueue m_RQ;
		int m_timeCounter;
*/


Layer::Layer(int layerNumber, const Configuration& config, Model& model) :
	m_layerNumber(layerNumber),
	m_config(config),
	m_model(model),
	m_af(*this),
	m_AQ(m_af),
	m_timeCounter(0)
{}

Layer::~Layer()
{
	TNeurons::iterator it = m_neurons.begin();
	TNeurons::iterator end = m_neurons.end();
	for (; it != end; ++it) {
		delete (*it);
	}
}

const float Layer::DEFAULT_IMPULSE = -1.0f;

void Layer::step(const TPattern& pattern, float impulse)
{
	float imp = (impulse > 0.0f) ? impulse : m_config.defaultImpulse;
	
	// If input neuron designated by given pattern doesn't exist, it's created.
	// (Then this neuron is fired.)
	Neuron* n = m_inputs.find(pattern);
	if (!n) {
		n = addNeuron();
		m_inputs.insert(pattern, n);
		exciteNeuron(*n, imp);
		fireNeuron(*n);
	}
	else {
		exciteNeuron(*n, imp);
	}
	


	decayActivation();		
}

Neuron* Layer::addNeuron()
{
	Neuron* n = new Neuron(m_config.fireMargin);
	m_neurons.insert(n);
	return n;
}

void Layer::exciteNeuron(Neuron& n, float impulse)
{
	n.activationValue += impulse;
	if (n.activationValue > n.threshold)
		fireNeuron(n);
}

/*
void Neuron::fire (void) {
          if (this->outputData != 0) {
			SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
		  }
		  Debug1->ListBox1->Items->Insert(0,"Neuron fired: " + id + " ActVal: " + activationVal );
		  if (id == 200) {
			  int stophere = 1;
		  }
		  this->activationVal = 0;
		  this->lastfired = stepCounter;
          // Neuron abtrennen wenn keine Synapsen mehr 0.0.67
		  if (this->layer > 0) {
		  for (unsigned int n=0;n<dendrites.size();n++) {
				if (dendrites[n] != 0) {
                //learn
				dendrites[n]->changeWeights();
				  if ((*dendrites[n]).dendriteFrom != 0)

					if ((*dendrites[n]).synapses == 0) {
						// If enabled again please be sure that there is not dendrite
						// in the activation queue anymore. otherwise segfaults.
						//int rettest = (*dendrites[n]).dendriteFrom->axonsRemove(dendrites[n]);

						//delete dendrites[n];
						//dendrites.erase(dendrites.begin()+n);
						//(*dendrites[n][m]).dendriteFrom = 0;
						//(*dendrites[n][m]).dendriteTo = 0;


					} else {
					  //WTA: Winner Takes All - inhibit
					  (*dendrites[n]).dendriteFrom->inhibit();
					}
					(*dendrites[n]).synapses--;
				}
		  }
		  }
		  //activate parent neurons
		  for (unsigned int n=0;n<axons.size();n++ ) {
					Debug1->ListBox1->Items->Insert(0,"Schedule Activation: " + axons[n]->dendriteTo->id + " in: " + AnsiString((*axons[n]).activationDelay) );
					axons[n]->stimulate();
					//this->activationQueue->schedActivation(&(*axons[n]),(*axons[n]).activationDelay);
		  }
		  //only insert into rec.queue when fired twice within recoveryTime
		  //--EXPERIMENTAL--
		  //if (this->lastfired + recoveryTime > stepCounter) {

		  //delete fired pattern from recoveryqueue
		  // -- Don't do this anymore 0.2.02
		  //if (this->layer >= 1) {
//
//					  (*recoveryQueue)[this->layer - 1].deletePattern(this,0,0);

		  //} 
		 
		  (*recoveryQueue)[this->layer].insert(this);
		  //this->predictNext();
		  //if (this->layer > 0) {
		//	  this->propagateDown(0);
		 // } 
		  
		  //0.0.37
		  //if (lastFiredNeuron != 0) {
		//	if (!this->containsDendrite(lastFiredNeuron)) {
		//		lastFiredNeuron->newLink(this,1,lastFiredNeuron->dendrites.size(),1);
		//	}
		 // }
		  lastFiredNeuron = this;

		  //} 
}
*/

void Layer::fireNeuron(Neuron& n)
{
	float impulse = n.activationValue;
	n.activationValue = 0.0f;
	if (n.totalOutputsWeight > 0.0f) {
		impulse /= n.totalOutputsWeight;
		Neuron::TOutputs::iterator it = n.outputs.begin();
		Neuron::TOutputs::iterator end = n.outputs.end();
		for (; it != end; ++it) {
			Dendrite* d = *it;
			m_AQ.scheduleActivation(d, impulse*d->weight, d->activationDelay);
		}
	}
	m_RQ.put(n);
}

/*
void Neuron::drainActivation(void) {
  for (int n=0;n<50 && n<(stepCounter-lastchecked)&&lastchecked>0 ;n++ ) {
			this->activationVal *= drain;
  }
  lastchecked = stepCounter;
}
*/

void Layer::decayActivation()
{
	TNeurons::iterator it = m_neurons.begin();
	TNeurons::iterator end = m_neurons.end();
	for (; it != end; ++it) {
		(*it)->activationValue *= m_config.drain;
	}
}

/*
void Neuron::activate(double activationValNew) {
  this->drainActivation();
  if (this->lastfired < stepCounter - recoveryTime && this->blockActivation < stepCounter) {
	this->activationVal += activationValNew;
	Debug1->ListBox1->Items->Insert(0,"Activate Neuron: " + id + " (Increase: " + AnsiString (activationValNew)+"; activationVal: " + AnsiString(activationVal) + ") " );
	this->checkActivation();
  };
}
*/

/*
void ActivationQueue::activate (void) {
	stepCounter++;
	for (unsigned int n=0;n<recQueue.size();n++ ) {
				recQueue[n].recover();
	}
	Debug1->ListBox1->Items->Insert(0,AnsiString("----------- ") + AnsiString(stepCounter) + AnsiString("------------"));
	priority_queue <Aqueue> qtemp;
	//first activate subsequent neurons
	while ((!this->q.empty()) && this->q.top().getPos() <= stepCounter) {
		Dendrite *d;
		d = this->q.top().getDendrite();
		double actVal = 0;
		//int cs = d->dendriteTo->countSynapses();
		//if (cs > 0) actVal = d->synapses / double(d->dendriteTo->countSynapses());
		//else actVal = 0;
		actVal = this->q.top().getAct();
		//actVal = 1;
		if (actVal > minActivation) {
		if (d->dendriteTo->lastfired < (stepCounter - recoveryTime)) {
			d->dendriteTo->activate(actVal);
			qtemp.push(Aqueue(d->dendriteTo->activationVal,d));
		}}
		this->q.pop();
	}
	//now check if the activation leads to depolarization of the subsequent neuron
	while (!qtemp.empty()) {
		Dendrite *d;
		d = qtemp.top().getDendrite();

		if (d->dendriteTo->checkActivation() == 1) {
			//d->synapses += 2; //hebbian learning rule - synaptic plasticity
		}
        d->synapses += 1; //hebbian learning rule - synaptic plasticity
		qtemp.pop();
	}
}
*/

void Layer::activate(AQItem& item)
{
	//~~~
}

/*
void Neuron::predictNext(void) {
	Debug1->ListBox1->Items->Insert(0,"PredictNext: " + this->id );
	//0.0.50: select strongest axon
	Dendrite *strongestAxon = NULL;
	for (unsigned int g=0;g<this->axons.size() ;g++ ) {
		if (g==0 && this->axons[0] != 0) strongestAxon = this->axons[0];
		if (this->axons[g] != 0) {
			AnsiString d = this->axons[g]->dendriteFrom->id;
			AnsiString e = this->axons[g]->dendriteTo->id;
			//a is actually wrong! will only cause to count the lastfired axon to be used for calculation.
			double a, b;
			if (strongestAxon->dendriteTo->countSynapses() > 0) {
				a = strongestAxon->synapses / double(strongestAxon->dendriteTo->countSynapses());
			} else a = 0;
			if (this->axons[g]->dendriteTo->countSynapses() > 0) {
				b = this->axons[g]->synapses / double(this->axons[g]->dendriteTo->countSynapses());
			} else b = 0;
			if (a < b) {
				strongestAxon = this->axons[g];
			}
		}
	}
	//--0.0.50

	if (strongestAxon) {
		if (strongestAxon->dendriteTo != 0) {

			for (unsigned int n=0;n<strongestAxon->dendriteTo->dendrites.size() ;n++ ) {
				if (strongestAxon->dendriteTo->dendrites[n]->dendriteFrom != 0) {
						int timeOffset = strongestAxon->activationDelay;
						timeOffset -= strongestAxon->dendriteTo->dendrites[n]->activationDelay;
						if (timeOffset > 0) {
							int tOffset = 0;
							strongestAxon->dendriteTo->dendrites[n]->dendriteFrom->propagateDown(&tOffset);
						}
				}
				//if (this->axons[0]->dendriteTo->dendrites[m][n] == this->axons[0]) {
				//	if (this->axons[0]->dendriteTo->dendrites[m][n+1]->dendriteFrom != 0) {
				//		this->axons[0]->dendriteTo->dendrites[n+1]->dendriteFrom->propagateDown();
				//	}
				//}

			}
		}
	}
}

void Neuron::propagateDown(int *timeOffset) {
	int toutput = int(timeOffset);
	Debug1->ListBox1->Items->Insert(0,"PropagateDown: " + this->id + " timeoffset: " + AnsiString(toutput) );
	if (this->outputData != 0) {
			this->activationQueue->schedActivation(&(*dendrites[0]), int(timeOffset));
			//SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
			//this->fire();
	}
	int lastDelay = 0;
	for (unsigned int m=this->dendrites.size() - 1 ; m>0 ; m--) {

		if (this->dendrites[m]->dendriteFrom != 0) {

			if (lastDelay != this->dendrites[m]->activationDelay) {
				timeOffset += (this->dendrites[m]->activationDelay - lastDelay);
			}
			this->dendrites[m]->dendriteFrom->propagateDown(timeOffset);
		}

	}
}

int Neuron::checkActivation (void) {
	this->drainActivation();
	if (this->activationVal >= this->threshold + fireMargin) {
			  this->fire();
			  return(1);
	}
	if (this->lastfired == stepCounter) {
        return (1);
	}
	return(0);
}

void Neuron::inhibit (void) {
		  //inhibits current neuron and all successors with the value that had
		  //been scheduled for activation of the successor before.
		  Debug1->ListBox1->Items->Insert(0,"Inhibit: " + id );

		  this->blockActivation = stepCounter + blockTime;//-1; //sets recoveryTime
		  this->activationVal = 0;
		  for (unsigned int n=0;n<axons.size();n++ ) {
				//double inval = double((*axons[n]).synapses) / (*axons[n]).dendriteTo->countSynapses((*axons[n]).self);
				//(*axons[n]).dendriteTo->activationVal -= inval;
				(*axons[n]).dendriteTo->blockActivation = stepCounter + blockTime;
                (*axons[n]).dendriteTo->activationVal = 0;
		  }
}
*/

/*
class RecoveryQueue {
	private:
	int counter;
	int lastelement;
	int lastInsertedElement;
	int stepLastElement;
	int layer;
	vector <Neuron*> queue[queueMax];
	public:
	Neuron *lastType2Neuron;
	RecoveryQueue(void) {}
	RecoveryQueue (int);
	int deletePattern(Neuron *,  unsigned int, unsigned int);
	void insert(Neuron *);
	void checkNewPattern ();
	void recover(void);
	bool noChangeInCycle(void);
	int countItems (void);
};
*/


/*
int RecoveryQueue::deletePattern(Neuron *neuron, unsigned int pos, unsigned int forderOld) {
   int forderNew = 0;
   int retVal = 0;
   if (pos == neuron->dendrites.size()) {return(1);}
   if (pos > 0) {
	   int delay = neuron->dendrites[pos]->activationDelay - neuron->dendrites[pos - 1]->activationDelay;
	   forderNew = forderOld - delay;
	   if (forderNew < 0) forderNew+=queueMax;
	   for (int n = 0;n<queueMax && !queue[forderNew].empty();n++) {
		if (neuron->dendrites[pos]->dendriteFrom == queue[forderNew][n]) {
		  retVal = deletePattern(neuron,pos+1,forderNew);
		  if (retVal == 1) {
			Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[forderNew][n]->id);
			queue[forderNew].erase(queue[forderNew].begin()+n);
			return(1);
		  }

		}
	   }
   } else {
	for (int m=0;m<queueMax ;m++ ) {
	for (unsigned int n=0;n < queue[m].size() ;n++ ) {
			if (neuron->dendrites[pos]->dendriteFrom == queue[m][n] ) {
				retVal = deletePattern(neuron,pos+1,m);
				if (retVal == 1) {
                   Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[m][n]->id);
				   queue[m].erase(queue[m].begin()+n);
				   return(1);
				}
			}
	}}
   }
   return(retVal);
}


void RecoveryQueue::insert(Neuron *neuron) {
        //if a new neuron is inserted into the queue it is not idle anymore.
		if (recQueueIdle >= this->layer) recQueueIdle = this->layer - 1;

		//insert actual neuron
		this->queue[this->counter].push_back(neuron);
		this->lastInsertedElement = stepCounter;
		//new type=2 neuron
		if (this->layer > 1) {
            
			Neuron *dNeuron = new Neuron(&aqueue,&recQueue,this->layer,2);
			neuron->newLink(dNeuron,0,1);
			this->lastType2Neuron = dNeuron;
		}
		//link lastType2Neuron of uppler layer to this element
		// -- segmentation fault
		//if (recQueue[this->layer+1].lastType2Neuron != 0) {
		//	recQueue[this->layer+1].lastType2Neuron->newLink(neuron,0,1);
		//}

}

void RecoveryQueue::checkNewPattern() {
	//Associate everything in recQueue with a new neuron
		//old: only check for new pattern if nothing has been fired on all of the layers below current layer (0.0.61)
		//if (this->queue[this->lastelement].empty() && recQueueIdle == (this->layer - 1)) {
		//	recQueueIdle = this->layer;
		//}
		//new: if recovery queue is 'full' and on the upper layer has nothing been fired
		//means - if no pattern has been recognized
		//then insert a new neuron in our model representing the current queue as a pattern
		if (this->noChangeInCycle() && this->countItems() > 1) {
			
	
		//if ( recQueueIdle == this->layer && this->countItems() > 1) { //experimental
		//if ( this->countItems() > 1) { //experimental
			Neuron *newNeuron = new Neuron(&aqueue,&recQueue,this->layer + 1,0);
			int c = this->counter - 1;
			for (int n=0;n<queueMax ;n++ ) {

				if (c<0) { c = queueMax - 1; }
				unsigned int countTotal = queue[c].size();
				if (countTotal > 1) {
				Neuron *newNeuron2 = new Neuron(&aqueue,&recQueue,this->layer + 1,1);
				for (unsigned int m=0;m < countTotal ;m++ ) {
						queue[c][m]->newLink(newNeuron2,0,countTotal);
				}
				newNeuron2->newLink(newNeuron,n,countTotal);
				} else if (countTotal == 1) {
					queue[c][0]->newLink(newNeuron,n,countTotal);
				}
				c--;
			}
			//deletePattern(newNeuron,0,0);
			newNeuron->fire();
			//lastFiredNeuron = newNeuron;
		}

		if (recQueue.capacity() < recQueue.size() + 3) {
			recQueue.reserve(recQueue.size()+3);
		}
}

void RecoveryQueue::recover(void) {
    if (stepLastElement != stepCounter) {


		this->lastelement = this->counter;
		this->counter++;
		if (this->counter>=queueMax) {  this->counter = 0; }
		this->queue[this->counter].clear();
		this->checkNewPattern();
		stepLastElement = stepCounter;
	}
}

bool RecoveryQueue::noChangeInCycle() {
	if (recQueue[this->layer+1].lastInsertedElement <=stepCounter - queueMax && (recQueue[this->layer+1].lastInsertedElement != 0 || stepCounter > queueMax)) {
		return(true);
	}
	return(false);
}
*/

/*
void Dendrite::changeWeights (void) {
		if (this->lastUsed + queueMax >= stepCounter) {
			//if last usage of dendrite was within recoveryTime
			// then: learn
			this->weight = this->weight * (1 - learnRate )+    (1 - learnRate);
		} else {
			//don't learn: this dendrite had not been used to activate the neuron
			//so this dendrite does not belong to the recognized pattern
			// so forget it
			this->weight = this->weight * (1 - learnRate );
        }
};

void Dendrite::stimulate (void) {
		this->aqueuelocal->schedActivation(this, this->activationDelay, this->weight);
		this->lastUsed = stepCounter;
};
*/

} //Odin
