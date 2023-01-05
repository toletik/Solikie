@echo off
cls

set ProjectName=Solikie
set UEVersion=UnrealEngine-4.27.2

echo Cleaning up UE4 project

del *.sln>NUL 2>NUL
del *.db>NUL 2>NUL
del *.log>NUL 2>NUL
rmdir /s /Q .vs>NUL 2>NUL
rmdir /s /Q .idea>NUL 2>NUL
rmdir /S /Q Binaries>NUL 2>NUL
rmdir /S /Q Intermediate>NUL 2>NUL
rmdir /S /Q Saved>NUL 2>NUL
rmdir /S /Q Coverage>NUL 2>NUL
rmdir /S /Q DerivedDataCache>NUL 2>NUL

pushd Plugins

for /D %%i in (*) do ( 
	pushd %%i
	rmdir /S /Q Binaries>NUL 2>NUL
	rmdir /S /Q Intermediate>NUL 2>NUL
	popd
)

popd

@echo Generate project file
pause