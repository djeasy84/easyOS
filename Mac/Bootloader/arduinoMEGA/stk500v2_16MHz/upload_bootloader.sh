#!/bin/bash

function help
{
    echo
    echo "upload bootloder (for arduinoMEGA board - 16MHz Crystal) command syntax:"
    echo
    echo "./upload_bootloader /dev/***"
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

upload_device=null
if [[ -n "$1" ]]
then
    upload_device=$1
fi

if [[ $upload_device == null ]]
then
    help
fi

../../../AVR-GCC/bin/avrdude -p atmega2560 -P $upload_device -c stk500v1 -b 19200 -C ../../../AVR-GCC/etc/avrdude.conf -e -Ulock:w:0x3F:m -Uefuse:w:0xFD:m -Uhfuse:w:0xD8:m -Ulfuse:w:0xFF:m
if [[ $? != 0 ]]
then
    upload_ko
fi
../../../AVR-GCC/bin/avrdude -p atmega2560 -P $upload_device -c stk500v1 -b 19200 -C ../../../AVR-GCC/etc/avrdude.conf -U flash:w:stk500boot_v2_mega2560.hex:i -Ulock:w:0x0F:m
if [[ $? != 0 ]]
then
    upload_ko
fi

upload_ok
