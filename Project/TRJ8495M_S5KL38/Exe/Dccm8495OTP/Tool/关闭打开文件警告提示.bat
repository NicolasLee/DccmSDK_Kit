@echo off
@echo ---请以管理员身份运行---
@echo ...
reg add HKCU\Software\Microsoft\Windows\CurrentVersion\Policies\Associations /v ModRiskFileTypes /t REG_SZ /d .exe;.bat;.vbs /f
gpupdate /force
exit