
.\codesign\signtool.exe sign /a /q .\Release\ColorCop.exe

copy .\Release\ColorCop.exe .\packaging\colorcop\

"C:\Program Files\Inno Setup 5\ISCC" inno\colorcop.iss

.\codesign\signtool.exe sign /a /q .\packaging\colorcop\colorcop-setup.exe

pause