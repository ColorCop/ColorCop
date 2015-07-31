echo killcolorcop.bat

del listoutput.txt
tasklist /FI "IMAGENAME eq colorcop.exe" > listoutput.txt

del foundcolorcop.txt
findstr ColorCop.ex listoutput.txt > foundcolorcop.txt

rem cmd /c taskkill /F /IM colorcop.exe /T