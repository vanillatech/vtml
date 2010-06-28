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
	for (int i=1; i<=10; ++i) {
		comboBox1->Items->Add(i);
		m_log->push_back(TDebugLogLayer());
	}
	comboBox1->SelectedIndex = 0;
	comboBox1->SelectedIndexChanged += gcnew EventHandler(this, &Form1::comboBox_Changed);
	m_sense->init();
}

System::Void Form1::comboBox_Changed(System::Object^  sender, System::EventArgs^  e)
{
	int index = comboBox1->SelectedIndex;
	if (index != m_currentIndex) {
		if (index < (int)m_log->size()) {
			switchLayer(index);
		}
		else
			comboBox1->SelectedIndex = m_currentIndex;
	}
}

void Form1::switchLayer(int index)
{
	m_currentIndex = index;
	richTextBox1->Clear();
	const TDebugLogLayer& layer = m_log->at(index);
	for (size_t i=0; i<layer.size(); ++i) {
		drawLogItem(layer[i].first, layer[i].second);
	}
	richTextBox1->SelectionStart = richTextBox1->Text->Length;
	richTextBox1->ScrollToCaret();
}

void Form1::drawLogItem(int messageType, const std::string& msg)
{
	Color color = Color::Black;
	switch (messageType) {
		case MSG_STEP_COUNTER:
			color = Color::Black;
			break;
		case MSG_NEW_INPUT_NEURON:
			color = Color::Green;
			break;
		case MSG_NEW_LINK:
			color = Color::DarkCyan;
			break;
		case MSG_NEURON_ACTIVATE:
			color = Color::DarkRed;
			break;
		case MSG_NEURON_FIRE:
			color = Color::Red;
			break;
		case MSG_ACTIVATION_SCHEDULED:
			color = Color::DarkRed;
			break;
		case MSG_PREDICT_NEXT:
			color = Color::DarkViolet;
			break;
		case MSG_PROPAGATE_DOWN:
			color = Color::Violet;
			break;
		case MSG_INHIBIT:
			color = Color::DarkGray;
			break;
		case MSG_DELETE_FROM_RQ:
			color = Color::DarkBlue;
			break;
		case MSG_NEW_OUTPUT:
			color = Color::Orange;
			break;
	}

	int pos = richTextBox1->Text->Length;
	richTextBox1->AppendText(gcnew System::String(msg.c_str()));
	richTextBox1->AppendText("\n");
	int pos1 = richTextBox1->Text->Length;

	richTextBox1->SelectionStart = pos;
	richTextBox1->Select(pos, pos1-pos);
	richTextBox1->SelectionColor = color;
	richTextBox1->SelectionLength = 0;
}

void Form1::OnCallback(Odin::ICallbackMsg* msg)
{
	if (!msg)
		return;

	int layer = msg->layer();
	if (layer <= 0 || layer > (int)m_log->size()) {
		delete msg;
		return;
	}

	int type = msg->type();
	std::string text = msg->message();
	(*m_log)[layer-1].push_back(std::make_pair(type, text));

	if (m_currentIndex == layer-1) {
		drawLogItem(type, msg->message());
		richTextBox1->SelectionStart = richTextBox1->Text->Length;
		richTextBox1->ScrollToCaret();
	}

	if (type == MSG_NEW_INPUT_NEURON) {
		CallbackMsg<MSG_NEW_INPUT_NEURON>* m = (CallbackMsg<MSG_NEW_INPUT_NEURON>*)msg;
		insertTreeItem("", m->id, m->value);
	}
	else if (type == MSG_NEW_LINK) {
		CallbackMsg<MSG_NEW_LINK>* m = (CallbackMsg<MSG_NEW_LINK>*)msg;
		insertTreeItem(m->fromId, m->toId, "");
	}
	delete msg;
}

/*
void Form1::OnCallback1(int code, const char* param1, const char* param2, const char* param3)
{
	addLogItem(code, param1);

	if (code == MSG_NEW_INPUT_NEURON) {
		insertTreeItem("", param2, param3);
	}
	else if (code == MSG_NEW_LINK) {
		insertTreeItem(param2, param3, "");
	}
}
*/

void Form1::insertTreeItem(const std::string& parentID, const std::string& ID, const std::string& pattern)
{
	char buf[256];
	sprintf_s(buf, 255, "%s ('%s')", ID.c_str(), pattern.c_str());
	buf[255] = 0;

	if (parentID.empty()) {
		TTreeSync::iterator it = m_treeSync->find(ID);
		if (it != m_treeSync->end())
			return;

		// adding item to the root of treeview
		THandleSet hs;
		TTreeItemIndex index;
		index.push_back(treeView1->Nodes->Count);
		hs.insert(index);
		m_treeSync->insert(std::make_pair(ID, hs));
		treeView1->Nodes->Add(gcnew System::String(buf));
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
		int lastIndex = doInsert(*h, buf);
		TTreeItemIndex index(*h);
		index.push_back(lastIndex);
		it->second.insert(index);
	}
}

int Form1::doInsert(const TTreeItemIndex& parent, const std::string& value)
{
	TreeNodeCollection^ nodes = treeView1->Nodes;
	for (size_t i=0; i<parent.size(); ++i) {
		nodes = nodes[parent[i]]->Nodes;
	}
	int ret = nodes->Count;
	nodes->Add(gcnew System::String(value.c_str()));
	return ret;
}

System::Void Form1::button1_Click(System::Object^  sender, System::EventArgs^  e)
{
	onGo();
}

System::Void Form1::textBox1_Key(System::Object^  sender, KeyPressEventArgs^  e) 
{
	if (e->KeyChar == 13) {
		onGo();
	}
	e->Handled = false;
}

void Form1::onGo()
{
	//if (!m_initialized) {
	//	m_sense->init();
	//	m_initialized = true;
	//}
	System::String^ text = textBox1->Text;
	for (int i=0; i<text->Length; ++i) {
		m_sense->input(text[i] & 0xFF);
		m_sense->nextStep();
	}
	m_sense->nextStep();
	textBox1->Text = "";
}

}
