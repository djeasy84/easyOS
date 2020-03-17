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
    echo "upload bootloder command syntax:"
    echo "     --help          to show this help"
    echo "     --board         to specify the board type [arduinoUNO - arduinoUNO_8MHz - arduinoNANO - arduinoNANO_8MHz - arduinoMEGA - easyHOME]"
    echo "     --upload        to specify the port name for upload [/dev/*** - usb]"
    echo
    echo "./upload_bootloader --board arduinoUNO --upload /dev/***"
    echo
    exit 1
}

function upload_ko
{
    echo
    echo "ERROR!!! :("
    echo
    exit 1
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
upload_type=null
file_name=null
fuse_ext=null
fuse_high=null
fuse_low=null

while (( $# > 0 ))
do
    if [[ $1 == "--board" ]]
    then
        shift 1
        if [[ $1 == "arduinoUNO" ]] || [[ $1 == "arduinoNANO" ]]
        then
            processor_type=atmega328p
            file_name=../Bootloader/arduinoUNO/optiboot_16MHz/optiboot_atmega328.hex
            fuse_ext=0xFD
            fuse_high=0xDE
            fuse_low=0xFF
        fi
        if [[ $1 == "arduinoUNO_8MHz" ]] || [[ $1 == "arduinoNANO_8MHz" ]]
        then
            processor_type=atmega328p
            file_name=../Bootloader/arduinoUNO/optiboot_8MHz/optiboot_atmega328_8mhz.hex
            fuse_ext=0xFD
            fuse_high=0xDE
            fuse_low=0xE2
        fi
        if [[ $1 == "arduinoMEGA" ]]
        then
            processor_type=atmega2560
            file_name=../Bootloader/arduinoMEGA/stk500v2_16MHz/stk500boot_v2_mega2560.hex
            fuse_ext=0xFD
            fuse_high=0xD8
            fuse_low=0xFF
        fi
        if [[ $1 == "easyHOME" ]]
        then
            processor_type=atmega2560
            file_name=../Bootloader/easyHOME/stk500v2_16MHz/stk500boot_v2_easyHOME.hex
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
    shift 1
done

if [[ $processor_type == null ]] || [[ $file_name == null ]] || [[ $fuse_ext == null ]] || [[ $fuse_high == null ]] || [[ $fuse_low == null ]] || [[ $upload_device == null ]]
then
    help
fi

if [[ $upload_device == "usb" ]]
then
    upload_type=avrispmkII
else
    upload_type=stk500v1
fi

./AVR-GCC/bin/avrdude -p $processor_type -P $upload_device -c $upload_type -b 19200 -C ./AVR-GCC/etc/avrdude.conf -e -Ulock:w:0x3F:m -Uefuse:w:$fuse_ext:m -Uhfuse:w:$fuse_high:m -Ulfuse:w:$fuse_low:m
if [[ $? != 0 ]]
then
    upload_ko
fi
./AVR-GCC/bin/avrdude -p $processor_type -P $upload_device -c $upload_type -b 19200 -C ./AVR-GCC/etc/avrdude.conf -U flash:w:$file_name:i -Ulock:w:0x0F:m
if [[ $? != 0 ]]
then
    upload_ko
fi

upload_ok
