# Razmer 2M-1104 УЧПУ Emulator

This is a PlatformIO project for the Razmer 2M-1104 УЧПУ (Numerical Control Unit) emulator, designed to run on Arduino Mega 2560.

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

## Project Structure

```
├── platformio.ini          # PlatformIO configuration
├── src/
│   └── main.cpp            # Main Arduino sketch
├── include/
│   ├── config.h            # Project configuration
│   └── modules.h           # Module includes
├── lib/
│   └── modules/            # All project modules
│       ├── app/            # Main application logic
│       ├── gpio/           # GPIO handling
│       ├── debug/          # Debug utilities
│       └── ...             # Other modules
└── test/                   # Unit tests (if any)
```

## Connection Scheme

Refer to the files in `lib/modules/gpio/options/` directory. The compiler will also output a message with the selected hardware and connection scheme.

## Linux Build (for debugging)

The original `build.sh` script is still available for building a Linux executable for debugging purposes.

## License

This project maintains the original license from the source repository.
