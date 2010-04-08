//----------------------------------------------------------------------------
#ifndef debugH
#define debugH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <ComCtrls.hpp>

//----------------------------------------------------------------------------
class TDebug1 : public TForm
{
__published:
	TButton *OKButton;
	TTreeView *TreeView1;
	TListBox *ListBox1;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	void __fastcall OKButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall DrawItem(TWinControl *Control, int Index, TRect &Rect,
          TOwnerDrawState State);
	void __fastcall RadioButton2Click(TObject *Sender);
	void __fastcall RadioButton1Click(TObject *Sender);
private:
public:
	virtual __fastcall TDebug1(TComponent* AOwner);
	TTreeNode * __fastcall addLink(AnsiString);
	TTreeNode * __fastcall addLink(AnsiString, TTreeNode*);
	void __fastcall refreshTT(void);
};
//----------------------------------------------------------------------------
extern PACKAGE TDebug1 *Debug1;
//----------------------------------------------------------------------------
#endif    
