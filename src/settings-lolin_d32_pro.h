#include "Arduino.h"

//######################### INFOS ####################################
/* This is a develboard-specific config-file for *Wemos Lolin32*. Specific doesn't mean it's only working with this board.
   Lolin32 is the predecessor of Lolin D32.
   PCB: None so far
   Infos: https://www.wemos.cc/en/latest/d32/d32_pro.html
   Schematics: https://www.wemos.cc/en/latest/_static/files/sch_d32_pro_v2.0.0.pdf
   Caveats: GPIO35 (battery monitoring) + SD can't be changed, it's built in (and because of the SD-pinout used, it is not compatible with MMC-mode)
   Status:
    tested with 2xSPI: RC522 & SD (by biologist79)
*/

//################## GPIO-configuration ##############################
// Please note: GPIOs 34, 35, 36, 39 are input-only and don't have pullup-resistors.
// So if connecting a button to these, make sure to add a 10k-pullup-resistor for each button.
// Further infos: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
// GPIOs 16+17 are not available for D32 pro as they're used to internal purposes (PSRAM).
#ifdef SD_MMC_1BIT_MODE
    // uSD-card-reader (via SD-MMC 1Bit)
    //
    // SD_MMC uses fixed pins
    //  MOSI    15
    //  SCKK    14
    //  MISO    2   // hardware pullup may required
#else
    // uSD-card-reader (via SPI) => Cannot be changed, it's built in!
    #define SPISD_CS                     4          // GPIO for chip select (SD)
    #ifndef SINGLE_SPI_ENABLE
        #define SPISD_MOSI              23          // GPIO for master out slave in (SD) => not necessary for single-SPI
        #define SPISD_MISO              19          // GPIO for master in slave ou (SD) => not necessary for single-SPI
        #define SPISD_SCK               18          // GPIO for clock-signal (SD) => not necessary for single-SPI
    #endif
#endif

// RFID (via SPI)
#define RST_PIN                         99          // Not necessary but has to be set anyway; so let's use a dummy-number
#define RFID_CS                         21          // GPIO for chip select (RFID)
#define RFID_MOSI                       23          // GPIO for master out slave in (RFID)
#define RFID_MISO                       19          // GPIO for master in slave out (RFID)
#define RFID_SCK                        18          // GPIO for clock-signal (RFID)

#ifdef RFID_READER_TYPE_PN5180
    #define RFID_BUSY                   33          // PN5180 BUSY PIN
    #define RFID_RST                    22          // PN5180 RESET PIN
    #define RFID_IRQ                    39          // PN5180 IRQ PIN (only needed for low power card detection)
	#define BUTTON_PIN_BITMASK          0x8000000000// 2^RFID_IRQ in hex
#endif
// I2S (DAC)
#define I2S_DOUT                        25          // Digital out (I2S)
#define I2S_BCLK                        27          // BCLK (I2S)
#define I2S_LRC                         26          // LRC (I2S)

// Rotary encoder
#define DREHENCODER_CLK                 32          // If you want to reverse encoder's direction, just switch GPIOs of CLK with DT (in software or hardware)
#define DREHENCODER_DT                  34          // Info: Lolin D32 / Lolin D32 pro 35 are using 35 for battery-voltage-monitoring!
#define DREHENCODER_BUTTON              33          // Button is used to switch Tonuino on and off

// Control-buttons
#define PAUSEPLAY_BUTTON                4           // GPIO to detect pause/play
#define NEXT_BUTTON                     0           // GPIO to detect next
#define PREVIOUS_BUTTON                 13           // GPIO to detect previous (Important: as of 19.11.2020 changed from 33 to 2)

// (optional) Power-control
#define POWER                           5           // GPIO used to drive transistor-circuit, that switches off peripheral devices while ESP32-deepsleep

// (optional) Neopixel
#define LED_PIN                         12          // GPIO for Neopixel-signaling

// (optinal) Headphone-detection
#ifdef HEADPHONE_ADJUST_ENABLE
    #define HP_DETECT                   22          // GPIO that detects, if there's a plug in the headphone jack or not
#endif

// (optional) Monitoring of battery-voltage via ADC
#ifdef MEASURE_BATTERY_VOLTAGE
    #define VOLTAGE_READ_PIN            35          // GPIO used to monitor battery-voltage. Cannot be changed, it's built in
    float referenceVoltage = 3.30;                  // Voltage between 3.3V and GND-pin at the develboard in battery-mode (disconnect USB!)
    float offsetVoltage = 0.1;                      // If voltage measured by ESP isn't 100% accurate, you can add an correction-value here
#endif

// (optional) For measuring battery-voltage a voltage-divider is already onboard. Connect a LiPo and use it!
#ifdef MEASURE_BATTERY_VOLTAGE
    uint8_t rdiv1 = 116;                               // Rdiv1 of voltage-divider (kOhms) (measure exact value with multimeter!)
    uint16_t rdiv2 = 100;                              // Rdiv2 of voltage-divider (kOhms) (measure exact value with multimeter!) => used to measure voltage via ADC!
#endif
