; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "controlrobots"
#define MyAppCheseName "controlrobots"
#define MyAppVersion "2.0.1"
#define MyAppPublisher "UBTECH"
#define MyAppURL "http://www.ubtrobot.com/"
#define MyAppExeName "controlrobots.exe"
#define BasePath "..\bin\win32\release"
#define OutputName "AlphaMini-controlrobots-x32-Setup-"
#define MyDateTimeString GetDateTimeString('yyyymmddhhnnss', '', '')
#define MyCompanyName "UBTECH"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{5E47597D-A139-4EB0-80B7-5B838FF102A2}
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
OutputDir=.\win32
OutputBaseFilename={#OutputName}{#MyDateTimeString}
SetupIconFile=.\wukong.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"
Name: "chinesesimplified"; MessagesFile: "compiler:Languages\ChineseSimplified.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: checkablealone

[Files]
Source: "{#BasePath}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BasePath}\*.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#BasePath}\Config\*.*"; DestDir: "{app}\Config"; Flags: ignoreversionSource: "{#BasePath}\Config\actionlist\*.*"; DestDir: "{app}\Config\actionlist"; Flags: ignoreversion
Source: "{#BasePath}\platforms\*.*"; DestDir: "{app}\platforms"; Flags: ignoreversion
Source: "{#BasePath}\Config\music\*.*"; DestDir: "{app}\Config\music"; Flags: ignoreversion

; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

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
      // 指定新的卸载文件名（不包含扩展名），请相应修改！
      NewUninsName := 'uninst';
      ApplicationName := '{#MyAppName}';
      // 以下重命名卸载文件
      uninspath:= ExtractFilePath(ExpandConstant('{uninstallexe}'));
      uninsname:= Copy(ExtractFileName(ExpandConstant('{uninstallexe}')),1,8);
      deleteFile(uninspath + NewUninsName + '.exe');
      deleteFile(uninspath + NewUninsName + '.dat');
      RenameFile(uninspath + uninsname + '.exe', uninspath + NewUninsName + '.exe');
      RenameFile(uninspath + uninsname + '.dat', uninspath + NewUninsName + '.dat');
      // 以下修改相应的注册表内容
      if RegKeyExists(HKEY_LOCAL_MACHINE, 'SOFTWARE/Microsoft/Windows/CurrentVersion/Uninstall/' + ApplicationName + '_is1') then
      begin
         RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE/Microsoft/Windows/CurrentVersion/Uninstall/' + ApplicationName + '_is1', 'UninstallString', '"' + uninspath + NewUninsName + '.exe"');
         RegWriteStringValue(HKEY_LOCAL_MACHINE, 'SOFTWARE/Microsoft/Windows/CurrentVersion/Uninstall/' + ApplicationName + '_is1', 'QuietUninstallString', '"' + uninspath + NewUninsName + '.exe" /SILENT');
      end;
  end;
end;