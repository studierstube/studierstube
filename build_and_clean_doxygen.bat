REM This batch file builds the documentation and opens the main page with explorer.
REM This batch file checks the existance of html directory.
REM if none it creates one.
REM Author: Michele Fiorenitno

echo %1

cd %STB4ROOT%\doc
set htmldir=doc\html

if %1==clean goto cleanup

if exist %htmldir%\*.* goto htmldirexists
mkdir %htmldir%
:htmldirexists

echo   Make sure you have installed doxygen.
echo   If not, download it from www.doxygen.org

doxygen.exe studierstube_v4.dox
echo Running doxygen (it may take a while)...
dir
explorer %htmldir%\index.html
pause
exit 0

: cleanup
REM check extension for security (do not calcels code!)
echo Cleaning (it may take a while)...
del %htmldir%\*.png
del %htmldir%\*.html
del %htmldir%\*.gif
del %htmldir%\*.css
del %htmldir%\*.md5
del %htmldir%\*.dot
echo Cleaning Done!

exit 0