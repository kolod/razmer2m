
#include "data.h"

Data::Data() : mPosition(0) {
    // Initialize the data array to zero
	clear();
}

void Data::append(char value) {
    // If the data array is full, do not append
    // This prevents overflow and ensures we stay within bounds
    // The size of mData is defined by RAZMER2M_ROWS_COUNT and RAZMER2M_COLUMNS_COUNT
    // Each row has an additional character for the null terminator
    if (mPosition >= sizeof(mData) - 1) return;

    // Set the next character in the current position
    // The current position is tracked by the last character in the row
    mData[mPosition] = value;

    // Move to the next position
    // Cursor is moved by columns in first and then by rows
    mPosition += RAZMER2M_COLUMNS_COUNT + 1;

    
    // If we reach the end of the array, reset move to next column
    // This ensures that we can continue appending data in a circular manner
    // The next position will be the start of the next row
    if (mPosition >= sizeof(mData)) 
        mPosition -= (RAZMER2M_ROWS_COUNT * (RAZMER2M_COLUMNS_COUNT + 1)) - 1;
}

String Data::row(uint8_t row) {
	// Check if the row index is valid
	if (row >= RAZMER2M_ROWS_COUNT) return String();

    // Ensure the row is properly null-terminated
	mData[(row + 1) * (RAZMER2M_COLUMNS_COUNT + 1) - 1] = 0;

    // Return the row as a String
    // The row is accessed by calculating its starting index in the flattened array
    // and using the String constructor to create a String from that segment
    return String(mData[row * (RAZMER2M_COLUMNS_COUNT+1)]);
}

String Data::all() {
	String result;

    // Iterate through each row and concatenate them into a single String
    // Each row is followed by a newline character for separation
	for (uint8_t row = 0; row < RAZMER2M_ROWS_COUNT; row++)
		result += this->row(row) + "\n";
	
    // Return the concatenated result
    // This will give a multi-line String representation of the entire data
	return result;
}

void Data::print() {
    // Print the entire data to the Serial output
    // This will call the all() method to get the complete String representation
	Serial.println(all());
}

void Data::clear() {
    // Reset the position to the start
    // This is necessary to ensure that the next append starts from the beginning
    mPosition = 0;

    // Clear the data array by setting all elements to zero
    // This effectively resets the data structure to an empty state
    // The size of mData is defined by RAZMER2M_ROWS_COUNT and RAZMER2M_COLUMNS_COUNT
    // Each row has an additional character for the null terminator
	for (uint8_t i = 0; i < sizeof(mData); mData[i++] = 0);
}

#ifdef NATIVE
    void Data::printDebug() {
        std::cout << "Current position: " << (int)mPosition << std::endl;
        std::cout << "Data content: " << std::endl;

        // Print the raw data array
        // This will show the internal state of the data array
        // Each character is printed, and null characters are represented as underscores
        std::cout << "Raw data: ";
        for (uint8_t i = 0; i < sizeof(mData); i++)
            std::cout << (mData[i] ? mData[i] : '_'); // Print underscore for null characters
        std::cout << std::endl;

        // Print the data as a matrix format
        std::cout << "Data matrix:" << std::endl;
        // Iterate through each row and print the characters
        // Each row is printed on a new line, with characters separated by spaces
        for (uint8_t row = 0; row < RAZMER2M_ROWS_COUNT; row++) {
            for (uint8_t col = 0; col <= RAZMER2M_COLUMNS_COUNT; col++) {
                uint8_t index = row * (RAZMER2M_COLUMNS_COUNT + 1) + col;
                std::cout << (mData[index] ? mData[index] : '_'); // Print underscore for null characters
            }
            std::cout << std::endl;
        }       
    }
#endif

// Global instance of Data for use in other parts of the program
// This allows other components to access and manipulate the data without needing to create a new instance
Data data;
