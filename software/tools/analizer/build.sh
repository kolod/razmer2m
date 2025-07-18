#!/bin/bash

set -e

clear
clear

#MODE=-DMODE_DEBUG
MODE=-DMODE_TEST
GCC_OPTIONS="-Wall -pedantic -g"
THIS_DIR=$(pwd)

modules=(analizer app ArduinoLinux avr biState_t debug gpio LLBuffer LLBufferItem logicLevel_t printer result_t rmc rsc signals time stime_t Ton VDBuffer VDBufferItem yn_t)

function buildModule {
	cd $1 &&
	if [ -f implementation.c ]; then
		gcc $MODE $GCC_OPTIONS -c implementation.c -o $1.o
	fi
	cd ../
}

function buildModules {
	for m in "${modules[@]}"; 
	do
		echo "$m"
		buildModule $m
	
	done
}

function packModuleToLib {
	cd $1 &&
	if [ -f $1.o ]; then
		ar -cr ../../libapp.a $1.o
		rm -f $1.o
	fi
	cd ../
}

function packModulesToLib {
	for m in "${modules[@]}"; 
	do
		packModuleToLib $m
	done
}

cd modules &&
buildModules &&
packModulesToLib &&
cd ../ && \
gcc  $MODE $GCC_OPTIONS  main_linux.c -o analizer -L./ -lapp
rm -f libapp.a
