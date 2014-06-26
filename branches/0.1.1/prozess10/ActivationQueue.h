#pragma once
#include <queue>
#include <vector>

using namespace std;

class Dendrite;

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
	//in this case priority_queue should be ordered by actVal
	Aqueue(double aVal, Dendrite* dendrite) {
		this->actVal = aVal;
		this->dendrite = dendrite;
		this->pos = 0;
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

inline bool operator<(const Aqueue &a, const Aqueue &b) {
	if (a.retMode()) return a.getPos() > b.getPos();
	else return a.getPos() < b.getPos();
}

class Layer;

class ActivationQueue {
	private:
	priority_queue<Aqueue> q;
	vector<Dendrite*> queue;
	Layer *layer;
	public:
	ActivationQueue (Layer*);
	void schedActivation (Dendrite* , int);
	void schedActivation (Dendrite* , int, double);

	void activate (void);
	bool isEmpty (void);
	//unsigned int step;

};
