object Bootloader: TBootloader
  Left = 589
  Top = 194
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'PIC32 Bootloader'
  ClientHeight = 389
  ClientWidth = 279
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 120
  TextHeight = 16
  object Memo: TMemo
    Left = 8
    Top = 164
    Width = 263
    Height = 217
    Lines.Strings = (
      '')
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object ProgressBar: TProgressBar
    Left = 8
    Top = 128
    Width = 263
    Height = 20
    Min = 0
    Max = 100
    TabOrder = 1
  end
  object BVer: TButton
    Left = 104
    Top = 16
    Width = 75
    Height = 25
    Caption = ' Ve&rsion'
    Enabled = False
    TabOrder = 2
    OnClick = BVerClick
  end
  object BHex: TButton
    Left = 196
    Top = 16
    Width = 75
    Height = 25
    Caption = '&Hex file'
    Enabled = False
    TabOrder = 3
    OnClick = BHexClick
  end
  object BConnect: TButton
    Left = 8
    Top = 16
    Width = 77
    Height = 25
    Caption = '&Connect'
    TabOrder = 4
    OnClick = BConnectClick
  end
  object BErase: TButton
    Left = 8
    Top = 52
    Width = 77
    Height = 25
    Caption = '&Erase'
    Enabled = False
    TabOrder = 5
    OnClick = BEraseClick
  end
  object BProgram: TButton
    Left = 104
    Top = 52
    Width = 75
    Height = 25
    Caption = '&Program'
    Enabled = False
    TabOrder = 6
    OnClick = BProgramClick
  end
  object BVerify: TButton
    Left = 196
    Top = 52
    Width = 75
    Height = 25
    Caption = '&Verify'
    Enabled = False
    TabOrder = 7
    OnClick = BVerifyClick
  end
  object BAll: TButton
    Left = 8
    Top = 88
    Width = 171
    Height = 25
    Caption = 'Er&ase-Program-Verify'
    Enabled = False
    TabOrder = 8
    OnClick = BAllClick
  end
  object BRun: TButton
    Left = 196
    Top = 88
    Width = 75
    Height = 25
    Caption = 'R&un'
    Enabled = False
    TabOrder = 9
    OnClick = BRunClick
  end
  object Timer: TTimer
    Enabled = False
    Interval = 10
    OnTimer = TimerTimer
    Left = 148
    Top = 188
  end
  object OpenDialog: TOpenDialog
    Left = 92
    Top = 188
  end
end
