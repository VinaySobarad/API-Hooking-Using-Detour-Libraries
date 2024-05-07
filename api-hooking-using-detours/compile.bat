@ECHO OFF

cl.exe /nologo /W0 hooker.cpp /MT /link /DLL detours\lib.X64\detours.lib /OUT:hooker.dll

cl.exe /nologo /Ox /MT /W0 /GS- /DNDEBUG /Tp targetforhooker.cpp /link /OUT:targetforhooker.exe /SUBSYSTEM:CONSOLE
del *.obj *.lib *.exp