pushd "%CD%"
CD /D "%~dp0"
netsh advfirewall set  currentprofile state off