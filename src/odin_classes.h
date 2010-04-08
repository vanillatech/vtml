
class Dendrite {
	public:
	ActivationQueue *aqueuelocal;
	Neuron *dendriteFrom;
	Neuron *dendriteTo;
	int synapses;
	int lastUsed;
	//bool inhibitory;
	int activationDelay;
	float weight;
	Dendrite(void);
	Dendrite(int);
	void changeWeights (void);
	void stimulate (void);
};


class Aqueue {
	private:
	int pos;
	double actVal;
	Dendrite* dendrite;
	public:
	Aqueue(int queuePos, Dendrite* dendrite) {
		this->pos = queuePos;
		this->dendrite = dendrite;
	}
	Aqueue(double aVal, Dendrite* dendrite) {
		this->actVal = aVal;
		this->dendrite = dendrite;
	}
	Aqueue(int queuePos, Dendrite* dendrite,double aVal) {
		this->actVal = aVal;
		this->dendrite = dendrite;
		this->pos = queuePos;
	}
	Dendrite* getDendrite() const {
		return this->dendrite;
	}
	bool retMode () const {
		if (this->pos != 0) return true;
		else return false;
	}
	double getPos() const {
		if (this->retMode()) return this->pos;
		else return this->actVal;
	}
	double getAct() const {
		return this->actVal;
	}
};

class ActivationQueue {
	private:
	priority_queue<Aqueue> q;
	Dendrite *queue[queueMax];
	public:
	ActivationQueue (void) {
	}
	void schedActivation (Dendrite* , int);
	void schedActivation (Dendrite* , int, double);

	void activate (void);
	bool isEmpty (void);

};

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

class Neuron {
	private:

	  void drainActivation (void);


	public:
	  //TDebug1 *debuginst;
	  TTreeNode *debugCallingNode;
	  ActivationQueue* activationQueue;
	  vector <RecoveryQueue> *recoveryQueue;
	  vector <Dendrite*> axons;
	  vector <Dendrite*> dendrites;
	  double activationVal;
	  int layer;
	  AnsiString id;
	  int lastchecked;
	  char outputData;
	  int lastfired;
	  int blockActivation;
	  int type; //0:temporal, 1:spatial, 2:downward
	  float threshold;
	  //Constructor
	  Neuron (ActivationQueue*, vector< RecoveryQueue > * ,unsigned int, int);
	  //Functions
	  void newLink (Neuron *, int, int);
	  int countSynapses ();
	  void activate (double);
	  void fire (void);
	  void predictNext (void);
	  void propagateDown(int *);
	  int checkActivation (void);
	  void inhibit (void);
	  bool containsDendrite(Neuron *);
	  int axonsRemove(Dendrite *);
};

class Sense {
	private:
	ActivationQueue *aqueue;
	Neuron *inputNeurons[256];
	Neuron *outputNeurons[256];
	public:
	Sense (ActivationQueue *queue, vector< RecoveryQueue > *recQueueTemp) {
		//inputNeurons = new (Neuron*)[256];
		//inputNeurons = new Neuron(&queue)[256];
		//first check that the capacity of recQueue is sufficient
		(*recQueueTemp).reserve((*recQueueTemp).size()+3);
		this->aqueue = queue;
		for ( int  i = 0 ; i < 256 ; i++ ) {
			 this->inputNeurons[i] = new  Neuron (aqueue,recQueueTemp,0,0);
			 this->outputNeurons[i] = new  Neuron (aqueue,recQueueTemp,0,0);

			 this->inputNeurons[i]->dendrites.push_back(new Dendrite(1));
			 this->inputNeurons[i]->dendrites[0]->dendriteTo = this->inputNeurons[i];
			 this->inputNeurons[i]->dendrites[0]->dendriteFrom = this->outputNeurons[i];
			 this->inputNeurons[i]->dendrites[0]->synapses = 1;
			 /*this->inputNeurons[i]->axons.push_back(new Dendrite());
			 this->inputNeurons[i]->axons[0]->dendriteTo = this->outputNeurons[i];
			 this->inputNeurons[i]->axons[0]->dendriteFrom = this->inputNeurons[i];
			 this->inputNeurons[i]->axons[0]->synapses = 1;*/
			 /*this->outputNeurons[i]->axons.resize(1);
			 this->outputNeurons[i]->axons[0].push_back(new Dendrite());
			 this->outputNeurons[i]->axons[0][0]->dendriteFrom = this->outputNeurons[i];
			 this->outputNeurons[i]->axons[0][0]->dendriteTo = this->inputNeurons[i];
			 this->outputNeurons[i]->axons[0][0]->synapses = 1;
			 this->outputNeurons[i]->axons[0][0]->self=0;*/
			 this->outputNeurons[i]->dendrites.push_back(new Dendrite());
			 this->outputNeurons[i]->dendrites[0]->dendriteTo = this->outputNeurons[i];
			 this->outputNeurons[i]->dendrites[0]->synapses = 1;
			 this->outputNeurons[i]->dendrites[0]->activationDelay = 0;

			 //this->outputNeurons[i]->dendrites[1]->inhibitory = true;

			 AnsiString captionID = char(i);
			 outputNeurons[i]->outputData = char(i);
			 captionID +=  " (" +  inputNeurons[i]->id + ")";
			 inputNeurons[i]->debugCallingNode = SDIAppForm->addLink(captionID);
			//*inputNeurons = new Neuron (queue);
		}


	}
	void input (int c) {
		this->outputNeurons[c]->lastfired = stepCounter;
		//this->aqueue->schedActivation( this->inputNeurons[c]->dendrites[0],0);
		this->inputNeurons[c]->dendrites[0]->stimulate();
		//this->inputNeurons[c]->checkActivation();
	}
};
