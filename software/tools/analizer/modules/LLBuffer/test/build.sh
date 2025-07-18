#!/bin/bash

set -e

clear
clear

#MODE=-DMODE_DEBUG
MODE=-DMODE_TEST
GCC_OPTIONS="-Wall -pedantic -g"

gcc  $MODE $GCC_OPTIONS  main.c -o test
