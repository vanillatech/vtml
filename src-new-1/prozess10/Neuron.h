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
	  void propagateDown(int);
	  int checkActivation (void);
	  void inhibit (void);
	  bool containsDendrite(Neuron *);
	  int axonsRemove(Dendrite *);
	  bool hasSameSuccessor(Neuron *);
};
