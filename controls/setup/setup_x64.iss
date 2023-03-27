; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "controlrobots"
#define MyAppCheseName "controlrobots"
#define MyAppVersion "2.0.1"
#define MyAppPublisher "UBTECH"
#define MyAppURL "http://www.ubtrobot.com/"
#define MyAppExeName "controlrobots.exe"
#define BasePath "..\bin\win64\release"
#define OutputName "AlphaMini-controlrobots-x64-Setup-"
#define MyDateTimeString GetDateTimeString('yyyymmddhhnnss', '', '')
#define MyCompanyName "UBTECH"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{56498BFE-E453-42C2-98F6-2BDD82EA0FE3}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={pf}\{#MyCompanyName}\{#MyAppCheseName}
DefaultGroupName={#MyCompanyName}\{#MyAppCheseName}
DisableProgramGroupPage=yes
OutputDir=.\win64
OutputBaseFilename={#OutputName}{#MyDateTimeString}
SetupIconFile=.\wukong.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#BasePath}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BasePath}\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BasePath}\Config\*.*"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "{#BasePath}\Config\actionlist\*.*"; DestDir: "{app}\Config\actionlist"; Flags: ignoreversion
Source: "{#BasePath}\platforms\*.*"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#BasePath}\Config\music\*.*"; DestDir: "{app}\Config\music"; Flags: ignoreversion

; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

[Icons]
Name: "{commondesktop}\{#MyAppCheseName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; Tasks: desktopicon
Name: "{group}\{#MyAppCheseName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{group}\{cm:UninstallProgram,{#MyAppCheseName}}"; Filename: "{app}/uninst.exe"

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

[CODE]                                        
procedure CurStepChanged(CurStep: TSetupStep);
var uninspath, uninsname, NewUninsName, ApplicationName : string;
begin
  if CurStep = ssDone then
  begin
      // ָ���µ�ж���ļ�������������չ����������Ӧ�޸ģ�
      NewUninsName := 'uninst';
      ApplicationName := '{#MyAppName}';
      // ����������ж���ļ�
      uninspath:= ExtractFilePath(ExpandConstant('{uninstallexe}'));
      uninsname:= Copy(ExtractFileName(ExpandConstant('{uninstallexe}')),1,8);
      deleteFile(uninspath + NewUninsName + '.exe');
      deleteFile(uninspath + NewUninsName + '.dat');
      RenameFile(uninspath + uninsname + '.exe', uninspath + NewUninsName + '.exe');
      RenameFile(uninspath + uninsname + '.dat', uninspath + NewUninsName + '.dat');
      // �����޸���Ӧ��ע�������
      if RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE/Microsoft/Windows/CurrentVersion/Uninstall/' + ApplicationName + '_is1') then
      begin
         RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE/Microsoft/Windows/CurrentVersion/Uninstall/' + ApplicationName + '_is1', 'UninstallString', '"' + uninspath + NewUninsName + '.exe"');
         RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE/Microsoft/Windows/CurrentVersion/Uninstall/' + ApplicationName + '_is1', 'QuietUninstallString', '"' + uninspath + NewUninsName + '.exe" /SILENT');
      end;
  end;
end;