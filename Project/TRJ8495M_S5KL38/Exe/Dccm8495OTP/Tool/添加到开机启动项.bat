echo off

echo 本地拷贝
echo 复制DccmServer.exe到开机启动项
xcopy "%~dp0DccmServer.exe" "C:\Users\Dayu\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\" /e /y
exit
