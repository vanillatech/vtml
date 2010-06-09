#include "stdafx.h"
#pragma unmanaged
#include "callbacks.h"
#include "prozess10/Sense.h"
#pragma managed
#include "Form1.h"

using namespace Odin;

namespace srcnew1
{

System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e) 
{
	m_sense->init();
}

void Form1::OnCallback(Odin::ICallbackMsg* msg)
{
	if (!msg)
		return;
	const char* s = msg->message().c_str();
	richTextBox1->Text->Concat(gcnew System::String(s));
	if (msg->type() == MSG_NEW_INPUT_NEURON) {
		CallbackMsg<MSG_NEW_INPUT_NEURON>* m = (CallbackMsg<MSG_NEW_INPUT_NEURON>*)msg;
		insertTreeItem("", m->id, m->value);
	}
	if (msg->type() == MSG_NEW_LINK) {
		CallbackMsg<MSG_NEW_LINK>* m = (CallbackMsg<MSG_NEW_LINK>*)msg;
		insertTreeItem(m->fromId, m->toId, "");
	}
	delete msg;
}

void Form1::insertTreeItem(const std::string& parentID, const std::string& ID, const std::string& pattern)
{
	char buf[256];
	sprintf_s(buf, 255, "%s ('%s')", ID, pattern.c_str());
	buf[255] = 0;

	if (parentID.empty()) {
		TTreeSync::iterator it = m_treeSync->find(ID);
		if (it != m_treeSync->end())
			return;

		treeView1->Nodes->Add(gcnew System::String(buf));
		m_treeSync->insert(std::make_pair(ID, THandleSet()));
		return;
	}

	TTreeSync::iterator pit = m_treeSync->find(parentID);
	if (pit == m_treeSync->end() || pit->second.empty())
		return;
	TTreeSync::iterator it = m_treeSync->find(ID);
	if (it == m_treeSync->end()) {
		it = m_treeSync->insert(std::make_pair(ID, THandleSet())).first;
	}
	THandleSet::iterator h = pit->second.begin();
	THandleSet::iterator end = pit->second.end();
	for (; h != end; ++h) {
		doInsert(*h, buf);
	}
}

void Form1::doInsert(const TTreeItemIndex& parent, const std::string& value)
{
	TreeNodeCollection^ nodes = treeView1->Nodes;
	for (size_t i=0; i<parent.size(); ++i) {
		nodes = nodes[i]->Nodes;
	}
	nodes->Add(gcnew System::String(value.c_str()));
}

System::Void Form1::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	System::String^ text = textBox1->Text;
	for (int i=0; i<text->Length; ++i) {
		m_sense->input(text[i] & 0xFF);
	}
	textBox1->Text = "";
}

}
