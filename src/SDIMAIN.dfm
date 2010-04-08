object SDIAppForm: TSDIAppForm
  Left = 197
  Top = 111
  Caption = 'odin 0.0.20'
  ClientHeight = 248
  ClientWidth = 369
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'System'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 16
  object Label1: TLabel
    Left = 8
    Top = 56
    Width = 44
    Height = 16
    Caption = 'Label1'
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 229
    Width = 369
    Height = 19
    AutoHint = True
    Panels = <>
    SimplePanel = True
  end
  object Edit1: TEdit
    Left = 8
    Top = 16
    Width = 257
    Height = 24
    TabOrder = 1
    OnKeyPress = Edit1KeyPress
  end
  object Button1: TButton
    Left = 287
    Top = 16
    Width = 65
    Height = 25
    Caption = '&OK'
    TabOrder = 2
    OnClick = Button1Click
  end
  object TreeView1: TTreeView
    Left = 192
    Top = 144
    Width = 65
    Height = 49
    Indent = 19
    TabOrder = 3
    Visible = False
  end
  object MainMenu1: TMainMenu
    Left = 8
    Top = 152
    object Datei1: TMenuItem
      Caption = '&Datei'
      object Einstellungen1: TMenuItem
        Caption = '&Einstellungen'
      end
      object DebugOption: TMenuItem
        Caption = '&Debug'
        OnClick = DebugOptionClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object Beenden1: TMenuItem
        Caption = '&Beenden'
        OnClick = Beenden1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Hilfe'
      Hint = 'Hilfethemen'
      object HelpAboutItem: TMenuItem
        Action = HelpAbout1
      end
    end
  end
  object ActionList1: TActionList
    Left = 40
    Top = 152
    object HelpAbout1: TAction
      Category = 'Help'
      Caption = '&Info...'
      Hint = 
        'Info|Anzeige von Programminformationen, Versionsnummer und Copyr' +
        'ight-Vermerk'
      OnExecute = HelpAbout1Execute
    end
  end
  object Timer1: TTimer
    Interval = 500
    OnTimer = Timer1Timer
    Left = 72
    Top = 152
  end
end
