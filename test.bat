cd build
nmake
nmake package

cd C:\Users\mo\AppData\Roaming
rmdir /s /Q openbible
xcopy /Q /E openbible_default openbible

cd C:\Users\mo\coding\git-openBibleViewer\build\_CPack_Packages\win32\NSIS\openBibleViewer-0.5.80-win32\bin
openBibleViewer.exe
cd C:\Users\mo\coding\git-openBibleViewer\