
class Layer {
	public:
	Layer(void);
	Layer(Layer *);
	void newStep (void);
	Layer *lower;
	Layer *higher;
	unsigned int step;
	ActivationQueue *aqueue;
	RecoveryQueue *recQueue;
};

class Dendrite {
	public:
	bool weightFrozen;
	Neuron *dendriteFrom;
	Neuron *dendriteTo;
	int synapses;
	unsigned int lastUsed;
	//bool inhibitory;
	int activationDelay;
	float weight;
	Dendrite(Layer *);
	Dendrite(Layer *, int);
	void changeWeights (void);
	void stimulate (void);
	void stimulate (int);
	private:
	Layer *layer;
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
	Layer *layer;
	public:
	ActivationQueue (Layer*);
	void schedActivation (Dendrite* , int);
	void schedActivation (Dendrite* , int, double);

	void activate (void);
	bool isEmpty (void);
	//unsigned int step;

};

class RecoveryQueue {
	private:
	Layer* layer;
	int counter;
	int lastelement;
	//unsigned int lastInsertedElement;
	unsigned int stepLastElement;
	vector <Neuron*> queue[queueMax];
	public:
	Neuron *lastType2Neuron;
	RecoveryQueue (Layer*);
	int deletePattern(Neuron *,  unsigned int, unsigned int);
	void insert(Neuron *);
	void checkNewPattern ();
	void recover(void);
	//bool noChangeInCycle(void);
	int countItems (void);
	int countInputNeuronsCurrentStep(void);
};

class Neuron {
	private:

	  void drainActivation (void);
	  Layer *layer;


	public:
	  //TDebug1 *debuginst;
	  TTreeNode *debugCallingNode;
	  vector <Dendrite*> axons;
	  vector <Dendrite*> dendrites;
	  double activationVal;
	  AnsiString id;
	  unsigned int lastchecked;
	  char outputData;
	  unsigned int lastfired;
	  unsigned int blockActivation;
	  int type; //0:input, 1:intrinsic, 2:output
	  float threshold;
	  //Constructor
	  Neuron (Layer *, int);
	  //Functions
	  Dendrite* newLink (Neuron *, int, int);
	  Dendrite* newLink (Neuron *);
	  int countSynapses ();
	  void activate (double);
	  void fire (void);
	  void predictNext (void);
	  void propagateDown(int *);
	  int checkActivation (void);
	  void inhibit (void);
	  bool containsDendrite(Neuron *);
	  int axonsRemove(Dendrite *);
	  bool hasSameSuccessor(Neuron *);
};

class Sense {
	private:
	//provide 8 bit input interface
	Neuron *inputNeurons[256];
	Neuron *outputNeurons[256];
	Layer *layer;
	public:
	Sense (Layer* nLayer) {
		this->layer = nLayer;
		for ( int  i = 0 ; i < 256 ; i++ ) {
			 this->inputNeurons[i] = new  Neuron (this->layer,0);
			 this->outputNeurons[i] = new  Neuron (this->layer,2);

			 this->inputNeurons[i]->dendrites.push_back(new Dendrite(this->layer,1));
			 this->inputNeurons[i]->dendrites[0]->dendriteTo = this->inputNeurons[i];
			 this->inputNeurons[i]->dendrites[0]->dendriteFrom = this->outputNeurons[i];
			 this->inputNeurons[i]->dendrites[0]->synapses = 1;
			 this->inputNeurons[i]->dendrites[0]->weightFrozen = true;
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
			 this->outputNeurons[i]->dendrites.push_back(new Dendrite(this->layer));
			 this->outputNeurons[i]->dendrites[0]->dendriteTo = this->outputNeurons[i];
			 this->outputNeurons[i]->dendrites[0]->synapses = 1;
			 this->outputNeurons[i]->dendrites[0]->weightFrozen = true;
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
		this->outputNeurons[c]->lastfired = this->layer->step;
		//this->aqueue->schedActivation( this->inputNeurons[c]->dendrites[0],0);
		this->inputNeurons[c]->dendrites[0]->stimulate();
		//this->inputNeurons[c]->checkActivation();
	}
	void nextStep (void) {
        this->layer->aqueue->activate();
    }
};
