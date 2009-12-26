@echo off
echo Ctags build...
ctags -f tags -R . c:\wxWidgets-2.8.7\include
echo Ctags done.
echo Cscope begin:
echo    Search *.h
find . c:\wxWidgets-2.8.7\include -name *.h -type f > cscope.files
echo    Search *.hpp
find . -name *.hpp -type f >> cscope.files
echo    Search *.cpp
find . -name *.cpp -type f >> cscope.files
echo    Search done.
cscope -bq
echo Cscope done.
