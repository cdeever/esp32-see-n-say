# ADC Input Circuit for See-N-Say Speaker Monitoring

## Overview

This circuit safely interfaces the See-N-Say speaker output (0-5V peak-to-peak audio signal) with the ESP32-S2 ADC input (0-3.3V maximum safe input). The circuit provides voltage division, DC offset removal, bias centering, and overvoltage protection.

**Purpose**: Monitor the See-N-Say speaker output to detect which of the 12 animal sounds is playing.

---

## Circuit Diagram

```
See-N-Say                                                    ESP32-S2
Speaker Output                                               ADC1_CH0
(0-5V p-p)                                                   (GPIO1)
    |
    |
    +--------[R1 = 10kΩ]--------+--------[C1 = 10µF]--------+--------[R3 = 1kΩ]--------+-------> ADC Input
                                |          (Electrolytic     |                          |         (0-3.3V)
                                |           16V or higher)   |                          |
                                |                            |                          |
                             [R2 = 10kΩ]                 [R4 = 10kΩ]              [D1] BAT54S
                                |                            |                   (Schottky Diode)
                                |                            |                    Dual Package:
                                |                            |                    - Anode to ADC
                               GND                         +3.3V                 - Cathode 1 to GND
                                                                                  - Cathode 2 to 3.3V


Signal Path:
1. Speaker out → Voltage divider (R1+R2) → Half amplitude
2. DC blocking (C1) → Remove DC offset
3. Bias point (R4) → Center signal at 1.65V
4. Series resistor (R3) → Current limiting
5. Clamping diodes (D1) → Overvoltage protection
6. ADC input → Safe 0-3.3V range
```

---

## Bill of Materials (BOM)

| Ref | Value | Type | Package | Part Number (Example) | Quantity | Notes |
|-----|-------|------|---------|----------------------|----------|-------|
| R1  | 10kΩ | Resistor | 1/4W | CF14JT10K0 (Stackpole) | 1 | ±5% tolerance OK |
| R2  | 10kΩ | Resistor | 1/4W | CF14JT10K0 (Stackpole) | 1 | ±5% tolerance OK |
| R3  | 1kΩ  | Resistor | 1/4W | CF14JT1K00 (Stackpole) | 1 | Current limiting |
| R4  | 10kΩ | Resistor | 1/4W | CF14JT10K0 (Stackpole) | 1 | Bias resistor |
| C1  | 10µF | Electrolytic Cap | Radial | ECA-1HM100 (Panasonic) | 1 | 16V or higher |
| D1  | BAT54S | Schottky Diode | SOT-23 | BAT54S (ON Semi) | 1 | Dual diode package |

**Alternative for D1**: If BAT54S is unavailable, use two individual 1N5819 Schottky diodes (DO-41 package).

**Total Cost**: Approximately $2-3 USD for all components.

---

## Design Rationale

### Problem Statement
- See-N-Say speaker output: **0-5V peak-to-peak** (amplified audio)
- ESP32-S2 ADC safe input: **0-3.3V maximum**
- Signal has DC offset from amplifier bias point
- Risk of overvoltage damage to ADC pin

### Solution Components

#### 1. Voltage Divider (R1 + R2)
**R1 = R2 = 10kΩ**

- Forms 2:1 voltage divider
- Input: 0-5V → Output: 0-2.5V peak-to-peak
- Divider ratio: `Vout = Vin × (R2 / (R1 + R2)) = Vin × 0.5`
- 10kΩ values chosen for:
  - Low power consumption (~0.25mA @ 5V)
  - High enough impedance to not load speaker output
  - Low enough to minimize noise pickup

#### 2. DC Blocking Capacitor (C1)
**C1 = 10µF electrolytic**

- Removes DC offset from speaker amplifier
- Passes AC audio signal (300Hz-3kHz range)
- Cutoff frequency: `fc = 1 / (2π × R_total × C1)`
  - With R_total ≈ 11kΩ (R3 + R4||ADC_input): `fc ≈ 1.45Hz`
  - Well below audio range (300Hz+), ensures full signal passes

**Polarity**: Positive terminal towards R1/speaker side

#### 3. Bias Resistor (R4)
**R4 = 10kΩ to +3.3V**

- Creates DC bias point at ~1.65V (midpoint of 0-3.3V range)
- Centers AC signal around ADC mid-scale
- Allows maximum signal swing without clipping
- Forms voltage divider with ADC input impedance (~47kΩ typical)

#### 4. Series Protection Resistor (R3)
**R3 = 1kΩ**

- Limits current into ADC pin if voltage exceeds safe range
- Max current (worst case): `I = (5V - 3.3V) / 1kΩ = 1.7mA`
- ESP32 ADC can handle this current without damage
- Doesn't significantly attenuate signal (voltage drop minimal)

#### 5. Clamping Diodes (D1)
**BAT54S Schottky diode (dual package)**

- Clamps voltage to 0V (lower diode) and 3.3V (upper diode)
- Schottky chosen for:
  - Low forward voltage (~0.3V) - doesn't clip signal unnecessarily
  - Fast response time - protects against transients
  - Low leakage - doesn't load signal
- Dual package saves space and simplifies assembly

---

## Voltage Analysis

### Input Conditions
- See-N-Say speaker: **0-5V peak-to-peak** AC signal
- Worst case: 5V peak amplitude

### Signal Path Calculations

**After Voltage Divider (R1+R2)**:
```
V_divided = 5V × (R2 / (R1 + R2))
          = 5V × (10kΩ / 20kΩ)
          = 2.5V peak-to-peak
```

