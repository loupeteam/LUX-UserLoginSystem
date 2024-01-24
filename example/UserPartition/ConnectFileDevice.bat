echo off
set Name="webHMI-UserLoginSystem"

mkdir C:\ARSimUser

rmdir "C:\ARSimUser\%Name%"


mklink /D "C:\ARSimUser\%Name%" "%~dp0"
