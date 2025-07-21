
#include "reader.h"


Reader::Reader() : state(ReaderState::A7NotActive), count(0) {
	errorTimer.setInterval(READER_TIMEOUT_MS);
	sendErrorTimer.setInterval(SEND_ERROR_INTERVAL_MS);
}

void Reader::writeCode(Buffer *buffer) {
	buffer->writeNext(gpio.read());
	count++;
}

void Reader::setError(Buffer *buffer) {
	buffer->writeAll(CONNECTION_ERROR_MASK_1);
	buffer->unlockForWriting();
	state = ReaderState::readA7NotActive;
}

void Reader::begin() {
	errorTimer.reset();
	sendErrorTimer.reset();
	state = ReaderState::A7NotActive;
}

void Reader::run(Buffer *buffer) {
	if (state != ReaderState::A7NotActive) {
		if (errorTimer.isExpired(true)) {
			buffer->unlockForWriting();
			state = ReaderState::Error;
			return;
		}
	}

	if (state == ReaderState::A7NotActive && gpio.A7() == LOW) {
		errorTimer.reset();
		state = ReaderState::A7Active;
		return;
	}

	if (state == ReaderState::A7Active && gpio.A7() == HIGH) {
		if (!buffer->lockForWriting()) {
			state = ReaderState::A7NotActive;
			return;
		}
		count = 0;
		state = ReaderState::A7NotActive2;
		return;
	}

	if (state == ReaderState::A7NotActive2 && gpio.A7() == LOW) {
		state = ReaderState::B1;
		return;
	}

	// Read first axis
	if (state == ReaderState::B1 && gpio.B1() == HIGH) {
		readCode(buffer);
		state = ReaderState::B2;
		return;
	}

	// Read second axis
	if (state == ReaderState::B2 && gpio.B2() == HIGH) {
		readCode(buffer);
		state = ReaderState::B3;
		return;
	}

	// Read third axis
	if (state == ReaderState::B3 && gpio.B3() == HIGH) {
		readCode(buffer);
		state = ReaderState::B4;
		return;
	}

	// Read fourth axis
	if (state == ReaderState::B4 && gpio.B4() == HIGH) {
		readCode(buffer);

		// Check if we have read enough signs
		if (count >= RAZMER2M_SIGNS_COUNT) {
			buffer->unlockForWriting();
			state = ReaderState::A7NotActive;
			return;
		}

		state = ReaderState::B1;
	}
}
