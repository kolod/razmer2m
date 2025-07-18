# Razmer 2M-1104 УЧПУ Emulator

This is an Arduino-based emulator for a Razmer 2M-1104 УЧПУ (Numerical Control Unit) - a Soviet/Russian CNC control system. The emulator simulates the behavior of this industrial control unit.

## Description

The emulator simulates the operation of the Razmer 2M-1104 УЧПУ in terms of signal indication output.

### Output Signals:
- цикл А7 (cycle A7)
- подциклы В0-В4 (subcycles B0-B4)
- w1(МР1), w2(МР1), w4(МР1), w8(МР1)
- МР1.3Б.9 (error signal from the second register)

## Hardware Requirements

- Arduino Mega 2560
- Appropriate wiring as specified in the gpio options files

### Pin Mapping Table

| Signal Name | ATmega2560 Pin | ATmega328p Pin | Description                |
|-------------|----------------|----------------|----------------------------|
| W1_PIN      | PH3            | PB0            | МР1 bit 1                  |
| W2_PIN      | PH4            | PB1            | МР1 bit 2                  |
| W4_PIN      | PH5            | PB2            | МР1 bit 4                  |
| W8_PIN      | PH6            | PB3            | МР1 bit 8                  |
| ER_PIN      | PH1            | PB4            | МР1.3Б.9 (error signal)    |
| B0_PIN      | PA0            | PC0            | Subcycle B0                |
| B1_PIN      | PA1            | PC1            | Subcycle B1                |
| B2_PIN      | PA2            | PC2            | Subcycle B2                |
| B3_PIN      | PA3            | PC3            | Subcycle B3                |
| B4_PIN      | PA4            | PC4            | Subcycle B4                |
| A7_PIN      | PH0            | PC5            | Cycle A7                   |

*Note: Pin assignments are defined in `config.h` and depend on the target microcontroller (ATmega2560 or ATmega328p).*

## Configuration

### 1. File config.h
- Set the startup delay in milliseconds (`START_DELAY_MS`)
- Choose hardware type by commenting/uncommenting the appropriate option
- Optionally invert B0-B4 signals by uncommenting `GPIO_INVERT_B_SIGNALS`

### 2. GPIO Options
Check the files in `lib/modules/gpio/options/` for different hardware configurations:
- `arduino_mega_pro_avr.c` - Standard AVR configuration
- `arduino_mega_pro_wiring.c` - Wiring configuration  
- `*_b_direct.c` - Direct B signal configuration
- `*_b_inverted.c` - Inverted B signal configuration

## Building and Uploading

### Using PlatformIO CLI:
```bash
# Build the project
pio build

# Upload to Arduino Mega
pio run -t upload

# Open serial monitor
pio device monitor
```

### Using PlatformIO IDE:
1. Open the project folder in VS Code with PlatformIO extension
2. Use the PlatformIO toolbar to build and upload

## Connection Scheme

Refer to the files in `lib/modules/gpio/options/` directory. The compiler will also output a message with the selected hardware and connection scheme.

## Linux Build (for debugging)

The original `build.sh` script is still available for building a Linux executable for debugging purposes.

## License

This project maintains the original license from the source repository.
