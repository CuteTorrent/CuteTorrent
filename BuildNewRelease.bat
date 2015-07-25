call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86 || goto:errorEnd
python src\Build_Incrementer.py || goto:errorEnd
msbuild CuteTorrentVS2013.sln /t:Rebuild /p:Configuration=Release || goto:errorEnd
xcopy Win32\Release Setup\data\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt || goto:errorEnd
xcopy cutetorrent-search-engines Setup\data\searchEngines\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt || goto:errorEnd
xcopy cutetorrent-styles Setup\data\styles\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt || goto:errorEnd
xcopy webControll Setup\data\webControll\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt || goto:errorEnd
iscc Setup\CuteTorrent_Setup.iss || goto:errorEnd
:end
goto:eof

:errorEnd
echo ErrorLevel: %ERRORLEVEL%
echo "Error occured"
goto:eof