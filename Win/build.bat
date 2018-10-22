@ECHO OFF

REM
REM MIT License
REM
REM This file is part of easyOS.
REM You can find the software at https://github.com/djeasy84/easyOS
REM
REM Copyright (c) 2018 - Lorenzo Baglioni (djeasy84) <lbaglioni84@gmail.com>
REM
REM Permission is hereby granted, free of charge, to any person obtaining a copy
REM of this software and associated documentation files (the "Software"), to deal
REM in the Software without restriction, including without limitation the rights
REM to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
REM copies of the Software, and to permit persons to whom the Software is
REM furnished to do so, subject to the following conditions:
REM
REM The above copyright notice and this permission notice shall be included in all
REM copies or substantial portions of the Software.
REM
REM THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
REM IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
REM FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
REM AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
REM LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
REM OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
REM SOFTWARE.
REM
REM

GOTO :MAIN

:HELP
    ECHO:
    ECHO build command syntax:
    ECHO     build.bat [board] [project] {COM} {NOBOOTLOADER}
    ECHO:
    ECHO     Mandatory fields:
    ECHO         board: arduinoUNO, arduinoUNO_8MHz, arduinoNANO, arduinoNANO_8MHz, arduinoMEGA, easyHOME
    ECHO         project: name of your project
    ECHO:
    ECHO     Optional field:
    ECHO         COM: COM1, COM2, ...
	ECHO         NOBOOTLOADER: to upload without bootloader (it will erase a previous bootloader if present)
    ECHO:
    EXIT /B -1

:BUILD_KO
    ECHO:
    ECHO ERROR!!! :(
    ECHO:
    EXIT /B -1

:BUILD_OK
    ECHO:
    ECHO Build completed!!! :)
    ECHO:
    EXIT /B 0

:UPLOAD_OK
    ECHO:
    ECHO Upload completed!!! :)
    ECHO:
    EXIT /B 0

:MAIN
SET processor_type=
SET board_type=
SET upload_device=%3
SET file_name=%2
SET cpu_speed=
SET upload_type=
SET upload_speed=
SET no_bootloader=false
SET fuse_ext=
SET fuse_high=
SET fuse_low=

IF "%1%"=="arduinoUNO" (
    SET processor_type=atmega328p
    SET board_type=__BOARD_arduinoUNO__
	SET upload_type=arduino
	SET upload_speed=115200
	SET cpu_speed=__FCPU_16MHz__
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xFF
) ELSE IF "%1%"=="arduinoUNO_8MHz" (
    SET processor_type=atmega328p
    SET board_type=__BOARD_arduinoUNO__
	SET upload_type=arduino
	SET upload_speed=57600
	SET cpu_speed=__FCPU_8MHz__
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xE2
) ELSE IF "%1%"=="arduinoNANO" (
    SET processor_type=atmega328p
    SET board_type=__BOARD_arduinoNANO__
	SET upload_type=arduino
	SET upload_speed=115200
	SET cpu_speed=__FCPU_16MHz__
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xFF
) ELSE IF "%1%"=="arduinoNANO_8MHz" (
    SET processor_type=atmega328p
    SET board_type=__BOARD_arduinoNANO__
	SET upload_type=arduino
	SET upload_speed=57600
	SET cpu_speed=__FCPU_8MHz__
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xE2
) ELSE IF "%1%"=="arduinoMEGA" (
    SET processor_type=atmega2560
    SET board_type=__BOARD_arduinoMEGA__
	SET upload_type=wiring
	SET upload_speed=115200
	SET cpu_speed=__FCPU_16MHz__
	SET fuse_ext=0xFD
    SET fuse_high=0xD8
    SET fuse_low=0xFF
) ELSE IF "%1%"=="easyHOME" (
    SET processor_type=atmega2560
    SET board_type=__BOARD_easyHOME__
	SET upload_type=wiring
	SET upload_speed=115200
	SET cpu_speed=__FCPU_16MHz__
	SET fuse_ext=0xFD
    SET fuse_high=0xD8
    SET fuse_low=0xFF
) ELSE (
    GOTO :HELP
)

IF /I "%4%"=="NOBOOTLOADER" (
    SET no_bootloader=true
)

IF NOT DEFINED file_name (
    GOTO :HELP
)

SET res=false
IF %processor_type%==atmega328p SET res=true
IF %processor_type%==atmega2560 SET res=true
IF %res%==true (
    ".\AVR-GCC\bin\avr-g++.exe" ..\Projects\%file_name%\%file_name%.c -Os -Wl,-u,vfprintf -lprintf_flt -mmcu=%processor_type% -o ..\Projects\%file_name%\%file_name%.out -D%board_type% -D%cpu_speed%
    if errorlevel 1 (
        GOTO :BUILD_KO
    )
) ELSE (
    GOTO :BUILD_KO
)

SET res=false
IF %processor_type%==atmega328p SET res=true
IF %processor_type%==atmega2560 SET res=true
IF %res%==true (
    ".\AVR-GCC\bin\avr-objdump.exe" -S ..\Projects\%file_name%\%file_name%.out > ..\Projects\%file_name%\%file_name%.c.asm
    if errorlevel 1 (
        GOTO :BUILD_KO
    )
) ELSE (
    GOTO :BUILD_KO
)

SET res=false
IF %processor_type%==atmega328p SET res=true
IF %processor_type%==atmega2560 SET res=true
IF %res%==true (
    ".\AVR-GCC\bin\avr-size.exe" ..\Projects\%file_name%\%file_name%.out
    if errorlevel 1 (
        GOTO :BUILD_KO
    )
) ELSE (
    GOTO :BUILD_KO
)

IF NOT DEFINED upload_device (
    GOTO :BUILD_OK
)

SET res=false
IF %processor_type%==atmega328p SET res=true
IF %processor_type%==atmega2560 SET res=true
IF %res%==true (
    ".\AVR-GCC\bin\avr-objcopy.exe" -O ihex ..\Projects\%file_name%\%file_name%.out ..\Projects\%file_name%\%file_name%.hex
	if errorlevel 1 (
        GOTO :BUILD_KO
    )
) ELSE (
    GOTO :BUILD_KO
)

SET res=false
IF %processor_type%==atmega328p SET res=true
IF %processor_type%==atmega2560 SET res=true
IF %res%==true (
    IF %no_bootloader%==true (
	    ".\AVR-GCC\bin\avrdude.exe" -p %processor_type% -P %upload_device% -c stk500v1 -b 19200 -C .\AVR-GCC\etc\avrdude.conf -e -Ulock:w:0x3F:m -Uefuse:w:%fuse_ext%:m -Uhfuse:w:%fuse_high%:m -Ulfuse:w:%fuse_low%:m
	    if errorlevel 1 (
            GOTO :BUILD_KO
        )
		".\AVR-GCC\bin\avrdude.exe" -p %processor_type% -P %upload_device% -c stk500v1 -b 19200 -C .\AVR-GCC\etc\avrdude.conf -U flash:w:..\Projects\%file_name%\%file_name%.hex:i -Ulock:w:0x0F:m
	    if errorlevel 1 (
            GOTO :BUILD_KO
        )
	) ELSE (
        ".\AVR-GCC\bin\avrdude.exe" -q -q -p %processor_type% -D -P %upload_device% -c %upload_type% -b %upload_speed% -C .\AVR-GCC\etc\avrdude.conf -U flash:w:..\Projects\%file_name%\%file_name%.hex:i
		if errorlevel 1 (
            GOTO :BUILD_KO
        )
	)
)

GOTO :UPLOAD_OK
