@REM �� ������ �̿��Ͻ� ��� �� �Ʒ��� ��ó�� ǥ�õǵ��� ���ּ���.
@echo off
setlocal enabledelayedexpansion
echo -----------------------
echo Spectate by op.gg
echo -----------------------
set RADS_PATH=
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKCU\SOFTWARE\RIOT GAMES\RADS" /v "LOCALROOTFOLDER"`) DO  (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKCU\SOFTWARE\Classes\VirtualStore\MACHINE\SOFTWARE\Wow6432Node\RIOT GAMES\RADS" /v "LOCALROOTFOLDER"`) DO (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKCU\SOFTWARE\Classes\VirtualStore\MACHINE\SOFTWARE\RIOT GAMES\RADS" /v "LOCALROOTFOLDER"`) DO (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKLM\Software\Wow6432Node\Riot Games\RADS" /v "LOCALROOTFOLDER"`) DO (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKLM\Software\Wow6432Node\Riot Games\RADS" /v "LOCALROOTFOLDER"`) DO (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKCU\SOFTWARE\RIOT GAMES\RADS" /v "LOCALROOTFOLDER"`) DO (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
FOR /f "usebackq skip=2 tokens=3,4,5,6,7,8,9" %%i in (`%systemroot%\system32\REG.EXE QUERY "HKLM\SOFTWARE\RIOT GAMES\RADS" /v "LOCALROOTFOLDER"`) DO (
	SET RADS_PATH=%%i %%j %%k %%l %%m %%n %%o
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� ã�� ���Դϴ�.
echo EN: Finding other LOL directory path..
echo ===================
for /f "Tokens=3,4,5,6,7,8,9,10,11,12,13,14,15" %%a in ('%systemroot%\system32\REG.EXE Query HKLM\Software /V /F "LocalRootFolder" /S /E ^| %systemroot%\system32\find.exe "RADS"') do (
	set RADS_PATH=%%a %%b %%c %%d %%e %%f %%g %%h %%i %%j %%k %%l %%m
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� �ٸ���ο��� ã�� ���Դϴ�.
echo EN: Finding another LOL directory path..
echo ===================
for /f "Tokens=3,4,5,6,7,8,9,10,11,12,13,14,15" %%a in ('%systemroot%\system32\REG.EXE Query HKLM\Software /s ^| %systemroot%\system32\find.exe "LocalRootFolder" ^| %systemroot%\system32\find.exe "RADS"') do (
	set RADS_PATH=%%a %%b %%c %%d %%e %%f %%g %%h %%i %%j %%k %%l %%m
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� �ٸ���ο��� ã�� ���Դϴ�.
echo EN: Finding another LOL directory path..
echo ===================
for /f "Tokens=3,4,5,6,7,8,9,10,11,12,13,14,15" %%a in ('%systemroot%\system32\REG.EXE Query HKCU\Software /V /F "LocalRootFolder" /S /E ^| %systemroot%\system32\find.exe "RADS"') do (
	set RADS_PATH=%%a %%b %%c %%d %%e %%f %%g %%h %%i %%j %%k %%l %%m
	goto runApp
)
cls
echo ===================
echo KR: ���� ��θ� �ٸ���ο��� ã�� ���Դϴ�.
echo EN: Finding another LOL directory path again..
echo ===================
for /f "Tokens=3,4,5,6,7,8,9,10,11,12,13,14,15" %%a in ('%systemroot%\system32\REG.EXE Query HKCU\Software /s ^| %systemroot%\system32\find.exe "LocalRootFolder" ^| %systemroot%\system32\find.exe "RADS"') do (
	set RADS_PATH=%%a %%b %%c %%d %%e %%f %%g %%h %%i %%j %%k %%l %%m
	goto runApp
)
cls
goto cannotFind
:runApp
set RADS_PATH=%RADS_PATH:/=\%
@cd /d "%RADS_PATH%\solutions\lol_game_client_sln\releases"

set init=0
set v0=0&set v1=0&set v2=0&set v3=0
for /f "delims=" %%F in ('dir *.*.*.* /b') do (
	for /F "tokens=1,2,3,4 delims=." %%i in ("%%F") do (
		if !init! equ 0 ( set init=1&set flag=1 ) else (
			set flag=0
			
			if %%i gtr !v0! ( set flag=1 ) else (
				if %%j gtr !v1! ( set flag=1 ) else (
					if %%k gtr !v2! ( set flag=1 ) else (
						if %%l gtr !v3! ( set flag=1 )
					)
				)
			)
		)
		
		if !flag! gtr 0 (
			set v0=%%i&set v1=%%j&set v2=%%k&set v3=%%l
		)
	)
)

if !init! equ 0 goto cannotFind
set lolver=!v0!.!v1!.!v2!.!v3!

@cd /d "!RADS_PATH!\solutions\lol_game_client_sln\releases\!lolver!\deploy"
if exist "League of Legends.exe" (
	@start "" "League of Legends.exe" "8394" "LoLLauncher.exe" "" "spectator 20001.f.spectator.op.gg:80 7qcKufjigt4UWOugchgLzQX2O6Kd4muZ 2354180388 EUW1-%RANDOM%%RANDOM%"
	goto exit
)
:cannotFind
echo ===================
echo KR: LOL ��θ� �ڵ����� ã�� �� �����ϴ�. ���򸻿��� �����ϱ� ���� �ذ��� ���ø� 100% �ذ� �� �� �ֽ��ϴ�. 100% �ذ� �� �� ������, ä�ù濡�� ���� �缭 �����ؼ� �����������!!!!!!!
echo EN: Cannot found LOL directory path for automatic. Please see our spectate help page: http://euw.op.gg/help/observer
echo ===================
@pause
goto exit
:exit