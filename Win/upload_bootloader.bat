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
    ECHO upload bootloder command syntax:
    ECHO     upload_bootloader.bat [board] [COM]
    ECHO:
    ECHO     Mandatory fields:
    ECHO         board: arduinoUNO, arduinoUNO_8MHz, arduinoNANO, arduinoNANO_8MHz, arduinoMEGA, easyHOME
    ECHO         COM: COM1, COM2, ...
	ECHO:
    EXIT /B -1

:UPLOAD_KO
    ECHO:
    ECHO ERROR!!! :(
    ECHO:
    EXIT /B -1

:UPLOAD_OK
    ECHO:
    ECHO Upload completed!!! :)
    ECHO:
    EXIT /B 0

:MAIN
SET processor_type=
SET upload_device=%2
SET file_name=
SET fuse_ext=
SET fuse_high=
SET fuse_low=

IF "%1%"=="arduinoUNO" (
    SET processor_type=atmega328p
	SET file_name=..\Bootloader\arduinoUNO\optiboot_16MHz\optiboot_atmega328.hex
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xFF
) ELSE IF "%1%"=="arduinoUNO_8MHz" (
    SET processor_type=atmega328p
	SET file_name=..\Bootloader\arduinoUNO\optiboot_8MHz\optiboot_atmega328_8mhz.hex
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xE2
) ELSE IF "%1%"=="arduinoNANO" (
    SET processor_type=atmega328p
	SET file_name=..\Bootloader\arduinoUNO\optiboot_16MHz\optiboot_atmega328.hex
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xFF
) ELSE IF "%1%"=="arduinoNANO_8MHz" (
    SET processor_type=atmega328p
	SET file_name=..\Bootloader\arduinoUNO\optiboot_8MHz\optiboot_atmega328_8mhz.hex
	SET fuse_ext=0xFD
    SET fuse_high=0xDE
    SET fuse_low=0xE2
) ELSE IF "%1%"=="arduinoMEGA" (
    SET processor_type=atmega2560
	SET file_name=..\Bootloader\arduinoMEGA\stk500v2_16MHz\stk500boot_v2_mega2560.hex
	SET fuse_ext=0xFD
    SET fuse_high=0xD8
    SET fuse_low=0xFF
) ELSE IF "%1%"=="easyHOME" (
    SET processor_type=atmega2560
	SET file_name=..\Bootloader\easyHOME\stk500v2_16MHz\stk500boot_v2_easyHOME.hex
	SET fuse_ext=0xFD
    SET fuse_high=0xD8
    SET fuse_low=0xFF
) ELSE (
    GOTO :HELP
)

IF NOT DEFINED upload_device (
    GOTO :HELP
)

SET res=false
IF %processor_type%==atmega328p SET res=true
IF %processor_type%==atmega2560 SET res=true
IF %res%==true (
	".\AVR-GCC\bin\avrdude.exe" -p %processor_type% -P %upload_device% -c stk500v1 -b 19200 -C .\AVR-GCC\etc\avrdude.conf -e -Ulock:w:0x3F:m -Uefuse:w:%fuse_ext%:m -Uhfuse:w:%fuse_high%:m -Ulfuse:w:%fuse_low%:m
	if errorlevel 1 (
		GOTO :UPLOAD_KO
	)
	".\AVR-GCC\bin\avrdude.exe" -p %processor_type% -P %upload_device% -c stk500v1 -b 19200 -C .\AVR-GCC\etc\avrdude.conf -U flash:w:%file_name%:i -Ulock:w:0x0F:m
	if errorlevel 1 (
		GOTO :UPLOAD_KO
	)
)

GOTO :UPLOAD_OK