**After DC Blocking + Bias**:
- C1 removes any DC component
- R4 creates 1.65V bias point: `Vbias = 3.3V × (ADC_Rin / (R4 + ADC_Rin))`
- With ADC input ~47kΩ: `Vbias ≈ 1.65V`
- Signal swings: **1.65V ± 1.25V**
- Range: **0.4V to 2.9V** (safely within 0-3.3V limit)

**Safety Margin**:
- Minimum voltage: 0.4V (0.4V above 0V floor)
- Maximum voltage: 2.9V (0.4V below 3.3V ceiling)
- Total headroom: **0.4V on each side**

**Protection Operation**:
- If signal exceeds 3.3V: Upper diode conducts, clamps to 3.3V + 0.3V = 3.6V (safe)
- If signal goes negative: Lower diode conducts, clamps to -0.3V (safe)
- R3 limits current during clamping to < 2mA

---

## Assembly Notes

### Construction Steps

1. **Start with voltage divider**:
   - Solder R1 and R2 in series
   - Connect junction to C1 positive terminal

2. **DC blocking**:
   - Solder C1 negative terminal to one end of R3
   - **Verify polarity**: Positive towards R1, negative towards R3

3. **Bias network**:
   - Connect R4 between +3.3V and R3/C1 junction
   - This creates the 1.65V bias point

4. **Protection**:
   - Solder BAT54S diode at ADC input point
   - **Orientation**: Common anode to ADC, cathodes to GND and 3.3V
   - Alternatively: Two 1N5819 diodes (cathode to ADC, anodes to GND/3.3V respectively)

5. **Final connections**:
   - Input wire to start of R1
   - Output wire from R3 to ESP32 GPIO1 (ADC1_CH0)
   - Ground wire from R2 to ESP32 GND
   - Power wire from R4 to ESP32 3.3V

### PCB Layout Tips (if applicable)

- Keep trace from C1 to ADC input short
- Place clamping diodes close to ADC pin
- Use ground plane under signal path if possible
- Separate analog ground from digital ground if mixed signal

### Breadboard/Protoboard Construction

- Use solid core wire for connections
- Keep speaker input wire away from ESP32 to minimize noise coupling
- Double-check polarity of C1 before power-on
- Twist speaker input wires if length > 6 inches

---

## Testing Procedure

### Equipment Needed
- Multimeter (DC voltage measurement)
- Function generator or audio source (optional)
- Oscilloscope (optional, for waveform verification)

### Step 1: Power-Off Resistance Check
1. Disconnect circuit from ESP32
2. Measure resistance from ADC output to GND: Should be ~10kΩ (R2 || R4 ≈ 5kΩ + R3)
3. Measure resistance across C1: Should be open circuit (> 1MΩ after charge)

### Step 2: Bias Voltage Verification
1. Connect circuit to ESP32 (powered)
2. **DO NOT** connect See-N-Say speaker yet
3. Measure voltage at ADC input point (between R3 and D1)
4. **Expected**: ~1.65V ± 0.1V
5. **Action**: If out of range, check R4 connection and 3.3V supply

### Step 3: Input Protection Test
1. With ESP32 powered, apply test voltages to input (R1 side):
   - Apply 0V: ADC should read ~1.65V (bias point)
   - Apply 3.3V: ADC should read ~1.65V + some AC component (within 0-3.3V)
   - Apply 5V: ADC should not exceed ~2.9V
2. **Safety**: Never exceed 5V input - may damage diodes

### Step 4: Audio Signal Test
1. Connect See-N-Say speaker to input
2. Activate See-N-Say (pull string, make sound)
3. Measure voltage at ADC with multimeter (DC setting)
   - **Expected**: 1.5V - 2.0V (slight variation as signal changes)
4. If available, use oscilloscope:
   - Should see clean audio waveform
   - Centered around 1.65V
   - Peak-to-peak amplitude 0.5V to 2.0V typical

### Step 5: ADC Reading Verification
1. Load firmware with ADC reading code
2. Monitor serial output for ADC values
3. **Expected range**: 500 - 3500 (out of 4095 for 12-bit ADC)
4. Signal should vary with See-N-Say audio

---

## Troubleshooting

| Symptom | Possible Cause | Solution |
|---------|----------------|----------|
| ADC reads constant 0V | C1 polarity reversed | Reverse C1 polarity |
| ADC reads constant 3.3V | Short circuit in diode | Check D1 connections |
| No bias voltage (~0V at idle) | R4 disconnected from 3.3V | Verify R4 to 3.3V connection |
| Signal clipped / distorted | Input voltage too high | Verify voltage divider R1=R2=10kΩ |
| ADC damaged (always reads 0 or 4095) | Overvoltage before protection | Replace ESP32, verify circuit |
| Noisy readings | Poor grounding | Use star ground, shorten wires |

---

## Safety Warnings

⚠️ **IMPORTANT**:
- Never connect See-N-Say speaker directly to ADC without this protection circuit
- Maximum ADC input: 3.3V (exceeding will damage ESP32)
- Verify all component values before power-on
- Test bias voltage (Step 2) before connecting See-N-Say speaker
- Do not exceed 5V input to protection circuit

---

## Related Documentation

- [Pin Assignments](./pin-assignments.md) - GPIO mapping reference
- [Hardware Specifications Index](./README.md) - Overview of all spec documents
- Code configuration: `../main/config.h` - ADC GPIO and attenuation settings
