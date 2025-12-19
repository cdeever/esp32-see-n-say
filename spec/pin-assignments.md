# ESP32-S2 Pin Assignments

Complete GPIO mapping for the ESP32 See-N-Say Audio Replacement project.

---

## Audio Output (I2S)

| Function | GPIO | Pin Name | Notes |
|----------|------|----------|-------|
| I2S Bit Clock (BCK) | 37 | IO37 | Serial clock for audio data |
| I2S Word Select (WS/LRCLK) | 33 | IO33 | Left/Right channel select |
| I2S Data Out (DO) | 34 | IO34 | Serial audio data output |

**Configuration**: Master TX mode, 16-bit samples, 44.1kHz sample rate
**Driver**: I2S driver in `main/audio/i2s_output.c`
**Defined in**: `main/config.h` - `CONFIG_GPIO_I2S_*`

---

## Audio Input (ADC)

| Function | GPIO | ADC Channel | Attenuation | Notes |
|----------|------|-------------|-------------|-------|
| See-N-Say Speaker Input | 1 | ADC1_CH0 | ADC_ATTEN_DB_11 | **Requires protection circuit** |

**Protection Circuit**: See [adc-input-circuit.md](./adc-input-circuit.md) for voltage divider, bias, and clamping circuit.

**Input Characteristics**:
- Safe voltage range: 0-3.3V (with attenuation: 0-2.45V recommended)
- Signal after protection: 0.4V - 2.9V typical
- ADC resolution: 12-bit (4096 levels)
- Planned sample rate: 8kHz for audio detection

**Configuration**:
- Defined in: `main/config.h` - `CONFIG_GPIO_ADC_INPUT`, `CONFIG_ADC_CHANNEL`, `CONFIG_ADC_ATTEN`
- Driver: Will be in `main/adc/adc_monitor.c` (to be implemented)

**⚠️ WARNING**: Never connect speaker output directly to this pin - use the protection circuit!

---

## SD Card (SPI)

| Function | GPIO | Pin Name | SPI Signal | Notes |
|----------|------|----------|------------|-------|
| MOSI (Master Out) | 11 | IO11 | SPI_MOSI | Data to SD card |
| MISO (Master In) | 13 | IO13 | SPI_MISO | Data from SD card |
| CLK (Clock) | 12 | IO12 | SPI_CLK | SPI clock |
| CS (Chip Select) | 10 | IO10 | SPI_CS | SD card select (active low) |

**⚠️ TBD**: Verify these pin assignments for your specific ESP32-S2 development board.
Some boards have dedicated SD card pins or use different GPIO for SPI.

**SD Card Configuration**:
- Interface: SPI mode (not SDMMC mode)
- Speed: 20MHz typical (adjust based on card quality)
- Voltage: 3.3V logic (most modern SD cards support this)
- File format: FAT32 recommended for compatibility
- Defined in: `main/config.h` - `CONFIG_GPIO_SD_*`

**Driver**: Will use ESP-IDF `esp_vfs_fat` and `sdmmc` components.

---

## Power

| Function | Pin Name | Voltage | Current | Notes |
|----------|----------|---------|---------|-------|
| Power Supply | 3V3 | 3.3V | ~200mA typical | Main power rail |
| Ground | GND | 0V | - | Multiple GND pins available |
| USB Power (if applicable) | 5V / VBUS | 5.0V | Up to 500mA | USB-powered boards only |

**Power Budget**:
- ESP32-S2 core: ~80mA active, ~5mA sleep
- I2S DAC/Amplifier: ~50mA typical (depends on external hardware)
- SD card: ~50mA active, ~1mA idle
- ADC input circuit: < 1mA
- **Total estimated**: ~200mA active operation

**Power Source Options**:
1. USB power (5V → 3.3V regulator on board)
2. External 3.3V regulated supply
3. Battery (with voltage regulation to 3.3V)

---

## Reserved / Unavailable Pins

