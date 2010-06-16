#pragma once
#include <string>

namespace Odin
{

//template <class TPattern> struct IModel
//{
//	virtual void input(const TPattern& pattern) = 0;
//};

typedef std::string TNeuronID;

struct ICallbackMsg;

struct ICallback
{
	//default callbacks do nothing... inherit.
	virtual void onCallback(ICallbackMsg* msg)
	{}
};

struct ICallbackMsg
{
	virtual ~ICallbackMsg() {}
	virtual std::string message() const = 0;

	int type() const {
		return m_type;
	}

	int layer() const {
		return m_layer;
	}

	ICallbackMsg(int _type, int _layer) : 
		m_type(_type),
		m_layer(_layer)
	{}
	
	private:
		int m_type;
		int m_layer;
};

template <int Type> struct CallbackMsg
{
};

enum ECallbackTypes {
	MSG_STEP_COUNTER,
	MSG_NEW_INPUT_NEURON,
	MSG_NEW_LINK,
	MSG_NEURON_ACTIVATE,
	MSG_NEURON_FIRE,
	MSG_ACTIVATION_SCHEDULED,
	MSG_PREDICT_NEXT,
	MSG_PROPAGATE_DOWN,
	MSG_INHIBIT,
	MSG_DELETE_FROM_RQ,
};

/*Debug1->ListBox1->Items->Insert
(0,AnsiString("----------- ") + AnsiString(stepCounter) + AnsiString("------------"));
*/

template <> struct CallbackMsg<MSG_STEP_COUNTER> : ICallbackMsg
{
	CallbackMsg(int _layer, int _step) : 
		ICallbackMsg(MSG_STEP_COUNTER, _layer), 
		step(_step)
	{}

	virtual std::string message() const;

	int step;
};

template <> struct CallbackMsg<MSG_NEW_INPUT_NEURON> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id, const std::string& _value) : 
		ICallbackMsg(MSG_NEW_INPUT_NEURON, _layer),
		id (_id),
		value(_value)
	{}

	virtual std::string message() const;

	int step;
	TNeuronID id;
	std::string value;
};

/*
AnsiString captionID = toNeuron->id;
		  captionID += " layer: ";
		  captionID += toNeuron->layer;
		  toNeuron->debugCallingNode = SDIAppForm->addLink(captionID,this->debugCallingNode);
		  Debug1->refreshTT();

		  Debug1->ListBox1->Items->Insert(0,"NewLink: " + id + " to " + toNeuron->id);
*/

template <> struct CallbackMsg<MSG_NEW_LINK> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _fromId, int _toLayer, const TNeuronID& _toId) : 
		ICallbackMsg(MSG_NEW_LINK, _layer),
		fromId(_fromId), toLayer(_toLayer), toId(_toId)
	{}

	virtual std::string message() const;

	TNeuronID fromId;
	int toLayer;
	TNeuronID toId;
};

/*Debug1->ListBox1->Items->Insert
(0,"Activate Neuron: " + id + " (Increase: " 
+ AnsiString (activationValNew)+"; activationVal: " + AnsiString(activationVal) + ") " );
*/

template <> struct CallbackMsg<MSG_NEURON_ACTIVATE> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id, float _actVal, float _increase)  : 
		ICallbackMsg(MSG_NEURON_ACTIVATE, _layer),
		id(_id), actVal(_actVal), increase(_increase)
	{}

	virtual std::string message() const;

	TNeuronID id;
	float actVal;
	float increase;
};

/*
if (this->outputData != 0) {
			SDIAppForm->Label1->Caption = SDIAppForm->Label1->Caption + this->outputData;
		  }
		  Debug1->ListBox1->Items->Insert(0,"Neuron fired: " + id + " ActVal: " + activationVal );
*/


template <> struct CallbackMsg<MSG_NEURON_FIRE> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id, float _actVal) : 
		ICallbackMsg(MSG_NEURON_FIRE, _layer),
		id(_id), actVal(_actVal)
	{}

	virtual std::string message() const;

	TNeuronID id;
	float actVal;
};

/*
Debug1->ListBox1->Items->Insert
(0,"Schedule Activation: " + axons[n]->dendriteTo->id + " in: " + AnsiString((*axons[n]).activationDelay) );
*/

template <> struct CallbackMsg<MSG_ACTIVATION_SCHEDULED> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id, int _delay) : 
		ICallbackMsg(MSG_ACTIVATION_SCHEDULED, _layer),
		id(_id), delay(_delay)
	{}

	virtual std::string message() const;

	TNeuronID id;
	int delay;
};


/*Debug1->ListBox1->Items->Insert(0,"PredictNext: " + this->id );*/

template <> struct CallbackMsg<MSG_PREDICT_NEXT> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id)  : 
		ICallbackMsg(MSG_PREDICT_NEXT, _layer),
		id(_id)
	{}

	virtual std::string message() const;

	TNeuronID id;
};

/*
int toutput = int(timeOffset);
	Debug1->ListBox1->Items->Insert(0,"PropagateDown: " + this->id + " timeoffset: " + AnsiString(toutput) );
*/

template <> struct CallbackMsg<MSG_PROPAGATE_DOWN> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id, int _timeOffset) : 
		ICallbackMsg(MSG_PROPAGATE_DOWN, _layer),
		id(_id), timeOffset(_timeOffset)
	{}

	virtual std::string message() const;

	TNeuronID id;
	int timeOffset;
};

/* Debug1->ListBox1->Items->Insert(0,"Inhibit: " + id );*/

template <> struct CallbackMsg<MSG_INHIBIT> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id)  : 
		ICallbackMsg(MSG_INHIBIT, _layer),
		id(_id)
	{}

	virtual std::string message() const;

	TNeuronID id;
};

/*Debug1->ListBox1->Items->Insert(0,"Delete from Rq: " + queue[forderNew][n]->id);*/

template <> struct CallbackMsg<MSG_DELETE_FROM_RQ> : ICallbackMsg
{
	CallbackMsg(int _layer, const TNeuronID& _id) : 
		ICallbackMsg(MSG_DELETE_FROM_RQ, _layer),
		id(_id)
	{}

	virtual std::string message() const;

	TNeuronID id;
};

} //Odin
