#!/bin/bash

export PATH=$PATH:../../../AVR-GCC/bin/

make clean

make atmega328_8mhz
