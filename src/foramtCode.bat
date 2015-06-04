AStyle.exe --recursive --style=allman --indent-switches --delete-empty-lines --align-pointer=type --align-reference=type --add-brackets --max-code-length=200 --break-after-logical --mode=c  --pad-oper --break-blocks --indent=tab *.cpp *.h
del /S *.orig
pause