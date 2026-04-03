; Inno Setup script for Color Cop

[Setup]
AppID={{197A931D-2802-405D-B53E-67DF09D5BE2E}}
AppName=Color Cop
AppVersion=5.5.6
AppVerName=Color Cop 5.5.6
AppPublisher=Jay Prall
AppPublisherURL=https://colorcop.net
AppSupportURL=https://colorcop.net
AppUpdatesURL=https://colorcop.net
DefaultDirName={pf}\Color Cop
DefaultGroupName=Color Cop
LicenseFile=..\LICENSE.txt
InfoBeforeFile=..\ReadMe.txt
OutputDir=..\Output
OutputBaseFilename=colorcop-setup
SetupIconFile=..\Res\idr_main.ico
Compression=lzma
SolidCompression=yes
VersionInfoVersion=5.5.6
DisableDirPage=yes
DisableProgramGroupPage=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; Flags: unchecked

[Files]
Source: "..\ReadMe.txt"; DestDir: "{app}"
Source: "..\Release\ColorCop.exe"; DestDir: "{app}"
Source: "..\packaging\ColorCop.chm"; DestDir: "{app}"
Source: "..\LICENSE.txt"; DestDir: "{app}"

; --- VC++ Runtime bundling ---
; Place vc_redist.x86.exe next to this .iss file (in /installer)
Source: "vc_redist.x86.exe"; DestDir: "{tmp}"; Flags: deleteafterinstall

[INI]
Filename: "{app}\ColorCop.url"; Section: "InternetShortcut"; Key: "URL"; String: "https://colorcop.net/"

[Icons]
Name: "{group}\Color Cop"; Filename: "{app}\ColorCop.exe"
Name: "{group}\Website"; Filename: "{app}\ColorCop.url"
Name: "{userdesktop}\Color Cop"; Filename: "{app}\ColorCop.exe"; Tasks: desktopicon

[Run]
; Install VC++ runtime if needed
Filename: "{tmp}\vc_redist.x86.exe"; \
    Parameters: "/install /quiet /norestart"; \
    StatusMsg: "Installing Microsoft Visual C++ Runtime..."; \
    Check: NeedsVC2015Runtime

Filename: "{app}\ColorCop.exe"; Description: "{cm:LaunchProgram,Color Cop}"; Flags: nowait postinstall skipifsilent

[UninstallDelete]
Type: files; Name: "{app}\ColorCop.url"

[Code]
function NeedsVC2015Runtime: Boolean;
var
  installed: Cardinal;
begin
  // Microsoft’s official detection key for the 2015–2022 VC++ runtime
  Result := not RegQueryDWordValue(
    HKLM,
    'SOFTWARE\Microsoft\VisualStudio\14.0\VC\Runtimes\x86',
    'Installed',
    installed
  ) or (installed <> 1);
end;
