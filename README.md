# EEPROM Library for AT25320A/M95040-w

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

This C library provides functions to work with the AT25320A EEPROM and other register-compatible EEPROMs like M95040-w. This library is compatible with SPC5 MCUs and can be adapted can be adapted for use with other MCUs by modifying functions categorized as 'HAL functions'.

## Functions

### Writing Control

- `EEPROM_enableWrite()`: Enable writing to the EEPROM.
- `EEPROM_disableWrite()`: Disable writing to the EEPROM.

### Status Register

- `EEPROM_readStatusReg()`: Read from the status register.
- `EEPROM_writeStatusReg()`: Write to the status register.

### Memory Access

- `EEPROM_readByte()`: Read a single byte from the EEPROM.
- `EEPROM_writeByte()`: Write a single byte to the EEPROM.
- `EEPROM_readRange()`: Read a range of bytes from the EEPROM.
- `EEPROM_writeRange()`: Write a range of bytes to the EEPROM.

### Waiting

- `EEPROM_wait()`: Polling function to wait until the EEPROM finishes writing and becomes available for further operations.

## Usage

For more detailed usage instructions, please refer to the comments within the library code.

## Compatibility

This library is designed specifically for the SPC5 MCUs. However, with appropriate adjustments to the hardware abstraction layer (HAL) functions, it can be adapted to work with other MCUs.

## Contribution

Contributions and improvements to the library are welcome. Feel free to submit pull requests or open issues for any suggestions or bug reports.
