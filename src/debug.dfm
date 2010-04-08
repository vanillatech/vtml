object Debug1: TDebug1
  Left = 200
  Top = 108
  Caption = 'Debug'
  ClientHeight = 304
  ClientWidth = 530
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object OKButton: TButton
    Left = 8
    Top = 273
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
    OnClick = OKButtonClick
  end
  object TreeView1: TTreeView
    Left = 8
    Top = 8
    Width = 516
    Height = 259
    Indent = 19
    TabOrder = 1
  end
  object ListBox1: TListBox
    Left = 8
    Top = 8
    Width = 514
    Height = 259
    Style = lbOwnerDrawFixed
    ItemHeight = 13
    TabOrder = 2
    OnDrawItem = DrawItem
  end
  object RadioButton1: TRadioButton
    Left = 112
    Top = 273
    Width = 97
    Height = 23
    Caption = 'Action Dump'
    Checked = True
    TabOrder = 3
    TabStop = True
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 232
    Top = 273
    Width = 113
    Height = 23
    Caption = 'Tree View'
    TabOrder = 4
    OnClick = RadioButton2Click
  end
end
