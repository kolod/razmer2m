#pragma once

#include <Arduino.h>
#include "config.h"

class Data {
public:
    Data();
    void append(char value);
    String row(uint8_t row);
    String all();
    void print();
    void clear();


#ifdef NATIVE
    void printDebug();
#endif

private:
    uint8_t mPosition;
    char mData[RAZMER2M_ROWS_COUNT * (RAZMER2M_COLUMNS_COUNT + 1)];
    
};

extern Data data;
