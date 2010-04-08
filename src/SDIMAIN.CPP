//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "SDIMain.h"
#include "About.h"
#include "odin.h"
#include "debug.h"

//---------------------------------------------------------------------
#pragma resource "*.dfm"
TSDIAppForm *SDIAppForm;



TTreeNode * __fastcall TSDIAppForm::addLink(AnsiString n, TTreeNode *callingNode) {
	TTreeNode *tn=TreeView1->Items->AddChild(callingNode,n);
	return (tn);
}

TTreeNode * __fastcall TSDIAppForm::addLink(AnsiString n) {
	TTreeNode *tn=TreeView1->Items->Add(TreeView1->TopItem,n);
	return (tn);
}
//---------------------------------------------------------------------
__fastcall TSDIAppForm::TSDIAppForm(TComponent *AOwner)
	: TForm(AOwner)
{
	 Label1->Caption = "";
	 readSense = new Sense(&aqueue, &recQueue);
	 recQueue[0].recover();
}
//---------------------------------------------------------------------


void __fastcall TSDIAppForm::HelpAbout1Execute(TObject *Sender)
{
  AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TSDIAppForm::Button1Click(TObject *Sender)
{
	AnsiString text1 = Edit1->Text;
	unsigned char *text1chr = text1.c_str();
	if (text1chr) {
		int m = strlen(text1chr);
		for (int n=0;n<m;n++ ) {
		   readSense->input(text1chr[n]);
		   //Read text char by char... Its not possible to read 2 chars
		   //simultaneously. So temporal separation is required.
		   aqueue.activate();
		   //for (unsigned int n=0;n<recQueue.size() ;n++ ) {
		   //		recQueue[n].recover();
		   //}
		   //***
		}
		//lastFiredNeuron->predictNext();

		aqueue.activate();
		for (unsigned int n=0;n<recQueue.size();n++ ) {
				recQueue[n].recover();
		}
		//--test
		//lastFiredNeuron->predictNext();
		aqueue.activate();
		for (unsigned int n=0;n<recQueue.size();n++ ) {
				recQueue[n].recover();
		}
		//--end test
	}
	Edit1->Text = "";
}
//---------------------------------------------------------------------------

void __fastcall TSDIAppForm::Beenden1Click(TObject *Sender)
{
exit(0);	
}
//---------------------------------------------------------------------------


void __fastcall TSDIAppForm::Timer1Timer(TObject *Sender)
{
/*if (!aqueue.isEmpty()) {
	aqueue.activate();
	for (unsigned int n=0;n<recQueue.size() ;n++ ) {
				recQueue[n].recover();
	}
} */
/*aqueue.activate();
for (unsigned int n=0;n<recQueue.size() ;n++ ) {

recQueue[n].recover();
} */
}


void __fastcall TSDIAppForm::DebugOptionClick(TObject *Sender)
{
	if (!DebugOption->Checked) {
    	
		Debug1->Show();
		DebugOption->Checked = true;
	} else {
		Debug1->Visible = false;
		DebugOption->Checked = false;
    }
	
}
//---------------------------------------------------------------------------

void __fastcall TSDIAppForm::Edit1KeyPress(TObject *Sender, char &Key)
{
	if (Key==13) {
		Label1->Caption = "";
		this->Button1Click(Sender);
	}
}
//---------------------------------------------------------------------------

