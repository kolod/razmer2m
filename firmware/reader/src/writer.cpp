#include "writer.h"

// Global writer instance for legacy C interface
static Writer writerInstance;

// Writer class implementation
Writer::Writer() : wSign(0), currentState(StateIdle), busyTimer(Timer()) {
    busyTimer.setInterval(BUSY_TIMEOUT_US);
}

void Writer::begin() {
    busyTimer.reset();
    currentState = StateWaitData;
}

void Writer::run(Buffer *buffer) {
    bool shouldYield = false;
    busyTimer.reset();

    while (!busyTimer.isExpired() && !shouldYield) {
        switch (currentState) {
            case StateIdle:
                break;
            case StateWaitData:
                shouldYield = writeDataWaitData(buffer);
                break;
            case StateGetNextSign:
                shouldYield = writeDataGetNextSign(buffer);
                break;
            case StateWriteStartSign:
                shouldYield = writeDataWriteStartSign(buffer);
                break;
            case StateWriteSigns:
                shouldYield = writeDataWriteSigns(buffer);
                break;
            case StateWriteStopSign:
                shouldYield = writeDataWriteStopSign(buffer);
        }
    }
}

// State machine methods

bool Writer::writeDataWaitData(Buffer *buffer) {
    if (buffer_lockForReading(buffer) != RESULT_SUCCESS)
        return true;
    
    currentState = StateWriteStartSign;
    return false;
}

bool Writer::writeDataGetNextSign(Buffer *buffer) {
    if (buffer_readNext(buffer, &wSign) != RESULT_SUCCESS) {
        currentState = StateWriteStopSign;
        return false;
    }
    currentState = StateWriteSigns;
    return false;
}

bool Writer::writeDataWriteStartSign(Buffer *buffer) {
    wSign = CONNECTION_PACKAGE_DELIMITER_START;
    if (serial_writeByte(&wSign) != RESULT_SUCCESS) {
        return false;
    }
    currentState = StateGetNextSign;
    return false;
}

bool Writer::writeDataWriteSigns(Buffer *buffer) {
    if (serial_writeByte(&wSign) != RESULT_SUCCESS) {
        return false;
    }
    currentState = StateGetNextSign;
    return false;
}

bool Writer::writeDataWriteStopSign(Buffer *buffer) {
    wSign = CONNECTION_PACKAGE_DELIMITER_STOP;
    if (serial_writeByte(&wSign) != RESULT_SUCCESS) {
        return false;
    }
    buffer_unlockForReading(buffer);
    currentState = StateWaitData;
    return true;
}
