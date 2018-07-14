REM Python 3.6 64bit is required!
set PYTHON=c:/tools/Python36

IF NOT EXIST %PYTHON% (
    echo Python does not exists!.
	pause
	exit 1
)

IF EXIST ".venv" (
    rmdir /q /s .venv
)

%PYTHON%/python.exe -m venv .venv

IF NOT EXIST ".venv" (
	echo Python virtual environment faild to create.
	pause
	exit 1
)

call .venv\scripts\activate.bat
pip install -r requirements.txt
