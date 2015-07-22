call "C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\vcvarsall.bat" x86

python src\Build_Incrementer.py

msbuild CuteTorrentVS2013.sln /t:Rebuild /p:Configuration=Release

xcopy Win32\Release Setup\data\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt
xcopy cutetorrent-search-engines Setup\data\searchEngines\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt
xcopy cutetorrent-styles Setup\data\styles\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt
xcopy webControll Setup\data\webControll\ /s/h/e/k/f/c/Y /EXCLUDE:xcopy-exlude-list.txt
iscc Setup\CuteTorrent_Setup.iss

pause