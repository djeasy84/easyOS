#!/bin/bash

#
# MIT License
#
# This file is part of easyOS.
# You can find the software at https://github.com/djeasy84/easyOS
#
# Copyright (c) 2018 - Lorenzo Baglioni (djeasy84) <lbaglioni84@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#

function help
{
    echo
    echo "build command syntax:"
    echo "     --help          to show this help"
    echo "     --board         to specify the board type [arduinoUNO - arduinoUNO_8MHz - arduinoNANO - arduinoNANO_8MHz - arduinoMEGA - easyHOME]"
    echo "     --upload        to specify the port name for upload [/dev/*** - blank if you want only compiling]"
    echo "     --file          to specify the file name for compiling"
    echo
    echo "     --nobootloader  to upload without bootloader (it will erase a previous bootloader if present)"
    echo
    echo "./build --board arduinoUNO --file example_project_name [--upload /dev/***] [--nobootloader]"
    echo
    exit 1
}

function build_ko
{
    echo
    echo "ERROR!!! :("
    echo
    exit 1
}

function build_ok
{
    echo
    echo "Build completed!!! :)"
    echo
    exit 0
}

function upload_ok
{
    echo
    echo "Upload completed!!! :)"
    echo
    exit 0
}

processor_type=null
upload_device=null
board_type=null
file_name=null
cpu_speed=null
upload_type=null
upload_speed=null
no_bootloader=false
fuse_ext=null
fuse_high=null
fuse_low=null

while (( $# > 0 ))
do
    if [[ $1 == "--board" ]]
    then
        shift 1
        if [[ $1 == "arduinoUNO" ]]
        then
            board_type=__BOARD_arduinoUNO__
            processor_type=atmega328p
            upload_type=arduino
            upload_speed=115200
            cpu_speed=__FCPU_16MHz__
			fuse_ext=0xFD
            fuse_high=0xDE
            fuse_low=0xFF
        fi
        if [[ $1 == "arduinoUNO_8MHz" ]]
        then
            board_type=__BOARD_arduinoUNO__
            processor_type=atmega328p
            upload_type=arduino
            upload_speed=57600
            cpu_speed=__FCPU_8MHz__
			fuse_ext=0xFD
            fuse_high=0xDE
            fuse_low=0xE2
        fi
        if [[ $1 == "arduinoNANO" ]]
        then
            board_type=__BOARD_arduinoNANO__
            processor_type=atmega328p
            upload_type=arduino
            upload_speed=115200
            cpu_speed=__FCPU_16MHz__
			fuse_ext=0xFD
            fuse_high=0xDE
            fuse_low=0xFF
        fi
        if [[ $1 == "arduinoNANO_8MHz" ]]
        then
            board_type=__BOARD_arduinoNANO__
            processor_type=atmega328p
            upload_type=arduino
            upload_speed=57600
            cpu_speed=__FCPU_8MHz__
			fuse_ext=0xFD
            fuse_high=0xDE
            fuse_low=0xE2
        fi
        if [[ $1 == "arduinoMEGA" ]]
        then
            board_type=__BOARD_arduinoMEGA__
            processor_type=atmega2560
            upload_type=wiring
            upload_speed=115200
            cpu_speed=__FCPU_16MHz__
			fuse_ext=0xFD
            fuse_high=0xD8
            fuse_low=0xFF
        fi
        if [[ $1 == "easyHOME" ]]
        then
            board_type=__BOARD_easyHOME__
            processor_type=atmega2560
            upload_type=wiring
            upload_speed=115200
            cpu_speed=__FCPU_16MHz__
			fuse_ext=0xFD
            fuse_high=0xD8
            fuse_low=0xFF
        fi
    fi
    if [[ $1 == "--upload" ]]
    then
        shift 1
        upload_device=$1
    fi
    if [[ $1 == "--file" ]]
    then
        shift 1
        file_name=$1
    fi
    if [[ $1 == "--nobootloader" ]]
    then
        shift 1
        no_bootloader=true
    fi
    shift 1
done

if [[ $board_type == null ]] || [[ $file_name == null ]]
then
    help
fi

if [[ $processor_type == atmega328p ]] || [[ $processor_type == atmega2560 ]]
then
    ./AVR-GCC/bin/avr-g++ -I../ -I../System/ -I../Libraries/ ../Projects/$file_name/$file_name.c -Os -mmcu=$processor_type -o ../Projects/$file_name/$file_name.out -D$board_type -D$cpu_speed
else
    build_ko
fi
if [[ $? != 0 ]]
then
    build_ko
fi

if [[ $processor_type == atmega328p ]] || [[ $processor_type == atmega2560 ]]
then
    ./AVR-GCC/bin/avr-objdump -S ../Projects/$file_name/$file_name.out > ../Projects/$file_name/$file_name.c.asm
else
    build_ko
fi
if [[ $? != 0 ]]
then
    build_ko
fi

if [[ $processor_type == atmega328p ]] || [[ $processor_type == atmega2560 ]]
then
    ./AVR-GCC/bin/avr-size -C --mcu=$processor_type ../Projects/$file_name/$file_name.out
else
    build_ko
fi
if [[ $? != 0 ]]
then
    build_ko
fi

if [[ $upload_device == null ]]
then
    build_ok
fi

if [[ $processor_type == atmega328p ]] || [[ $processor_type == atmega2560 ]]
then
    ./AVR-GCC/bin/avr-objcopy -O ihex ../Projects/$file_name/$file_name.out ../Projects/$file_name/$file_name.hex
else
    build_ko
fi
if [[ $? != 0 ]]
then
    build_ko
fi

if [[ $processor_type == atmega328p ]] || [[ $processor_type == atmega2560 ]]
then
    if [[ $no_bootloader == true ]]
    then
        ./AVR-GCC/bin/avrdude -p $processor_type -P $upload_device -c stk500v1 -b 19200 -C ./AVR-GCC/etc/avrdude.conf -e -Ulock:w:0x3F:m -Uefuse:w:$fuse_ext:m -Uhfuse:w:$fuse_high:m -Ulfuse:w:$fuse_low:m
        if [[ $? != 0 ]]
        then
            upload_ko
        fi
        ./AVR-GCC/bin/avrdude -p $processor_type -P $upload_device -c stk500v1 -b 19200 -C ./AVR-GCC/etc/avrdude.conf -U flash:w:../Projects/$file_name/$file_name.hex:i -Ulock:w:0x0F:m
        if [[ $? != 0 ]]
        then
            upload_ko
        fi
    else
        ./AVR-GCC/bin/avrdude -q -q -p $processor_type -D -P $upload_device -c $upload_type -b $upload_speed -C ./AVR-GCC/etc/avrdude.conf -U flash:w:../Projects/$file_name/$file_name.hex:i
        if [[ $? != 0 ]]
        then
            build_ko
        fi
    fi
fi

upload_ok