### Strapping Pins (Boot Configuration)
**⚠️ Do not use for GPIO during boot**:

| GPIO | Default Function | Strapping Behavior | Notes |
|------|------------------|-------------------|-------|
| 0 | Boot Mode | LOW = Download mode, HIGH = Normal boot | Can use after boot |
| 45 | VDD_SPI voltage | Sets flash voltage (1.8V or 3.3V) | Usually tied by hardware |
| 46 | ROM Message Print | Controls boot message output | Usually tied by hardware |

**Recommendation**: Avoid using GPIO 0, 45, 46 unless you understand the boot implications.

### Flash / PSRAM Pins
**⚠️ Reserved for SPI flash memory (DO NOT USE)**:

| GPIO | Function | Notes |
|------|----------|-------|
| 26-32 | SPI Flash (SPICS0, SPIHD, SPIWP, SPID, SPIQ, SPICLK) | Internal flash memory |

**Recommendation**: Never connect external circuits to GPIO 26-32.

### USB Pins (if using USB)
If using USB for programming/debugging:

| GPIO | Function | Notes |
|------|----------|-------|
| 19 | USB D- | Reserved for USB interface |
| 20 | USB D+ | Reserved for USB interface |

**Recommendation**: Avoid GPIO 19-20 if using USB connectivity.

---

## Available GPIO Summary

### Currently Assigned
- GPIO 1: ADC input (See-N-Say speaker)
- GPIO 10: SD card CS
- GPIO 11: SD card MOSI
- GPIO 12: SD card CLK
- GPIO 13: SD card MISO
- GPIO 33: I2S WS
- GPIO 34: I2S DO
- GPIO 37: I2S BCK

### Available for Future Use
**Unused GPIO pins** (verify against your board schematic):
- GPIO 2-9 (excluding any used by board peripherals)
- GPIO 14-18
- GPIO 21
- GPIO 35-36, 38-39 (verify not used by dev board LEDs/buttons)
- GPIO 40-44 (some may be input-only on certain ESP32-S2 variants)

**Note**: Always check your specific development board schematic before using "available" pins - they may be connected to onboard LEDs, buttons, or other peripherals.

---

## Board-Specific Notes

### Common ESP32-S2 Development Boards

#### ESP32-S2-Saola-1
- Onboard LED: GPIO 18
- Boot button: GPIO 0
- Built-in USB: GPIO 19/20
- **Recommendation**: Verify SD card pins before wiring

#### ESP32-S2-DevKitC-1
- Similar to Saola-1
- Check board revision for specific pin assignments

#### ESP32-S2-Kaluga-1
- Has built-in SD card slot (different pins than specified above)
- Has onboard audio codec (may interfere with I2S)
- **Recommendation**: Consult Kaluga-1 schematic before using

**⚠️ IMPORTANT**: Always verify pin assignments against your specific board's schematic and documentation before wiring.

---

## Pin Assignment Verification Checklist

Before wiring your circuit:

- [ ] Verify your ESP32-S2 board model and revision
- [ ] Check board schematic for conflicting pin usage
- [ ] Confirm I2S pins (37, 33, 34) are not used by onboard peripherals
- [ ] Verify ADC pin (GPIO 1 / ADC1_CH0) is accessible
- [ ] Check SD card pins (10-13) match your board's SPI configuration
- [ ] Ensure no strapping pins (0, 45, 46) are used during boot
- [ ] Confirm flash pins (26-32) are not accidentally wired
- [ ] Test with multimeter: verify 3.3V and GND before connecting circuits
- [ ] Double-check protection circuit for ADC input (see adc-input-circuit.md)

---

## Related Documentation

- [ADC Input Circuit](./adc-input-circuit.md) - Protection circuit for speaker input
- [Hardware Specifications Index](./README.md) - Overview of all spec documents
- Code configuration: `../main/config.h` - GPIO pin definitions
- ESP-IDF Documentation: [ESP32-S2 Pin Multiplexing](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s2/api-reference/peripherals/gpio.html)
