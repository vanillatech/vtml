#pragma once
#include <string>
#include <vector>

using namespace std;

class Dendrite;
class Layer;

class Neuron {
	private:

	  void drainActivation (void);
	  Layer *layer;
	  vector <Neuron*> neighbors;

	public:
	  //TDebug1 *debuginst;
	//  TTreeNode *debugCallingNode;
	  vector <Dendrite*> axons;
	  vector <Dendrite*> dendrites;
	  double activationVal;

#ifdef BORLAND_GUI
	  AnsiString id;
#else
	  std::string id;
#endif
	  unsigned int lastchecked;
	  char outputData;
	  unsigned int lastfired;
	  unsigned int blockActivation;
	  int type; //0:input, 1:intermediate, 2:output
	  float threshold;
	  
	  //Constructor
	  Neuron (Layer *, int);
	  
	  //Dtor
	  ~Neuron();

	  //Functions
	  Dendrite* newLink (Neuron *, int, int, float);
	  Dendrite* newLink (Neuron *, float);
	  Dendrite* newLink (Neuron *);
	  int countSynapses ();
	  int countSynapsesOnAxons(void);
	  int countMaxSynapsesOnAxons(void);
	  void activate (double);
	  void fire (void);
	  void predictNext (void);
	  void propagateDown(int);
	  float getAxonsWeight(void);
	  int checkActivation (void);
	  void inhibit (void);
	  void inhibit (bool);
	  bool containsDendrite(Neuron *);
	  bool isOutputNeuron (void);
	  bool isInhibited(void);
	  bool newNeighbor(Neuron *);
	  bool isNeighborOf(Neuron *);
	  bool hasNeighbors(void);
	  void newOutput(void);
	  Layer *getLayer(void);
	  int axonsRemove(Dendrite *);
	  bool hasSameSuccessor(Neuron *);
};

