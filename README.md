[![Stand With Ukraine](https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner2-direct.svg)](https://vshymanskyy.github.io/StandWithUkraine/)

# Razmer2M [![CI (Linux & Windows)](https://github.com/kolod/razmer2m/actions/workflows/ci.yml/badge.svg)](https://github.com/kolod/razmer2m/actions/workflows/tests.yml)

A hardware and software system for replacing a cathode ray tube indicator with an LED display for machine tools equipped with a numerical control device (NCU) "Razmer 2M 1104".

## Configuration

The project behavior can be customized through configuration values defined in `include/config.h`:

### Default Configuration Values

| Parameter | Default | Range | Description |
|-----------|---------|-------|-------------|
| `AXIS_COUNT` | 4 | 1-5 | Number of axes to display |
| `AXIS_DIGIT_COUNT` | 6 | 1-7 | Total number of digits per axis (including decimal) |
| `AXIS_DOT_POSITION` | 4 | 0-AXIS_DIGIT_COUNT | Position of decimal point from left (0-based) |


### Customizing Configuration

You can override these values at build time using CMake variables. For example, to set custom values for AXIS_COUNT, AXIS_DIGIT_COUNT, and AXIS_DOT_POSITION:

```
cmake --preset firmware -DAXIS_COUNT=3 -DAXIS_DIGIT_COUNT=5 -DAXIS_DOT_POSITION=2
cmake --build --preset firmware
```

Or, using the workflow command:

```
cmake --workflow firmware -DAXIS_COUNT=3 -DAXIS_DIGIT_COUNT=5 -DAXIS_DOT_POSITION=2
```

These values will be passed as preprocessor defines to the firmware build. You can also set them in your CMake GUI or by editing your CMakePresets.json.


## GPIO Pin Mapping and Configuration

The following GPIO pin assignments are used in the project (see `include/gpio.h`):

### Serial Communication
- **PD0**: Serial RX
- **PD1**: Serial TX

### Display (used only on receiver)
- **PB2**: Display CS
- **PB3**: Display MOSI
- **PB4**: Display MISO
- **PB5**: Display SCK (conflicts with built-in LED)

### Razmer2M (inputs for normal operation, output on emulator, not used on receiver)
- **PD2**: W1
- **PD3**: W2
- **PD4**: W4
- **PD5**: W8
- **PD6**: ER
- **PD7**: A7
- **PC0**: B0
- **PC1**: B1
- **PC2**: B2
- **PC3**: B3
- **PC4**: B4
- **PC5**: B5


## Building Firmware

The firmware for Razmer2M can be built using the provided CMake preset `firmware`. This will generate binaries for the emulator, receiver, and transmitter.

### Prerequisites
- CMake 3.31 or newer
- AVR-GCC toolchain installed and available in your PATH
- Ninja build system (recommended)

### Steps to Build Firmware

1. **Configure the firmware build directory using the preset:**
   ```sh
   cmake --preset firmware
   ```

   If you want to force CMake to re-configure from scratch (ignoring cached settings), add `--fresh`:
   ```sh
   cmake --preset firmware --fresh
   ```

2. **Build the firmware:**
   ```sh
   cmake --build --preset firmware
   ```

   To force a clean build (delete previous build outputs first), add `--clean-first`:
   ```sh
   cmake --build --preset firmware --clean-first
   ```

   To see all compiler and build commands in detail, add `--verbose`:
   ```sh
   cmake --build --preset firmware --verbose
   ```

   You can combine both options:
   ```sh
   cmake --build --preset firmware --clean-first --verbose
   ```

### Alternative: Using CMake Workflow

You can also use the CMake workflow command to configure and build the firmware in one step:

```sh
cmake --workflow firmware
```

This will run both configuration and build steps for the `firmware` preset automatically.

### Notes
- The preset builds firmware in the `build/firmware` directory using the AVR-GCC toolchain.
- Make sure AVR-GCC and Ninja are installed and available in your system PATH.
- You can edit `CMakePresets.json` to customize compilers or build options for firmware.


## Testing

The project includes two types of tests:

### Native Tests
Tests that run directly on the host system using Google Test framework.

#### Prerequisites
- CMake 3.31 or newer
- GCC/MinGW toolchain installed and available in your PATH for tests
- Ninja build system (recommended)

#### Steps to Build and Run Native Tests

1. **Configure the tests build directory using the preset:**
   ```sh
   cmake --preset tests-gcc
   ```

2. **Build the tests:**
   ```sh
   cmake --build --preset tests-gcc
   ```

3. **Run the tests:**
   ```sh
   cd build/tests
   ctest
   ```
   Or run the test executable directly:
   ```sh
   ./build/tests/tests/test_format_native
   ```

### Alternative: Using CMake Workflow

You can also use the CMake workflow commands:

**For native tests:**
```sh
cmake --workflow tests-gcc
```

**For AVR tests:**
```sh
cmake --workflow tests-avr
```

This will run configuration, build and run tests automatically.

### Notes
- The preset builds tests in the `build/tests` directory using the MinGW native compiler.
- Make sure MinGW and Ninja are installed and available in your system PATH.
- You can edit `CMakePresets.json` to customize compilers or build options.


## Continuous Integration

### GitHub Actions

The project includes automated testing via GitHub Actions with two test jobs:

**Native Tests:**
- Installs CMake 3.31, Ninja, and GCC
- Configures and builds tests using the `tests-gcc` preset
- Runs native tests with Google Test framework

**AVR Tests:**
- Installs CMake 3.31, Ninja, GCC, and AVR-GCC
- Builds AVR test application for ATmega328P
- Builds test runner application
- Compiles and tests AVR code for target hardware compatibility

The workflow runs on pushes and pull requests to `main` and `develop` branches, and can also be triggered manually via `workflow_dispatch`.

### Local Testing with act

You can run the GitHub Actions workflow locally using `act`, which uses Docker to simulate the GitHub Actions environment.

#### Prerequisites
- Docker installed and running
- `act` installed ([installation guide](https://github.com/nektos/act#installation))

#### Installation

**Windows (using winget):**
```powershell
winget install nektos.act
```

**Windows (using Chocolatey):**
```powershell
choco install act-cli
```

**Windows (using Scoop):**
```powershell
scoop install act
```

#### Running Tests Locally

1. **Navigate to project directory:**
   ```bash
   cd /path/to/razmer2m
   ```

2. **Run the test workflow:**
   ```bash
   act workflow_dispatch
   ```
   
   Or run a specific workflow file:
   ```bash
   act -W .github/workflows/tests.yml
   ```

3. **For debugging, use dry-run:**
   ```bash
   act --dry-run workflow_dispatch
   ```

#### Configuration

The project includes a `.actrc` file that configures `act` to use a Docker image with updated tools:
```
-P ubuntu-latest=ghcr.io/catthehacker/ubuntu:act-latest
```

#### Alternative: Manual Docker Testing

You can also run tests manually using the provided `Dockerfile.test`:

```bash
# Build test image
docker build -f Dockerfile.test -t razmer2m-test .

# Run container
docker run -it --rm -v $(pwd):/workspace razmer2m-test

# Inside container, run tests
cmake --preset tests-gcc
cmake --build --preset tests-gcc
cd build/tests && ctest --verbose
```
