; Inno Setup script for Color Cop

[Setup]
AppID={{197A931D-2802-405D-B53E-67DF09D5BE2E}}
AppName=Color Cop
AppVersion=5.5.3
AppVerName=Color Cop 5.5.3
AppPublisher=Jay Prall
AppPublisherURL=https://colorcop.net
AppSupportURL=https://colorcop.net
AppUpdatesURL=https://colorcop.net
DefaultDirName={pf}\Color Cop
DefaultGroupName=Color Cop
LicenseFile=..\LICENSE.TXT
InfoBeforeFile=..\ReadMe.txt
OutputDir=..\Output
OutputBaseFilename=colorcop-setup
SetupIconFile=..\Res\idr_main.ico
Compression=lzma
SolidCompression=yes
VersionInfoVersion=5.5.3
DisableDirPage=yes
DisableProgramGroupPage=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; Flags: unchecked

[Files]
Source: "..\readme.txt"; DestDir: "{app}"
Source: "..\Release\ColorCop.exe"; DestDir: "{app}"
Source: "..\packaging\ColorCop.chm"; DestDir: "{app}"
Source: "..\LICENSE.TXT"; DestDir: "{app}"

[INI]
Filename: "{app}\ColorCop.url"; Section: "InternetShortcut"; Key: "URL"; String: "https://colorcop.net/"

[Icons]
Name: "{group}\Color Cop"; Filename: "{app}\ColorCop.exe"
Name: "{group}\Website"; Filename: "{app}\ColorCop.url"
Name: "{userdesktop}\Color Cop"; Filename: "{app}\ColorCop.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\ColorCop.exe"; Description: "{cm:LaunchProgram,Color Cop}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\ColorCop.url"
