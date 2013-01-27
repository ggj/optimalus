rmdir /s /q c:\temp\ggj13\
mkdir c:\temp\ggj13\

copy %SDL_HOME%\lib\x86\SDL.dll c:\temp\ggj13\
copy %OPENAL_HOME%\libs\Win32\*.dll c:\temp\ggj13\
copy %SDL_IMAGE_HOME%\lib\x86\*.dll c:\temp\ggj13\

xcopy /s ..\resources\*.* c:\temp\ggj13\

copy ..\project\vs2012\ggj13\Release\ggj13.exe c:\temp\ggj13\ 

