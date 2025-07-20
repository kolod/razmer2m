/**
 * @file ascii2bcd.cpp
 * @brief ASCII to BCD (Binary Coded Decimal) conversion utilities
 * 
 * This module provides bidirectional conversion between ASCII characters
 * and BCD values for numerical display and input handling.
 */

#include "ascii2bcd.h"

/**
 * @brief Mapping table from BCD values to ASCII characters
 * 
 * Maps BCD values (0-13) to their corresponding ASCII characters:
 * - 0-9: Standard digits '0'-'9'
 * - 10: '*' (asterisk)
 * - 11: '#' (hash/pound)
 * - 12: '+' (plus)
 * - 13: '-' (minus)
 */
const static char bcdToAsciiMapping[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '#', '+', '-'
};

/**
 * @brief Convert ASCII character to BCD value
 * 
 * Searches through the mapping table to find the BCD value
 * corresponding to the given ASCII character.
 * 
 * @param v ASCII character to convert
 * @return BCD value (0-13) if found, 0 if character not in mapping
 */
int asciiToBcd(const uint8_t v) {
    // Search through the mapping table
    for (uint8_t i = 0; i < (sizeof(bcdToAsciiMapping) / sizeof (bcdToAsciiMapping[0])); i++){
        if(bcdToAsciiMapping[i] == v) {
            return i; // Return the BCD value (index)
        }
    }
    return 0; // Default to 0 if character not found
}

/**
 * @brief Convert BCD value to ASCII character
 * 
 * Uses direct array indexing to convert BCD value to its
 * corresponding ASCII character.
 * 
 * @param v BCD value (0-13) to convert
 * @return ASCII character if valid BCD value, 0 (null) if out of range
 */
char bcdToAscii(const uint8_t v) {
    // Check if BCD value is within valid range
    if (v < (sizeof bcdToAsciiMapping / sizeof (bcdToAsciiMapping[0]))) {
        return bcdToAsciiMapping[v]; // Return corresponding ASCII character
    }
    return 0; // Return null character for invalid BCD values
}
