; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

#define MyAppName "g6-2048"
#define MyAppVersion "1.0.0"
#define MyAppExeName "g6-2048.exe"
#define MyAppIcoName "g6-2048.ico"
#define MyAppDependDir1 "C:\mingw64\bin"
;#define MyAppDependDir2 "C:\msys64\usr\bin"
#define MyTmpDir "msi"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{DEC35FBB-5CCC-45E6-B104-0A17EC42081C}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
DefaultDirName={pf}\{#MyAppName}
DisableProgramGroupPage=yesLicenseFile=..\LICENSE
OutputDir={#MyTmpDir}
OutputBaseFilename={#MyAppName}-v{#MyAppVersion}-setup.exe
SetupIconFile={#MyAppIcoName}
Compression=lzma
SolidCompression=yes

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgtk-win32-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgobject-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgthread-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgdk-win32-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libatk-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libglib-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libcairo-2.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgdk_pixbuf-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libintl-8.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgio-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libgmodule-2.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libpango-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libpangocairo-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libpangowin32-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libfontconfig-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libfreetype-6.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libpangoft2-1.0-0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libexpat-1.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\libpng14-14.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppDependDir1}\zlib1.dll"; DestDir: "{app}"; Flags: ignoreversion
;Source: "{#MyAppDependDir2}\msys-2.0.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "{#MyAppIcoName}"; DestDir: "{app}"; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: "{commonprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppIcoName}"
Name: "{commondesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; IconFilename: "{app}\{#MyAppIcoName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent

