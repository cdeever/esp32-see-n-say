# Hardware Specifications

This directory contains hardware specifications and circuit diagrams for the ESP32 See-N-Say Audio Replacement project.

---

## Documents

### [ADC Input Circuit](./adc-input-circuit.md)
**Complete circuit design for safely interfacing See-N-Say speaker output with ESP32-S2 ADC input.**

**Key Information**:
- ASCII art circuit schematic
- Complete bill of materials with part numbers
- Voltage calculations and safety margins
- Component selection rationale
- Assembly instructions
- Testing and troubleshooting procedures

**Quick Specs**:
- Input: 0-5V peak-to-peak (See-N-Say speaker)
- Output: 0.4V - 2.9V typical (ADC-safe range)
- Protection: Voltage divider + DC blocking + clamping diodes
- Total cost: ~$2-3 USD for all components

**⚠️ Critical**: Never connect the See-N-Say speaker directly to the ESP32 ADC without this protection circuit. Maximum safe ADC input is 3.3V; exceeding this will damage the ESP32.

---

### [Pin Assignments](./pin-assignments.md)
**Complete mapping of all ESP32-S2 GPIO pins used in the project.**

**Includes**:
- I2S audio output pins (GPIO 37, 33, 34)
- ADC input pin (GPIO 1 / ADC1_CH0)
- SD card SPI pins (GPIO 10-13)
- Power and ground connections
- Reserved/unavailable pins (strapping, flash, USB)
- Available pins for future expansion

**Board-Specific Notes**:
- Pin verification checklist
- Common development board notes (Saola, DevKitC, Kaluga)
- Warnings about strapping and flash pins

---

## Quick Reference

### ADC Input Protection Circuit

**Required Components**:
| Component | Value | Purpose |
|-----------|-------|---------|
| R1, R2 | 10kΩ each | Voltage divider (2:1 ratio) |
| C1 | 10µF | DC blocking capacitor |
| R3 | 1kΩ | Series protection resistor |
| R4 | 10kΩ | Bias resistor to 3.3V |
| D1 | BAT54S | Schottky clamping diodes |

**Why It's Needed**:
- See-N-Say speaker: 0-5V (too high for ESP32)
- ESP32-S2 ADC: 0-3.3V maximum safe input
- Protection circuit ensures ADC input stays within 0.4V - 2.9V range

See [adc-input-circuit.md](./adc-input-circuit.md) for complete circuit diagram and assembly instructions.

---

### Pin Assignments Quick Reference

**Audio Input** (with protection circuit):
- GPIO 1 (ADC1_CH0) ← See-N-Say speaker output

**Audio Output** (I2S):
- GPIO 37 (I2S BCK) → Bit clock
- GPIO 33 (I2S WS) → Word select
- GPIO 34 (I2S DO) → Audio data

**SD Card** (SPI):
- GPIO 11 (MOSI), GPIO 13 (MISO), GPIO 12 (CLK), GPIO 10 (CS)
- ⚠️ Verify pins for your specific development board

See [pin-assignments.md](./pin-assignments.md) for complete details and reserved pins.

---

## Development Board

This project targets the **ESP32-S2** microcontroller.

**Target Hardware**:
- ESP32-S2 (specific development board TBD)
- 3.3V power supply (USB or external)
- I2S DAC or amplifier for audio output
- Micro SD card for audio file storage
- See-N-Say toy for input monitoring

**Pre-Flight Checklist**:
1. ✅ Verify your ESP32-S2 board model
2. ✅ Check pin assignments match your board schematic
3. ✅ Build ADC protection circuit on breadboard/protoboard
4. ✅ Test bias voltage (should be ~1.65V at idle)
5. ✅ Verify ADC readings before connecting See-N-Say
6. ✅ Prepare SD card with audio files (FAT32 format)

---

## Safety Warnings

⚠️ **ADC Input Protection**:
- Never connect speaker directly to ADC pin
- Maximum safe input: 3.3V absolute maximum
- Use protection circuit (adc-input-circuit.md) always
- Test bias voltage (~1.65V) before connecting speaker

⚠️ **Pin Conflicts**:
- Do not use GPIO 26-32 (reserved for flash memory)
- Be careful with GPIO 0, 45, 46 (strapping pins)
- Verify pins before wiring to avoid conflicts

⚠️ **Power Supply**:
- Ensure stable 3.3V power supply
- Estimated current draw: ~200mA during operation
- Use decoupling capacitors near ESP32 (100nF + 10µF)

---

## Implementation Status

| Component | Status | Documentation | Code |
|-----------|--------|---------------|------|
| ADC Input Circuit | ✅ Designed | [adc-input-circuit.md](./adc-input-circuit.md) | Pending |
| Pin Assignments | ✅ Documented | [pin-assignments.md](./pin-assignments.md) | In `main/config.h` |
| I2S Audio Output | ✅ Implemented | See code docs | `main/audio/i2s_output.c` |
| ADC Monitoring | 📝 Planned | In config.h | To be implemented |
| SD Card Support | 📝 Planned | In config.h | To be implemented |
| Audio Detection | 📝 Planned | See CLAUDE.md | To be implemented |

---

## Related Documentation

### Code Documentation
- [CLAUDE.md](../CLAUDE.md) - Main project documentation and architecture
- [main/config.h](../main/config.h) - GPIO and configuration definitions
- [main/README.md](../main/README.md) - Code structure and module organization

### Build & Development
- [Project README](../README.md) - Build instructions and project overview
- ESP-IDF Documentation: [ESP32-S2 Technical Reference](https://www.espressif.com/sites/default/files/documentation/esp32-s2_technical_reference_manual_en.pdf)

### Hardware References
- [ESP32-S2 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-s2_datasheet_en.pdf)
- [ESP32-S2 Pin Multiplexing](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/gpio.html)
- [ADC Specifications](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/adc.html)

---

## Contributing

When adding or modifying hardware specifications:

1. **Document thoroughly**: Include circuit diagrams, component values, and rationale
2. **Safety first**: Always include warnings about voltage limits and protection requirements
3. **Test before documenting**: Verify circuits work before committing to specs
4. **Update this index**: Add new spec files to the Documents section above
5. **ASCII art diagrams**: Use text-based diagrams that render in any markdown viewer

---

## Questions or Issues?

If you encounter problems with the hardware specifications or circuit designs:

1. Check the troubleshooting sections in each document
2. Verify component values and connections against schematics
3. Test step-by-step using the testing procedures provided
4. Review ESP32-S2 datasheet for GPIO capabilities and limits

**Remember**: Protect the ADC input! The See-N-Say speaker output (0-5V) must go through the protection circuit before connecting to GPIO 1.
