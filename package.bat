rmdir /S /Q Release-standalone
mkdir Release-standalone

copy Release\*.exe Release-standalone
copy README.txt Release-standalone
copy packagine\*.txt Release-standalone
copy packaging\*.diz Release-standalone
copy packaging\*.HLP Release-standalone


cd Release-standalone

"C:\Program Files\WinRAR\WinRAR.exe" a -afzip colorcop.zip *

pause
