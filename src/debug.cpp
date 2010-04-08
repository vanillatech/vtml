//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "debug.h"
#include "SDIMAIN.h"

//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TDebug1 *Debug1;
//--------------------------------------------------------------------- 
__fastcall TDebug1::TDebug1(TComponent* AOwner)
	: TForm(AOwner)
{
	refreshTT();
}
//---------------------------------------------------------------------

void __fastcall TDebug1::OKButtonClick(TObject *Sender)
{
	SDIAppForm->DebugOptionClick(this);
}
//---------------------------------------------------------------------------
TTreeNode * __fastcall TDebug1::addLink(AnsiString n, TTreeNode *callingNode) {
	TTreeNode *tn=TreeView1->Items->AddChild(callingNode,n);
	return (tn);
}

TTreeNode * __fastcall TDebug1::addLink(AnsiString n) {
	TTreeNode *tn=TreeView1->Items->Add(TreeView1->TopItem,n);
	return (tn);
}
void __fastcall TDebug1::refreshTT(void) {
	TreeView1->Items = SDIAppForm->TreeView1->Items;
}
void __fastcall TDebug1::FormResize(TObject *Sender)
{
	TreeView1->Width=Debug1->Width-22;
	TreeView1->Height=Debug1->Height-73;
	ListBox1->Width=Debug1->Width-22;
	ListBox1->Height=Debug1->Height-73;
	OKButton->Top=Debug1->Height - 59;
	RadioButton1->Top=Debug1->Height - 59;
	RadioButton2->Top=Debug1->Height - 59;
}
//---------------------------------------------------------------------------


void __fastcall TDebug1::FormClose(TObject *Sender, TCloseAction &Action)
{
	SDIAppForm->DebugOptionClick(this);		
}
//---------------------------------------------------------------------------

void __fastcall TDebug1::DrawItem(TWinControl *Control, int Index, TRect &Rect,
      TOwnerDrawState State)
{
	if (ListBox1->Items->Strings[Index].Pos("Neuron fired:")) {
		ListBox1->Canvas->Font->Color = clRed;
	} else if (ListBox1->Items->Strings[Index].Pos("NewLink:")) {
		ListBox1->Canvas->Font->Color = clGreen;
	} else  if (ListBox1->Items->Strings[Index].Pos("Inhibit:")) {
		ListBox1->Canvas->Font->Color = clYellow;
	} else  if (ListBox1->Items->Strings[Index].Pos("Activate Neuron:")) {
		ListBox1->Canvas->Font->Color = clBlue;
	} else  if (ListBox1->Items->Strings[Index].Pos("Schedule Activation:")) {
		ListBox1->Canvas->Font->Color = clBlue;
	} else  if (ListBox1->Items->Strings[Index].Pos("PropagateDown:")) {
		ListBox1->Canvas->Font->Color = clGray;
	}
	ListBox1->Canvas->TextRect(Rect, Rect.Left, Rect.Top,
							 ListBox1->Items->Strings[Index]);
	
}
//---------------------------------------------------------------------------

void __fastcall TDebug1::RadioButton2Click(TObject *Sender)
{
	ListBox1->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TDebug1::RadioButton1Click(TObject *Sender)
{
	ListBox1->Visible = true;	
}
//---------------------------------------------------------------------------

