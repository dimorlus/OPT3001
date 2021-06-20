object OPT: TOPT
  Left = 584
  Top = 332
  BorderStyle = bsDialog
  Caption = 'Light meter'
  ClientHeight = 325
  ClientWidth = 473
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnKeyPress = FormKeyPress
  OnShow = FormShow
  DesignSize = (
    473
    325)
  PixelsPerInch = 120
  TextHeight = 16
  object Bevel: TBevel
    Left = 32
    Top = 144
    Width = 406
    Height = 147
    Anchors = [akLeft, akTop, akRight]
    Style = bsRaised
  end
  object LConnect: TLabel
    Left = 32
    Top = 16
    Width = 3
    Height = 16
  end
  object LVer: TLabel
    Left = 32
    Top = 40
    Width = 3
    Height = 16
  end
  object Led: TLabel
    Left = 32
    Top = 108
    Width = 99
    Height = 16
    Caption = 'Lumen multiplier:'
  end
  object Llux0: TLabel
    Left = 88
    Top = 164
    Width = 12
    Height = 40
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Llum0: TLabel
    Left = 88
    Top = 224
    Width = 12
    Height = 40
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LEdOk: TLabel
    Left = 284
    Top = 108
    Width = 3
    Height = 16
  end
  object LMaster: TLabel
    Left = 32
    Top = 76
    Width = 100
    Height = 16
    Caption = 'Master multiplier:'
  end
  object LMOk: TLabel
    Left = 284
    Top = 76
    Width = 3
    Height = 16
  end
  object EdKlux: TEdit
    Left = 144
    Top = 104
    Width = 121
    Height = 24
    TabOrder = 0
    OnKeyPress = EdKluxKeyPress
  end
  object EdMaster: TEdit
    Left = 144
    Top = 72
    Width = 121
    Height = 24
    TabOrder = 1
    OnKeyPress = EdMasterKeyPress
  end
  object BBldr: TButton
    Left = 344
    Top = 104
    Width = 87
    Height = 25
    Caption = '&Bootloader'
    TabOrder = 2
    OnClick = BBldrClick
  end
  object Timer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = TimerTimer
    Left = 372
    Top = 48
  end
end
