#!/bin/bash

export PATH=$PATH:../../../AVR-GCC/bin/

make clean

make easyHOME

cp stk500boot_v2_easyHOME.hex ../../../../Bootloader/easyHOME/stk500v2_16MHz/

make clean
