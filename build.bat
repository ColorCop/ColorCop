@echo on

vcvars32.bat

"C:\Program Files\Microsoft Visual Studio\Common\IDE\IDE98\devenv.exe" ./htmlcop.dsw /REBUILD Debug /out foo.txt
pause
