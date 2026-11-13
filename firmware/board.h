// Pin map for the GA100 glitcher.
//
// Define exactly one of BOARD_RP2350A / BOARD_RP2350B / BOARD_RP2040 at
// configure time. NVVDD_ADC: compile-time option (-DNVVDD_ADC) to enable ADC
// sampling of the NVVDD rail on the pin below. Without it, `v` reports
// nvvdd=N/A.
#ifndef BOARD_H
#define BOARD_H

#if (defined(BOARD_RP2350A) + defined(BOARD_RP2350B) + defined(BOARD_RP2040)) != 1
#error "define exactly one of BOARD_RP2350A / BOARD_RP2350B / BOARD_RP2040"
#endif

// I2C to the NVVDD controller (external pull-ups). Must be hardware I2C pins
// on the same controller (i2c0 or i2c1):
//   i2c0: SDA = GP % 4 == 0 (0, 4, 8, ...),  SCL = GP % 4 == 1 (1, 5, 9, ...)
//   i2c1: SDA = GP % 4 == 2 (2, 6, 10, ...), SCL = GP % 4 == 3 (3, 7, 11, ...)
// I2C_BUS selects i2c0 or i2c1 automatically from PIN_I2C_SDA.
#if defined(BOARD_RP2040)
// KB2040: use the castellated TX/RX pads (GP0/GP1 = i2c0), so no STEMMA QT
// cable is required and the I2C pins match the RP2350 defaults.
#define PIN_I2C_SDA 0
#define PIN_I2C_SCL 1
// KB2040 exposes roughly GP0-GP13 + GP26-GP29; GP22 is not broken out.
// Avoid GP11 (D11): it is the BOOT button.
#define PIN_PEX_RST 10
#else
#define PIN_I2C_SDA 0
#define PIN_I2C_SCL 1
// Fundamental reset (any free GPIO). Only driven low, then released to high-Z.
#define PIN_PEX_RST 22
#endif

#define I2C_BUS     (((PIN_I2C_SDA) & 2) ? i2c1 : i2c0)

// NVVDD sense (optional, requires -DNVVDD_ADC at compile time).
#if defined(BOARD_RP2350B)
#define PIN_NVVDD_ADC 42
#define NVVDD_ADC_BASE 40
#else
// RP2350A and RP2040: ADC pins are GP26-GP29.
#define PIN_NVVDD_ADC 26
#define NVVDD_ADC_BASE 26
#endif

#define NVVDD_ADC_CH (PIN_NVVDD_ADC - NVVDD_ADC_BASE)

#if defined(BOARD_RP2350A) && (PIN_NVVDD_ADC < 26 || PIN_NVVDD_ADC > 29)
#error "RP2350A ADC pins are GP26-GP29"
#endif
#if defined(BOARD_RP2350B) && (PIN_NVVDD_ADC < 40 || PIN_NVVDD_ADC > 47)
#error "RP2350B ADC pins are GP40-GP47"
#endif
#if defined(BOARD_RP2040) && (PIN_NVVDD_ADC < 26 || PIN_NVVDD_ADC > 29)
#error "RP2040 ADC pins are GP26-GP29"
#endif
#if (PIN_I2C_SDA % 4) != 0 && (PIN_I2C_SDA % 4) != 2
#error "PIN_I2C_SDA must be a hardware I2C SDA pin (GP % 4 == 0 for i2c0, GP % 4 == 2 for i2c1)"
#endif
#if (PIN_I2C_SCL % 4) != ((PIN_I2C_SDA % 4) + 1)
#error "PIN_I2C_SCL must be an SCL pin on the same hardware I2C controller as PIN_I2C_SDA"
#endif
#if defined(BOARD_RP2350A) && (PIN_I2C_SDA > 29 || PIN_I2C_SCL > 29 || PIN_PEX_RST > 29)
#error "RP2350A GPIOs are GP0-GP29"
#endif
#if defined(BOARD_RP2040) && (PIN_I2C_SDA > 29 || PIN_I2C_SCL > 29 || PIN_PEX_RST > 29)
#error "RP2040 GPIOs are GP0-GP29"
#endif

#endif  // BOARD_H
