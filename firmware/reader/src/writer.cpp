#include "writer.h"

// Global writer instance for legacy C interface
static Writer writerInstance;

// Writer class implementation
Writer::Writer() : wSign(0), currentState(StateIdle), busyTimer(Timer()) {
    busyTimer.setInterval(BUSY_TIMEOUT_US);
}

void Writer::begin() {
    busyTimer.reset();
    state = WriterState::WaitData;
}

void Writer::run(Buffer *buffer) {
    busyTimer.reset();
    bool shouldYield = false;

    while (!busyTimer.isExpired() && !shouldYield) {
        switch (state) {
            case WriterState::WaitData:
                if (!buffer->lockForReading()) return;
                state = WriterState::StartSign;
                break;
                
            case WriterState::GetNextSign:
                state = (!buffer->readNext(&wSign)) ? WriterState::WriteStopSign : WriterState::WriteStartSign;
                break;

            case WriterState::WriteStartSign:
                wSign = CONNECTION_PACKAGE_DELIMITER_START;
                if (Serial.availableForWrite()) {
                    Serial.write((char*) wSign, 1);
                    state = WriterState::GetNextSign;
                }
                break;
                
            case WriterState::WriteSigns:
                shouldYield = writeDataWriteSigns(buffer);
                break;
            case WriterState::WriteStopSign:
                shouldYield = writeDataWriteStopSign(buffer);
        }
    }
}

// State machine methods

bool Writer::writeDataWaitData(Buffer *buffer) {
    if (buffer_lockForReading(buffer) != RESULT_SUCCESS)
        return true;
    
    state = WriterState::StartSign;
    return false;
}

bool Writer::writeDataGetNextSign(Buffer *buffer) {
    if (buffer_readNext(buffer, &wSign) != RESULT_SUCCESS) {
        currentState = WriterState::WriteStopSign;
        return false;
    }
    state = WriterState::Signs;
    return false;
}

bool Writer::writeDataWriteStartSign(Buffer *buffer) {
    wSign = CONNECTION_PACKAGE_DELIMITER_START;
    if (serial_writeByte(&wSign) != RESULT_SUCCESS) {
        return false;
    }
    state = WriterState::GetNextSign;
    return false;
}

bool Writer::writeDataWriteSigns(Buffer *buffer) {
    if (serial_writeByte(&wSign) != RESULT_SUCCESS) {
        return false;
    }
    state = WriterState::GetNextSign;
    return false;
}

bool Writer::writeDataWriteStopSign(Buffer *buffer) {
    wSign = CONNECTION_PACKAGE_DELIMITER_STOP;
    if (serial_writeByte(&wSign) != RESULT_SUCCESS) {
        return false;
    }
    buffer_unlockForReading(buffer);
    state = WriterState::WaitData;
    return true;
}

Writer writer;
