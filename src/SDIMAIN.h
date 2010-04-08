//----------------------------------------------------------------------------
#ifndef SDIMainH
#define SDIMainH
//----------------------------------------------------------------------------
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <StdCtrls.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <Controls.hpp>
#include <Forms.hpp>
#include <Graphics.hpp>
#include <Classes.hpp>
#include <Windows.hpp>
#include <System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
//----------------------------------------------------------------------------
class TSDIAppForm : public TForm
{
__published:
    TStatusBar *StatusBar;
	TMainMenu *MainMenu1;
	TMenuItem *Help1;
	TMenuItem *HelpAboutItem;
	TActionList *ActionList1;
	TAction *HelpAbout1;
	TMenuItem *Datei1;
	TMenuItem *Einstellungen1;
	TEdit *Edit1;
	TButton *Button1;
	TLabel *Label1;
	TMenuItem *Beenden1;
	TMenuItem *N1;
	TMenuItem *DebugOption;
	TTreeView *TreeView1;
	TTimer *Timer1;
		void __fastcall HelpAbout1Execute(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Beenden1Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall DebugOptionClick(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
private:
public:
	virtual __fastcall TSDIAppForm(TComponent *AOwner);
	TTreeNode * __fastcall addLink(AnsiString);
	TTreeNode * __fastcall addLink(AnsiString, TTreeNode*);
};
//----------------------------------------------------------------------------
extern TSDIAppForm *SDIAppForm;
//----------------------------------------------------------------------------
#endif
