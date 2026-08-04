/* OneWili C API - generated from FreeWili firmware sources. Do not edit. */
#ifndef ONEWILI_H
#define ONEWILI_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "onewili_enums.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    OW_OK = 0, OW_ERR_ARG, OW_ERR_IO, OW_ERR_TIMEOUT,
    OW_ERR_FAILED, OW_ERR_PROTOCOL, OW_ERR_BUFFER
} ow_status;

/* Callback transport: you supply the serial I/O (see examples/serial_pc.h).
 * write: return >=0 on success, <0 on error.
 * read : return bytes read (>0), 0 on timeout, <0 on error. */
typedef struct ow_transport {
    void* ctx;
    int (*write)(void* ctx, const uint8_t* data, size_t len);
    int (*read)(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms);
} ow_transport;

#ifndef OW_CMD_MAX
#define OW_CMD_MAX 1024
#endif
#ifndef OW_RESP_MAX
#define OW_RESP_MAX 4096
#endif
#ifndef OW_DEFAULT_TIMEOUT_MS
#define OW_DEFAULT_TIMEOUT_MS 5000
#endif

#ifndef OW_TEXT_EVENT_QUEUE
#define OW_TEXT_EVENT_QUEUE 8
#endif

typedef struct ow_device {
    ow_transport t;
    char line[OW_RESP_MAX];
    size_t line_len;
    /* Spontaneous text-event lines "[*<id> ...]" captured during calls or
     * polls; oldest dropped (and counted) when full. */
    char evq[OW_TEXT_EVENT_QUEUE][OW_RESP_MAX];
    uint32_t evq_head, evq_count;
    uint32_t dropped_text_events;
} ow_device;

/* Sends 0x02 (reset nav to root + quiet mode). */
ow_status ow_open(ow_device* dev, const ow_transport* transport);
void ow_close(ow_device* dev);

/* Poll for a spontaneous text event "[*<id> <args>]". Non-blocking (one
 * zero-timeout read pass). id/args receive the split line body.
 * Returns 1 = filled, 0 = none pending, negative = -(ow_status). */
int ow_poll_text_line(ow_device* dev, char* id, size_t id_cap,
                      char* args, size_t args_cap);

/* High. Sets a GPIO high.  Wire: i\g\s */
ow_status ow_io_gpio_set_io_high(ow_device* dev, int32_t pin);

/* Low. Sets a GPIO low.  Wire: i\g\l */
ow_status ow_io_gpio_set_io_low(ow_device* dev, int32_t pin);

/* Toggle. Toggles the specified GPIO.  Wire: i\g\t */
ow_status ow_io_gpio_set_io_toggle(ow_device* dev, int32_t pin);

/* PWM IO. Enables the PWM feature of GPIO.  Wire: i\g\p */
ow_status ow_io_gpio_set_pwm(ow_device* dev, int32_t gpio_number, double freq, double duty);

/* Get All IOs (hex). Reads all the IOs in a bitfield.  Wire: i\g\u */
ow_status ow_io_gpio_read_all(ow_device* dev, uint32_t* gpiostate);

/* Stream IO reads. Sends GPIO values as a specific millisecond rate to host.  Wire: i\g\o */
ow_status ow_io_gpio_stream_io(ow_device* dev, int32_t reportratems);

/* Toggle High-Speed Bidirectional IO. Toggle utilizing GPIO27 to set the direction of GPIO26..  Wire: i\g\e */
ow_status ow_io_gpio_toggle_hsbdio(ow_device* dev, int32_t pin);

/* Set IO Voltage Source. Selects the voltage source connected to the external IO voltage rail..  Wire: i\g\v */
ow_status ow_io_gpio_set_io_voltage_source(ow_device* dev, int32_t source);


/* Events emitted by GPIO Functions: */
/*   gpioReport (binary) - time_stamp_ns=hexU64, gpio_bitfield=hexU32 - Periodic GPIO bitfield report (binary API) */
#define OW_EVENT_IO_GPIO_GPIO_REPORT "gpioReport"
/* SPI1 Rx (12). IO direction for SPI1 Rx pin 12 (out/in).  Wire: i\g\a\a */
ow_status ow_io_gpio_io_direction_settings_s_pi1_rx12(ow_device* dev);

/* GPIO 26 (26). IO direction for GPIO 26 (out/in).  Wire: i\g\a\b */
ow_status ow_io_gpio_io_direction_settings_g_pio2626(ow_device* dev);

/* SPI1 CS (13). IO direction for SPI1 CS pin 13 (out/in).  Wire: i\g\a\c */
ow_status ow_io_gpio_io_direction_settings_s_pi1cs13(ow_device* dev);

/* GPIO (27). IO direction for GPIO 27 (out/in).  Wire: i\g\a\l */
ow_status ow_io_gpio_io_direction_settings_g_pio27(ow_device* dev);

/* UART1 Rx (9). IO direction for UART1 Rx pin 9 (out/in).  Wire: i\g\a\e */
ow_status ow_io_gpio_io_direction_settings_u_art1_rx9(ow_device* dev);

/* UART1 CTS (10). IO direction for UART1 CTS pin 10 (out/in).  Wire: i\g\a\f */
ow_status ow_io_gpio_io_direction_settings_u_art1cts10(ow_device* dev);

/* UART1 Tx (8). IO direction for UART1 Tx pin 8 (out/in).  Wire: i\g\a\g */
ow_status ow_io_gpio_io_direction_settings_u_art1_tx8(ow_device* dev);

/* UART1 RTS (11). IO direction for UART1 RTS pin 11 (out/in).  Wire: i\g\a\m */
ow_status ow_io_gpio_io_direction_settings_u_art1rts11(ow_device* dev);

/* SPI1 Tx (15). IO direction for SPI1 Tx pin 15 (out/in).  Wire: i\g\a\i */
ow_status ow_io_gpio_io_direction_settings_s_pi1_tx15(ow_device* dev);

/* SPI1 SCLK (14). IO direction for SPI1 SCLK pin 14 (out/in).  Wire: i\g\a\j */
ow_status ow_io_gpio_io_direction_settings_s_pi1sclk14(ow_device* dev);

/* GPIO25 (25). IO direction for GPIO 25 (out/in).  Wire: i\g\a\k */
ow_status ow_io_gpio_io_direction_settings_g_pio2525(ow_device* dev);

/* Write. Writes data to a specific I2C Address.  Wire: i\u\w */
ow_status ow_io_uart_u_art_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len);

/* Enable UART Read Events. Reads the number from the address.  Wire: i\u\r */
ow_status ow_io_uart_toggle_stream(ow_device* dev, uint8_t* data_bytes, size_t data_bytes_cap, size_t* data_bytes_len);

/* Enable UART API mode. Tests all addresses for I2C Response.  Wire: i\u\t */
ow_status ow_io_uart_uart_enable_api_mode(ow_device* dev);


/* Events emitted by UART Functions: */
/*   uart1 (text) - data_bytes=hexbytes - uart receive frame */
#define OW_EVENT_IO_UART_UART1 "uart1"
/* Baud Rate. UART baud rate in bits per second.  Wire: i\u\s\f */
ow_status ow_io_uart_settings_baud_rate(ow_device* dev, int32_t value);

/* RTS Hand Shaking. Enable RTS hardware handshaking.  Wire: i\u\s\r */
ow_status ow_io_uart_settings_r_ts_hand_shaking(ow_device* dev);

/* CTS Hand Shaking. Enable CTS hardware handshaking.  Wire: i\u\s\c */
ow_status ow_io_uart_settings_c_ts_hand_shaking(ow_device* dev);

/* Data Bits. UART data bits.  Wire: i\u\s\b */
ow_status ow_io_uart_settings_data_bits(ow_device* dev, int32_t value);

/* Parity. UART parity mode.  Wire: i\u\s\p */
ow_status ow_io_uart_settings_parity(ow_device* dev, int32_t value);

/* Stop Bits. UART stop bits.  Wire: i\u\s\s */
ow_status ow_io_uart_settings_stop_bits(ow_device* dev, int32_t value);

/* Module. Which UART module handles the port.  Wire: i\u\s\m */
ow_status ow_io_uart_settings_module(ow_device* dev, int32_t value);

/* SFP Poll. Polls for SFP Modules on the I2C bus. If any are found, return the PHY's temperature in Celsius and Signal Quality Indicator (SQI).  Wire: i\m\a */
ow_status ow_io_mdio_mdio_poll_sfp(ow_device* dev);

/* SFP Read. Reads a value from a register on the specified device address.  Wire: i\m\b */
ow_status ow_io_mdio_mdio_read_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, uint32_t* sfp_response);

/* SFP Write. Writes a value to a register on the specified device address.  Wire: i\m\c */
ow_status ow_io_mdio_mdio_write_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* data_bytes, size_t data_bytes_len);

/* SFP Read-Modify-Write. Read-Modify-Writes a value to a register on the specified device address. '1' bits in the mask indicate an overwrite.  Wire: i\m\e */
ow_status ow_io_mdio_mdiormwsfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);

/* PHY Address Poll. Polls all 32 possible PHY addresses. Test for a response from status register. Returns PHY addresses and clause compatibility.  Wire: i\m\y */
ow_status ow_io_mdio_mdio_poll(ow_device* dev);

/* Clause 22 Read. Reads a value from a register belonging to a Clause-22-Compatible-PHY.  Wire: i\m\g */
ow_status ow_io_mdio_mdio_read22(ow_device* dev, uint8_t phy_address, uint8_t register_address, uint32_t* mdio_response);

/* Clause 22 Write. Writes a value to a register belonging to a Clause-22-Compatible-PHY.  Wire: i\m\i */
ow_status ow_io_mdio_mdio_write22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* data_bytes, size_t data_bytes_len);

/* Clause 22 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite.  Wire: i\m\j */
ow_status ow_io_mdio_mdiormw22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);

/* Clause 45 Read. Reads a value from a register belonging to a Clause-45-Compatible-PHY.  Wire: i\m\k */
ow_status ow_io_mdio_mdio_read45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response);

/* Clause 45 Write. Writes a value to a register belonging to a Clause-45-Compatible-PHY.  Wire: i\m\l */
ow_status ow_io_mdio_mdio_write45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len);

/* Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Compatible-PHY. '1' bits in the mask indicate an overwrite.  Wire: i\m\m */
ow_status ow_io_mdio_mdiormw45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);

/* Clause 22 Access to Clause 45 Read. Reads a value from a register belonging to a Clause-45-Emulation-Compatible-PHY.  Wire: i\m\n */
ow_status ow_io_mdio_mdio_read_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response);

/* Clause 22 Access to Clause 45 Write. Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY.  Wire: i\m\o */
ow_status ow_io_mdio_mdio_write_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len);

/* Clause 22 Access to Clause 45 Read-Modify-Write. Read-Modify-Writes a value to a register belonging to a Clause-45-Emulation-Compatible-PHY. '1' bits in the mask indicate an overwrite.  Wire: i\m\p */
ow_status ow_io_mdio_mdiormw_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len);

/* Stream Motion. Streams accelerometer and gyroscope data to the host at the given rate. 0 stops the stream..  Wire: i\s\m */
ow_status ow_io_sensors_enable_motion_stream(ow_device* dev, int32_t stream_rate_ms);

/* Stream Field. Streams magnetometer data to the host at the given rate. 0 stops the stream..  Wire: i\s\f */
ow_status ow_io_sensors_enable_field_stream(ow_device* dev, int32_t stream_rate_ms);

/* Stream Env. Streams temperature, humidity and ambient light to the host. This stream is change-driven: the rate is a heartbeat floor, so samples can arrive faster when readings move. 0 stops the stream..  Wire: i\s\e */
ow_status ow_io_sensors_enable_env_stream(ow_device* dev, int32_t stream_rate_ms);

/* Stream Orientation. Streams fused roll, pitch, yaw and heading to the host at the given rate. 0 stops the stream..  Wire: i\s\r */
ow_status ow_io_sensors_enable_orientation_stream(ow_device* dev, int32_t stream_rate_ms);

/* Get Sensors. Prints the most recent sample from each of the four sensor groups..  Wire: i\s\g */
ow_status ow_io_sensors_get_sensors(ow_device* dev);


/* Events emitted by Sensor Functions: */
/*   motion (text) - ax_mg=decS32, ay_mg=decS32, az_mg=decS32, gx_ddps=decS32, gy_ddps=decS32, gz_ddps=decS32 - Accelerometer and gyroscope data */
#define OW_EVENT_IO_SENSORS_MOTION "motion"
/*   field (text) - mx_dut=decS32, my_dut=decS32, mz_dut=decS32, magnitude_dut=decS32, heading_cdeg=decS32 - Magnetometer data */
#define OW_EVENT_IO_SENSORS_FIELD "field"
/*   env (text) - temp_cc=decS32, rh_cpct=decS32, lux_clux=decU32 - Temperature, humidity and ambient light */
#define OW_EVENT_IO_SENSORS_ENV "env"
/*   orientation (text) - roll_cdeg=decS32, pitch_cdeg=decS32, yaw_cdeg=decS32, heading_cdeg=decS32, flags=decS32 - Fused roll, pitch, yaw and heading */
#define OW_EVENT_IO_SENSORS_ORIENTATION "orientation"
/* Write. Writes data to a specific I2C Address.  Wire: i\i\w */
ow_status ow_io_i2c_i2c_write(ow_device* dev, uint8_t address, uint8_t register_, const uint8_t* data_bytes, size_t data_bytes_len);

/* Read. Reads the number from the address.  Wire: i\i\r */
ow_status ow_io_i2c_i2c_read(ow_device* dev, uint8_t* i2crepsone, size_t i2crepsone_cap, size_t* i2crepsone_len);

/* Poll. Tests all addresses for I2C Response.  Wire: i\i\p */
ow_status ow_io_i2c_i2c_poll(ow_device* dev);


/* Events emitted by I2C Functions: */
/*   i2cmon (text) - data_bytes=hexbytes - I2C monitor captured bytes */
#define OW_EVENT_IO_I2C_I2CMON "i2cmon"
/* Frequency. I2C bus clock frequency in Hz.  Wire: i\i\s\f */
ow_status ow_io_i2c_settings_frequency(ow_device* dev, int32_t value);

/* PullUps. Enable I2C bus pull-up resistors.  Wire: i\i\s\p */
ow_status ow_io_i2c_settings_pull_ups(ow_device* dev, int32_t value);

/* Write and Read. Writes data to SPI and returns response data.  Wire: i\e\w */
ow_status ow_io_spi_s_pi_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len, uint8_t* spi_response, size_t spi_response_cap, size_t* spi_response_len);

/* Frequency. SPI clock frequency in Hz.  Wire: i\e\s\f */
ow_status ow_io_spi_settings_frequency(ow_device* dev, int32_t value);

/* Chip Select Pin. GPIO pin used as SPI chip select.  Wire: i\e\s\c */
ow_status ow_io_spi_settings_chip_select_pin(ow_device* dev, int32_t value);

/* Data Bits. SPI data bits per transfer.  Wire: i\e\s\b */
ow_status ow_io_spi_settings_data_bits(ow_device* dev, int32_t value);

/* CPOL. SPI clock polarity.  Wire: i\e\s\p */
ow_status ow_io_spi_settings_c_pol(ow_device* dev);

/* CPHA. SPI clock phase.  Wire: i\e\s\a */
ow_status ow_io_spi_settings_c_pha(ow_device* dev);

/* Stream CAN(FD). Streams received CAN frames and errors to the host..  Wire: i\c\o */
ow_status ow_io_canfd_enable_canfd_stream(ow_device* dev, int32_t channel, int32_t enabled);

/* Transmit CAN(FD). Transmits a CAN(FD) frame..  Wire: i\c\w */
ow_status ow_io_canfd_write_canfd(ow_device* dev, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len);

/* Transmit CAN(FD) Periodic. Transmits a CAN(FD) frame periodically (period in us; 0 = as fast as possible)..  Wire: i\c\p */
ow_status ow_io_canfd_write_canfd_periodic(ow_device* dev, int32_t index, int32_t enable, int32_t period, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len);

/* Setup Filter. Sets up a hardware receive filter (the byte-filter args are optional)..  Wire: i\c\f */
ow_status ow_io_canfd_setup_filter(ow_device* dev, int32_t channel, int32_t index, int32_t enable, int32_t xtd_id, uint32_t mask, uint32_t accept, uint32_t maskb0, uint32_t accept_b0, uint32_t maskb1, uint32_t accept_b1);

/* Read CAN Register(s). Reads 32-bit words from CAN controller SFR registers..  Wire: i\c\r */
ow_status ow_io_canfd_read_can_registers(ow_device* dev, int32_t channel, uint32_t start_address, int32_t word_count, char* registers, size_t registers_cap);

/* Set CAN Register. Sets a CAN controller register..  Wire: i\c\s */
ow_status ow_io_canfd_set_can_register(ow_device* dev, int32_t channel, uint32_t start_address, int32_t byte_count, uint32_t word_to_write);


/* Events emitted by CANFD Functions: */
/*   can0 (text) - arb_id=string, data_bytes=hexbytes - CAN RX frame on channel 0 (hex arb id, 'x' suffix = extended, then hex data) */
#define OW_EVENT_IO_CANFD_CAN0 "can0"
/*   can1 (text) - arb_id=string, data_bytes=hexbytes - CAN RX frame on channel 1 (hex arb id, 'x' suffix = extended, then hex data) */
#define OW_EVENT_IO_CANFD_CAN1 "can1"
/*   canTx0 (text) - arb_id=string, data_bytes=hexbytes - CAN TX echo on channel 0 (hex arb id, 'x' suffix = extended, then hex data) */
#define OW_EVENT_IO_CANFD_CAN_TX0 "canTx0"
/*   canTx1 (text) - arb_id=string, data_bytes=hexbytes - CAN TX echo on channel 1 (hex arb id, 'x' suffix = extended, then hex data) */
#define OW_EVENT_IO_CANFD_CAN_TX1 "canTx1"
/*   canRxReport (binary) - time_stamp_ns=hexU64, gpio_bitfield=hexU32, can_id=hexU32, header_bits=hexU32, data_words=hexbytes - CAN RX frame report (binary API, MCP2518 memory-map layout) */
#define OW_EVENT_IO_CANFD_CAN_RX_REPORT "canRxReport"
/* Stream Analog In. Streams analog input values to the host at the given rate..  Wire: i\j\s */
ow_status ow_io_analog_in_enable_analog_in_stream(ow_device* dev, int32_t stream_rate_ms);

/* Read TLA2024. Reads the latest TLA2024 voltages for all 4 channels..  Wire: i\j\r */
ow_status ow_io_analog_in_read_analog_in2024(ow_device* dev, double* v0, double* v1, double* v2, double* v3);

/* Config TLA2024 Channel. Configures a TLA2024 channel: mux 0-7 = A0-A1,A0-A3,A1-A3,A2-A3,A0-GND,A1-GND,A2-GND,A3-GND; range 0-5 = 6.144V,4.096V,2.048V,1.024V,0.512V,0.256V..  Wire: i\j\c */
ow_status ow_io_analog_in_config_analog_in2024(ow_device* dev, int32_t channel, int32_t mux, int32_t range);

/* TLA2024 Data Rate. Sets the TLA2024 data rate: 0-6 = 128,250,490,920,1600,2400,3300 SPS..  Wire: i\j\f */
ow_status ow_io_analog_in_set_data_rate2024(ow_device* dev, int32_t rate);

/* Stream TLA2024. Streams TLA2024 voltages to the host at the given rate (0 stops)..  Wire: i\j\t */
ow_status ow_io_analog_in_enable_analog_in2024_stream(ow_device* dev, int32_t stream_rate_ms);


/* Events emitted by Analog In Functions: */
/*   ainIn (text) - v0=float, v1=float, v2=float, v3=float - Internal ADC voltages (connector channels 0-3) */
#define OW_EVENT_IO_ANALOG_IN_AIN_IN "ainIn"
/*   adcIn (text) - v0=float, v1=float, v2=float, v3=float - TLA2024 voltages (connector channels 0-3) */
#define OW_EVENT_IO_ANALOG_IN_ADC_IN "adcIn"
/* Set Analog Output. sets the voltage of an analog output 0 or 1. ch 2 and 3 are use for window comparator.  Wire: i\a\s */
ow_status ow_io_analog_out_set_analog_output(ow_device* dev, int32_t channel, double value);

/* Set Trigger Window. Trigger will be 1 when TrigV is between V- and V+..  Wire: i\a\t */
ow_status ow_io_analog_out_set_trigger_window(ow_device* dev, double value_low, double value_high);

/* Enable Trigger. Enables the Trigger Input to CPU.  Wire: i\a\e */
ow_status ow_io_analog_out_set_enable_trigger(ow_device* dev);

/* Set Programmable VOut. Sets the programmable VOut: enable then target voltage..  Wire: i\a\u */
ow_status ow_io_analog_out_set_v_prog_vout(ow_device* dev, int32_t enable, double set_voltage);

/* Glitch Programmable VOut. Briefly glitches the programmable VOut for the given nanoseconds..  Wire: i\a\g */
ow_status ow_io_analog_out_set_glitch(ow_device* dev, int32_t nano_seconds);

/* Set Waveform. Configures and starts the DAC63204 function generator on analog output 0 or 1..  Wire: i\a\w */
ow_status ow_io_analog_out_set_waveform(ow_device* dev, int32_t channel, ow_dac_wave_shape_menu waveform, double frequency_hz, double low_voltage, double high_voltage, ow_dac_wave_phase phase);

/* Waveform Run/Stop. Starts or stops the configured waveforms on analog outputs 0 and 1 in a single write..  Wire: i\a\x */
ow_status ow_io_analog_out_set_waveform_run(ow_device* dev, int32_t mask);

/* configure. Configures digital playback.  Wire: i\p\c */
ow_status ow_io_logic_player_setup_player(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t start_mode, int32_t trigger_pin, bool loop);

/* configure analog. Configures DAC playback.  Wire: i\p\a */
ow_status ow_io_logic_player_setup_analog(ow_device* dev, int32_t mask, int32_t analog_rate_ns, int32_t analog_resolution);

/* load. Loads a raw buffer from the filesystem.  Wire: i\p\l */
ow_status ow_io_logic_player_load_file(ow_device* dev, const char* file_path);

/* start. Starts playback.  Wire: i\p\s */
ow_status ow_io_logic_player_start(ow_device* dev);

/* stop. Stops playback.  Wire: i\p\e */
ow_status ow_io_logic_player_stop(ow_device* dev);

/* configure. Configures the logic analyzer capture..  Wire: i\b\c */
ow_status ow_io_logic_analyzer_setup_logic_analyzer(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t trigger_pin, int32_t trigger_type, int32_t rearm);

/* configure analog. Configures the analog capture inputs..  Wire: i\b\a */
ow_status ow_io_logic_analyzer_setup_analog(ow_device* dev, int32_t analog_mask, int32_t analog_rate_ns, int32_t analog_res);

/* start. Starts logic analyzer capture..  Wire: i\b\s */
ow_status ow_io_logic_analyzer_start(ow_device* dev);

/* stop. Stops logic analyzer capture..  Wire: i\b\e */
ow_status ow_io_logic_analyzer_stop(ow_device* dev);

/* trigger. Manually triggers the logic analyzer..  Wire: i\b\t */
ow_status ow_io_logic_analyzer_trigger(ow_device* dev, int32_t trigger_type);


/* Events emitted by Logic Analyzer Functions: */
/*   logicAnalyzerReport (binary) - trigger_time_stamp_ns=hexU64, sample_rate_ns=decU32, samples=hexbytes - Logic analyzer capture report (binary API; header then digital + analog samples) */
#define OW_EVENT_IO_LOGIC_ANALYZER_LOGIC_ANALYZER_REPORT "logicAnalyzerReport"
/* Take a Picture. Take a picture from WILEye and save its SD card or FREE-WILi's Files system by file name..  Wire: i\f\t */
ow_status ow_io_wil_eye_take_picture(ow_device* dev, int32_t destination, const char* filename);

/* Start Recording Video. Start recording video from WILEye and save it to SD card by file name.  Wire: i\f\v */
ow_status ow_io_wil_eye_start_recording_video(ow_device* dev, const char* filename);

/* Stop Recording Video. Stop recording video from WILEye.  Wire: i\f\s */
ow_status ow_io_wil_eye_stop_recording_video(ow_device* dev);

/* Stream AI Detection Events. Stream AI Detection Events from WILEye.  Wire: i\f\a */
ow_status ow_io_wil_eye_toggle_ai_detection_stream(ow_device* dev, int32_t ai_stream_mode);

/* Set Zoom. Set the zoom level of WILEye.  Wire: i\f\m */
ow_status ow_io_wil_eye_set_zoom_level(ow_device* dev, int32_t zoom);

/* Set Contrast. Set the contrast level of WILEye.  Wire: i\f\c */
ow_status ow_io_wil_eye_set_contrast(ow_device* dev, int32_t contrast);

/* Set Saturation. Set the saturation level of WILEye.  Wire: i\f\i */
ow_status ow_io_wil_eye_set_saturation(ow_device* dev, int32_t saturation);

/* Set Brightness. Set the brightness level of WILEye.  Wire: i\f\b */
ow_status ow_io_wil_eye_set_brightness(ow_device* dev, int32_t brightness);

/* Set Hue. Set the hue level of WILEye.  Wire: i\f\u */
ow_status ow_io_wil_eye_set_hue(ow_device* dev, int32_t hue);

/* Set Resolution. Set the resolution state of WILEye.  Wire: i\f\y */
ow_status ow_io_wil_eye_set_resolution(ow_device* dev, int32_t resolutionstate);

/* Enable Disable Flash. Set the flash state of WILEye.  Wire: i\f\l */
ow_status ow_io_wil_eye_set_flash_state(ow_device* dev, bool flash);


/* Events emitted by WILEye Functions: */
/*   WILEye (text) - data_bytes=hexbytes - DEPRECATED: never emitted; kept for wire compatibility (see WILEyeAI) */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE "WILEye"
/*   WILEyeImgStart (text) - data=string - WILEye image transfer started ('Image Stream Start: N bytes') */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_IMG_START "WILEyeImgStart"
/*   WILEyeImgChunk (text) - data=string - WILEye image chunk received ('N bytes') */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_IMG_CHUNK "WILEyeImgChunk"
/*   WILEyeImgEnd (text) - data=string - WILEye image transfer complete ('saved as: <file>') */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_IMG_END "WILEyeImgEnd"
/*   WILEyeImgAbort (text) - data=string - WILEye image transfer aborted (timeout) */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_IMG_ABORT "WILEyeImgAbort"
/*   WILEyeSDcard (text) - data=string - WILEye SD card switched to USB mode */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_S_DCARD "WILEyeSDcard"
/*   WILEyeAI (text) - data=string - WILEye AI detection event (mode + bounding box text) */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_AI "WILEyeAI"
/*   WILEyeUnknown (text) - data=string - WILEye unknown message received */
#define OW_EVENT_IO_WIL_EYE_WIL_EYE_UNKNOWN "WILEyeUnknown"
/* Play Audio File. Plays a .wav file from the sounds directory..  Wire: i\k\f */
ow_status ow_io_audio_play_audio_file(ow_device* dev, const char* file_path);

/* Record Audio. Records audio to a file (blank name = auto-named)..  Wire: i\k\r */
ow_status ow_io_audio_record_audio_file(ow_device* dev, const char* file_name);

/* Play Audio Asset. Plays a built-in audio asset by index or name..  Wire: i\k\a */
ow_status ow_io_audio_play_audio_asset(ow_device* dev, const char* asset_name);

/* Stream Audio. Enables or disables audio streaming to the host..  Wire: i\k\s */
ow_status ow_io_audio_enable_audio_stream(ow_device* dev, int32_t enable);

/* Numbers to Speech. Speaks the given number aloud..  Wire: i\k\n */
ow_status ow_io_audio_numbers_to_speech(ow_device* dev, double number);

/* Play Tone. Plays a tone of the given frequency, duration, and amplitude..  Wire: i\k\t */
ow_status ow_io_audio_tone(ow_device* dev, double frequency, double duration_ms, double amplitude);

/* Text to Speech. Speaks the given text aloud (text to speech)..  Wire: i\k\v */
ow_status ow_io_audio_speak(ow_device* dev, const char* text);


/* Events emitted by Audio Functions: */
/*   record (text) - progress=decU32 - Sound recording progress (permille of the clip length) */
#define OW_EVENT_IO_AUDIO_RECORD "record"
/*   audio (text) - s0=decS32, s1=decS32, s2=decS32, s3=decS32, s4=decS32, s5=decS32, s6=decS32, s7=decS32 - PDM microphone sample batch (8 signed samples per event) */
#define OW_EVENT_IO_AUDIO_AUDIO "audio"
/* Configure Strip. Configure one of 8 serial LED strips: 0-based strip index, external GPIO (0=disabled; valid: 8-17,25,26,27), LED count (1-1024), LED type (rgb=3-byte WS2812, rgbw=4-byte SK6812), inverted polarity flag.  Wire: i\l\c */
ow_status ow_io_serial_leds_configure_strip(ow_device* dev, int32_t strip, int32_t gpio, int32_t length, ow_ow_serial_led_type led_type, bool inverted);

/* Show Config. Prints the configuration of all 8 serial LED strips and PSRAM buffer availability.  Wire: i\l\s */
ow_status ow_io_serial_leds_show_config(ow_device* dev);

/* Set LEDs. Sets a run of LEDs on a strip to an RGB(W) value: strip 0-7, start index, repeat count, then red/green/blue/white 0-255 (white ignored on 3-byte strips).  Wire: i\l\v */
ow_status ow_io_serial_leds_set_leds(ow_device* dev, int32_t strip, int32_t start, int32_t count, int32_t red, int32_t green, int32_t blue, int32_t white);

/* Set Show. Runs a light show pattern on one strip (0-7) or all strips (-1).  Wire: i\l\w */
ow_status ow_io_serial_leds_set_show(ow_device* dev, int32_t strip, ow_ow_led_light_show show);

/* Enable Jambu Orca. Configures strips 1..N for the Jambu Orca 8-channel LED breakout (GPIOs 13,14,11,15,26,25,9,10).  Wire: i\l\j */
ow_status ow_io_serial_leds_enable_jambu_orca(ow_device* dev, int32_t num_strips);

/* Auto Show. Automatically run the light show selected in the Light Show app on all serial LED strips.  Wire: i\l\a */
ow_status ow_io_serial_leds_auto_show(ow_device* dev);

/* Set Board LED. Sets a led to a specific color.  Wire: g\s */
ow_status ow_gui_set_led_color(ow_device* dev, int32_t ledindex, int32_t red, int32_t green, int32_t blue, int32_t duration, ow_ow_led_manager_led_mode mode);

/* Show FWI Image. Shows an freewili image (fwi) file from the file system..  Wire: g\l */
ow_status ow_gui_show_fwi_image(ow_device* dev, const char* filename);

/* Reset Display. Clears any GUI menu actions done to the display such as show image or show text.  Wire: g\t */
ow_status ow_gui_clear_display(ow_device* dev);

/* Show Text Display. Show text on the free wili display.  Wire: g\p */
ow_status ow_gui_show_text(ow_device* dev, const char* texttodisplay);

/* Read All Buttons. Sets the baud rate for I2C in Hz.  Wire: g\u */
ow_status ow_gui_read_all(ow_device* dev);

/* Stream Buttons. Sends GPIO values as a specific rate to host.  Wire: g\o */
ow_status ow_gui_stream_io(ow_device* dev, int32_t pin);

/* Show Asset Image. Reads the number from the address.  Wire: g\a */
ow_status ow_gui_show_image_asset_by_id(ow_device* dev, int32_t image_id);

/* Screenshot. Saves the display to SD. Args: filename, png/fwi, counter 0/1, timestamp 0/1..  Wire: g\i */
ow_status ow_gui_screenshot(ow_device* dev, const char* filename, ow_ow_screenshot_file_type filetype, bool counter, bool timestamp);

/* Simulate Keypress. Injects a button action into the DISPLAY GUI..  Wire: g\k */
ow_status ow_gui_simulate_keypress(ow_device* dev, ow_ow_gui_button button, ow_ow_button_press_type presstype);


/* Events emitted by GUI Functions: */
/*   button (text) - gray=decU32, yellow=decU32, green=decU32, blue=decU32, red=decU32 - Button state report (1 = pressed, per button) */
#define OW_EVENT_GUI_BUTTON "button"
/* Add Panel. Reinitializes the custom panel for controls..  Wire: g\c\a */
ow_status ow_gui_panels_add_panel(ow_device* dev, bool use_tile, int32_t tile_id, const char* color, bool show_menu);

/* Add Panel Picklist. Shows a panel that allows user to pick from a list..  Wire: g\c\b */
ow_status ow_gui_panels_add_panel_picklist(ow_device* dev, bool use_tile, int32_t tile_id, int32_t icon_id, int32_t log_index, const char* back_color, const char* fore_color, const char* caption);

/* Show Panel.  Wire: g\c\c */
ow_status ow_gui_panels_show_panel(ow_device* dev, int32_t index);

/* Add LED. Add a LED control to the panel..  Wire: g\b\a */
ow_status ow_gui_controls_add_led(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t color, int32_t size, bool inital_value);

/* Add LogList. Adds a Log control or a list control to the panel..  Wire: g\b\b */
ow_status ow_gui_controls_add_log_list(ow_device* dev, int32_t index, int32_t log, int32_t x, int32_t y, int32_t width, int32_t height, int32_t font_type, int32_t font_size, const char* back_color, const char* fore_color, bool list_mode);

/* Add Plot. Adds a plot to the panel..  Wire: g\b\c */
ow_status ow_gui_controls_add_plot(ow_device* dev, int32_t index, int32_t plot_data_index_bit_field, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min_y, int32_t max_y, const char* back_color);

/* Add Number. add a numeric control to a panel.  Wire: g\b\l */
ow_status ow_gui_controls_add_number(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, bool is_float, int32_t float_digit_count, bool is_hex_format, bool is_unsigned);

/* Add Text. Add static text to the panel.  Wire: g\b\e */
ow_status ow_gui_controls_add_text(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, const char* text);

/* Add Bargraph. Add a bar graph to a panel..  Wire: g\b\f */
ow_status ow_gui_controls_add_bargraph(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* bar_color);

/* Add Meter. Add a Meter control to a panel.  Wire: g\b\g */
ow_status ow_gui_controls_add_meter(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* needle_color);

/* Add Button. Add a button control to a panel.  Wire: g\b\i */
ow_status ow_gui_controls_add_button(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, const char* fore_color, const char* back_color, const char* text);

/* Add Picture. Shows a ROM picture on the panel..  Wire: g\b\j */
ow_status ow_gui_controls_add_picture(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t picture_id);

/* Add Picture From File. Loads a picture from the file system.  Wire: g\b\k */
ow_status ow_gui_controls_add_picture_from_file(ow_device* dev, int32_t index, int32_t x, int32_t y, const char* picture_path);

/* Add Waterfall. Adds an FFT waterfall (spectrogram) control to the panel. Rows commit when the control value changes..  Wire: g\b\m */
ow_status ow_gui_controls_add_waterfall(ow_device* dev, int32_t index, int32_t plot_data_index, int32_t bin_count, int32_t x, int32_t y, int32_t width, int32_t height, const char* back_color);

/* Add Wili8. Adds a clipped, integer-scaled Wili8 canvas control. Animation 0 is Wave; 255 stores ScriptPath for future custom execution..  Wire: g\b\n */
ow_status ow_gui_controls_add_wili8(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t scale, const char* back_color, int32_t animation, const char* script_path);

/* Add File List. Adds a device-fed SD/flash file browser control. Activating a file (or OK in pick dir mode) raises a filepicked event with the full path..  Wire: g\b\o */
ow_status ow_gui_controls_add_file_list(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t mode, const char* back_color, const char* start_path, const char* filter);

/* Set Control Value Text. sets the text value of a control.  Wire: g\e\a */
ow_status ow_gui_control_properties_set_control_value_text(ow_device* dev, int32_t index, const char* text);

/* Set Control Value Int. Set the text value of a control.  Wire: g\e\b */
ow_status ow_gui_control_properties_set_control_value_int(ow_device* dev, int32_t index, int32_t value);

/* Set Control Value Float. Set the float value of the control..  Wire: g\e\c */
ow_status ow_gui_control_properties_set_control_value_float(ow_device* dev, int32_t index, double value);

/* Set List Item Text. Sets the text and color of a specific list item.  Wire: g\e\k */
ow_status ow_gui_control_properties_set_list_item_text(ow_device* dev, int32_t log_index, int32_t list_item, int32_t color, const char* text);

/* Set Control Value Min Max Int. Sets whether a min and max is applied to a controls value.  Wire: g\e\e */
ow_status ow_gui_control_properties_set_control_value_min_max_int(ow_device* dev, int32_t index, bool enable, int32_t min, int32_t max);

/* Set Control Value Min Max Float. Sets whether a min and max is applied to a controls value.  Wire: g\e\l */
ow_status ow_gui_control_properties_set_control_value_min_max_float(ow_device* dev, int32_t index, bool enable, double min, double max);

/* Set Plot Data. This adds data to a plot.  Wire: g\e\f */
ow_status ow_gui_control_properties_set_plot_data(ow_device* dev, int32_t plot_data_index, int32_t settings, int32_t value);

/* Set List Item Selected. This sets which item in a list is selected..  Wire: g\e\g */
ow_status ow_gui_control_properties_set_list_item_selected(ow_device* dev, int32_t log_index, int32_t list_index);

/* Set List Item Top Index. This sets the first viewable item in the list. .  Wire: g\e\i */
ow_status ow_gui_control_properties_set_list_item_top_index(ow_device* dev, int32_t log_item, int32_t list_index);

/* Set Control Property. Sets a property based on a property type index.  Wire: g\e\j */
ow_status ow_gui_control_properties_set_control_property(ow_device* dev, int32_t index, int32_t property, int32_t value);

/* Message Box. Shows a message box with optional buttons and auto close timer. .  Wire: g\f\a */
ow_status ow_gui_dialogs_message_box(ow_device* dev, int32_t auto_close_half_sec, bool show_ok, bool show_ok_cancel, bool show_none, int32_t picture_index, const char* message);

/* Set Dialog Description. Sets the description of the dialog..  Wire: g\f\b */
ow_status ow_gui_dialogs_set_dialog_description(ow_device* dev, const char* description);

/* Progress Bar. shows a dialog with a progress bar.  Wire: g\f\c */
ow_status ow_gui_dialogs_progress_bar(ow_device* dev, int32_t picture_index, bool ok_to_close, bool auto_close_at100, int32_t auto_close_half_sec, const char* title);

/* Number Edit. Shows a dialog box to edit numbers.  Wire: g\f\k */
ow_status ow_gui_dialogs_number_edit(ow_device* dev, int32_t min, int32_t max, int32_t initial, bool use_min_max, bool is_unsigned, bool hex_fomat, const char* message);

/* Number Edit Float. Shows a dialog to enter a float number.  Wire: g\f\e */
ow_status ow_gui_dialogs_number_edit_float(ow_device* dev, double min, double max, double initial, bool use_min_max, int32_t digit_count, const char* message);

/* Text Edit. Shows a dialog to edit a text value..  Wire: g\f\f */
ow_status ow_gui_dialogs_text_edit(ow_device* dev, const char* message, const char* inital_value);

/* Pick List. Shows a list of items to pick from. The list of items is loaded into a log..  Wire: g\f\g */
ow_status ow_gui_dialogs_pick_list(ow_device* dev, int32_t log_index, const char* message);

/* Show Text Editor. Shows a full screen text editor..  Wire: g\f\i */
ow_status ow_gui_dialogs_show_text_editor(ow_device* dev, int32_t editor_type, const char* message, const char* inital_value, bool* basic);

/* Set Progess Dialog Value. Sets the value of progress on the dialog.  Wire: g\f\j */
ow_status ow_gui_dialogs_set_progess_dialog_value(ow_device* dev, int32_t value0_to100);

/* File Picker. Shows a full screen file browser dialog over the current view. The chosen path (or cancel) returns as a filepicked event..  Wire: g\f\l */
ow_status ow_gui_dialogs_file_picker(ow_device* dev, int32_t mode, const char* start_path, const char* filter);


/* Events emitted by Dialogs: */
/*   filepicked (text) - panel=decS32, control=decS32, picked=bool, path=string - File list / file picker result */
#define OW_EVENT_GUI_DIALOGS_FILEPICKED "filepicked"
/* Software Reset. Performs a software reset of the device..  Wire: h\s\1 */
ow_status ow_hardware_settings_home_software_reset(ow_device* dev);

/* Reset To Bootloader. Resets the device into the USB bootloader..  Wire: h\s\2 */
ow_status ow_hardware_settings_home_software_reset_to_bootloader(ow_device* dev);

/* All Settings To Defaults. Restores all settings to their default values..  Wire: h\s\3 */
ow_status ow_hardware_settings_home_all_settings_to_defaults(ow_device* dev);

/* Baud Rate. UART baud rate in bits per second.  Wire: h\s\u\f */
ow_status ow_hardware_settings_home_uart_settings_baud_rate(ow_device* dev, int32_t value);

/* RTS Hand Shaking. Enable RTS hardware handshaking.  Wire: h\s\u\r */
ow_status ow_hardware_settings_home_uart_settings_r_ts_hand_shaking(ow_device* dev);

/* CTS Hand Shaking. Enable CTS hardware handshaking.  Wire: h\s\u\c */
ow_status ow_hardware_settings_home_uart_settings_c_ts_hand_shaking(ow_device* dev);

/* Data Bits. UART data bits.  Wire: h\s\u\b */
ow_status ow_hardware_settings_home_uart_settings_data_bits(ow_device* dev, int32_t value);

/* Parity. UART parity mode.  Wire: h\s\u\p */
ow_status ow_hardware_settings_home_uart_settings_parity(ow_device* dev, int32_t value);

/* Stop Bits. UART stop bits.  Wire: h\s\u\s */
ow_status ow_hardware_settings_home_uart_settings_stop_bits(ow_device* dev, int32_t value);

/* Module. Which UART module handles the port.  Wire: h\s\u\m */
ow_status ow_hardware_settings_home_uart_settings_module(ow_device* dev, int32_t value);

/* Frequency. I2C bus clock frequency in Hz.  Wire: h\s\i\f */
ow_status ow_hardware_settings_home_i2c_settings_frequency(ow_device* dev, int32_t value);

/* PullUps. Enable I2C bus pull-up resistors.  Wire: h\s\i\p */
ow_status ow_hardware_settings_home_i2c_settings_pull_ups(ow_device* dev, int32_t value);

/* Accel Range. Accelerometer full-scale range index.  Wire: h\s\v\a */
ow_status ow_hardware_settings_home_sensor_settings_accel_range(ow_device* dev, int32_t value);

/* Gyro Range. Gyroscope full-scale range index.  Wire: h\s\v\g */
ow_status ow_hardware_settings_home_sensor_settings_gyro_range(ow_device* dev, int32_t value);

/* Move Threshold. The amount accel must change to signal movement.  Wire: h\s\v\m */
ow_status ow_hardware_settings_home_sensor_settings_move_threshold(ow_device* dev, int32_t value);

/* TCal Scale. Temperature calibration, the m of mX+b.  Wire: h\s\v\s */
ow_status ow_hardware_settings_home_sensor_settings_t_cal_scale(ow_device* dev, double value);

/* TCal Offset. Temperature calibration, the b of mX+b.  Wire: h\s\v\o */
ow_status ow_hardware_settings_home_sensor_settings_t_cal_offset(ow_device* dev, double value);

/* Stream Defaults. Bitmask of sensor streams enabled at boot: 1 accel-legacy, 2 temp, 4 motion, 8 field, 16 env, 32 orientation.  Wire: h\s\v\b */
ow_status ow_hardware_settings_home_sensor_settings_stream_defaults(ow_device* dev, int32_t value);

/* Frequency. SPI clock frequency in Hz.  Wire: h\s\s\f */
ow_status ow_hardware_settings_home_spi_settings_frequency(ow_device* dev, int32_t value);

/* Chip Select Pin. GPIO pin used as SPI chip select.  Wire: h\s\s\c */
ow_status ow_hardware_settings_home_spi_settings_chip_select_pin(ow_device* dev, int32_t value);

/* Data Bits. SPI data bits per transfer.  Wire: h\s\s\b */
ow_status ow_hardware_settings_home_spi_settings_data_bits(ow_device* dev, int32_t value);

/* CPOL. SPI clock polarity.  Wire: h\s\s\p */
ow_status ow_hardware_settings_home_spi_settings_c_pol(ow_device* dev);

/* CPHA. SPI clock phase.  Wire: h\s\s\a */
ow_status ow_hardware_settings_home_spi_settings_c_pha(ow_device* dev);

/* SPI1 Rx (12). IO direction for SPI1 Rx pin 12 (out/in).  Wire: h\s\o\a */
ow_status ow_hardware_settings_home_io_direction_settings_s_pi1_rx12(ow_device* dev);

/* GPIO 26 (26). IO direction for GPIO 26 (out/in).  Wire: h\s\o\b */
ow_status ow_hardware_settings_home_io_direction_settings_g_pio2626(ow_device* dev);

/* SPI1 CS (13). IO direction for SPI1 CS pin 13 (out/in).  Wire: h\s\o\c */
ow_status ow_hardware_settings_home_io_direction_settings_s_pi1cs13(ow_device* dev);

/* GPIO (27). IO direction for GPIO 27 (out/in).  Wire: h\s\o\l */
ow_status ow_hardware_settings_home_io_direction_settings_g_pio27(ow_device* dev);

/* UART1 Rx (9). IO direction for UART1 Rx pin 9 (out/in).  Wire: h\s\o\e */
ow_status ow_hardware_settings_home_io_direction_settings_u_art1_rx9(ow_device* dev);

/* UART1 CTS (10). IO direction for UART1 CTS pin 10 (out/in).  Wire: h\s\o\f */
ow_status ow_hardware_settings_home_io_direction_settings_u_art1cts10(ow_device* dev);

/* UART1 Tx (8). IO direction for UART1 Tx pin 8 (out/in).  Wire: h\s\o\g */
ow_status ow_hardware_settings_home_io_direction_settings_u_art1_tx8(ow_device* dev);

/* UART1 RTS (11). IO direction for UART1 RTS pin 11 (out/in).  Wire: h\s\o\m */
ow_status ow_hardware_settings_home_io_direction_settings_u_art1rts11(ow_device* dev);

/* SPI1 Tx (15). IO direction for SPI1 Tx pin 15 (out/in).  Wire: h\s\o\i */
ow_status ow_hardware_settings_home_io_direction_settings_s_pi1_tx15(ow_device* dev);

/* SPI1 SCLK (14). IO direction for SPI1 SCLK pin 14 (out/in).  Wire: h\s\o\j */
ow_status ow_hardware_settings_home_io_direction_settings_s_pi1sclk14(ow_device* dev);

/* GPIO25 (25). IO direction for GPIO 25 (out/in).  Wire: h\s\o\k */
ow_status ow_hardware_settings_home_io_direction_settings_g_pio2525(ow_device* dev);

/* Clk Source. Choose the clock source that drives the FPGA (CPU clock, oscillator, USB, or RTC).  Wire: h\s\f\c */
ow_status ow_hardware_settings_home_fpga_clock_settings_clk_source(ow_device* dev, int32_t value);

/* Clk Divider (int). Set the integer part of the clock divider used to derive the FPGA clock frequency.  Wire: h\s\f\i */
ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_int(ow_device* dev, int32_t value);

/* Clk Divider (Frac). Set the fractional part of the clock divider used to fine-tune the FPGA clock frequency.  Wire: h\s\f\f */
ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_frac(ow_device* dev, int32_t value);

/* Comms Mode. Choose whether the CPU talks to the FPGA configuration registers over SPI or I2C.  Wire: h\s\f\m */
ow_status ow_hardware_settings_home_fpga_clock_settings_comms_mode(ow_device* dev, int32_t value);

/* FrequencyMhz. basic frequency calculated automatically (default = 433.92). The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ.  Wire: h\s\r\f */
ow_status ow_hardware_settings_home_radio_settings_frequency_mhz(ow_device* dev, int32_t value);

/* Modulation. set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.  Wire: h\s\r\m */
ow_status ow_hardware_settings_home_radio_settings_modulation(ow_device* dev, int32_t value);

/* Devation. Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz..  Wire: h\s\r\a */
ow_status ow_hardware_settings_home_radio_settings_devation(ow_device* dev, int32_t value);

/* Channel. Channelnumber from 0 to 255. Default is channel 0.  Wire: h\s\r\b */
ow_status ow_hardware_settings_home_radio_settings_channel(ow_device* dev, int32_t value);

/* ChannelSpacing. channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz..  Wire: h\s\r\c */
ow_status ow_hardware_settings_home_radio_settings_channel_spacing(ow_device* dev, int32_t value);

/* RxBandwidth. Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz..  Wire: h\s\r\y */
ow_status ow_hardware_settings_home_radio_settings_rx_bandwidth(ow_device* dev, int32_t value);

/* DataRate. Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud.  Wire: h\s\r\e */
ow_status ow_hardware_settings_home_radio_settings_data_rate(ow_device* dev, int32_t value);

/* PowerAmp. TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max.  Wire: h\s\r\g */
ow_status ow_hardware_settings_home_radio_settings_power_amp(ow_device* dev, int32_t value);

/* SyncMode. Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync- carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold..  Wire: h\s\r\1 */
ow_status ow_hardware_settings_home_radio_settings_sync_mode(ow_device* dev, int32_t value);

/* SyncWord. sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low).  Wire: h\s\r\i */
ow_status ow_hardware_settings_home_radio_settings_sync_word(ow_device* dev, int32_t value);

/* AddrCheck. Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast..  Wire: h\s\r\j */
ow_status ow_hardware_settings_home_radio_settings_addr_check(ow_device* dev, int32_t value);

/* Address. Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF)..  Wire: h\s\r\k */
ow_status ow_hardware_settings_home_radio_settings_address(ow_device* dev, int32_t value);

/* WhiteData. Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on..  Wire: h\s\r\l */
ow_status ow_hardware_settings_home_radio_settings_white_data(ow_device* dev);

/* PacketFormat. Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins..  Wire: h\s\r\n */
ow_status ow_hardware_settings_home_radio_settings_packet_format(ow_device* dev, int32_t value);

/* LengthConfig. 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved.  Wire: h\s\r\o */
ow_status ow_hardware_settings_home_radio_settings_length_config(ow_device* dev, int32_t value);

/* PacketLength. Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed..  Wire: h\s\r\p */
ow_status ow_hardware_settings_home_radio_settings_packet_length(ow_device* dev, int32_t value);

/* CRCEnabled. 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX..  Wire: h\s\r\x */
ow_status ow_hardware_settings_home_radio_settings_c_rc_enabled(ow_device* dev);

/* CRCAutoFlush. Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size..  Wire: h\s\r\0 */
ow_status ow_hardware_settings_home_radio_settings_c_rc_auto_flush(ow_device* dev);

/* DCBlockingFilter. Disable digital DC blocking filter before demodulator. Only for data rates <= 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity)..  Wire: h\s\r\r */
ow_status ow_hardware_settings_home_radio_settings_d_c_blocking_filter(ow_device* dev);

/* Manchester. Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable..  Wire: h\s\r\s */
ow_status ow_hardware_settings_home_radio_settings_manchester(ow_device* dev);

/* ForwordErrorCorrection. Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable..  Wire: h\s\r\t */
ow_status ow_hardware_settings_home_radio_settings_forword_error_correction(ow_device* dev);

/* PreambleBytes. Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24.  Wire: h\s\r\u */
ow_status ow_hardware_settings_home_radio_settings_preamble_bytes(ow_device* dev, int32_t value);

/* PQT. Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4-PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted..  Wire: h\s\r\v */
ow_status ow_hardware_settings_home_radio_settings_p_qt(ow_device* dev, int32_t value);

/* AppendStatus. When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK..  Wire: h\s\r\w */
ow_status ow_hardware_settings_home_radio_settings_append_status(ow_device* dev);

/* FrequencyMhz. basic frequency calculated automatically (default = 433.92). The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ.  Wire: h\s\t\f */
ow_status ow_hardware_settings_home_radio_settings_2_frequency_mhz(ow_device* dev, int32_t value);

/* Modulation. set modulation mode. 0 = 2-FSK, 1 = GFSK, 2 = ASK/OOK, 3 = 4-FSK, 4 = MSK.  Wire: h\s\t\m */
ow_status ow_hardware_settings_home_radio_settings_2_modulation(ow_device* dev, int32_t value);

/* Devation. Frequency deviation in kHz. Value from 1.58 to 380.85. Default is 47.60 kHz..  Wire: h\s\t\a */
ow_status ow_hardware_settings_home_radio_settings_2_devation(ow_device* dev, int32_t value);

/* Channel. Channelnumber from 0 to 255. Default is channel 0.  Wire: h\s\t\b */
ow_status ow_hardware_settings_home_radio_settings_2_channel(ow_device* dev, int32_t value);

/* ChannelSpacing. channel spacing is multiplied by the channel number CHAN and added to the base frequency in kHz. Value from 25.39 to 405.45. Default is 199.95 kHz..  Wire: h\s\t\c */
ow_status ow_hardware_settings_home_radio_settings_2_channel_spacing(ow_device* dev, int32_t value);

/* RxBandwidth. Receive Bandwidth in kHz. Value from 58.03 to 812.50. Default is 812.50 kHz..  Wire: h\s\t\y */
ow_status ow_hardware_settings_home_radio_settings_2_rx_bandwidth(ow_device* dev, int32_t value);

/* DataRate. Data Rate in kBaud. Value from 0.02 to 1621.83. Default is 99.97 kBaud.  Wire: h\s\t\e */
ow_status ow_hardware_settings_home_radio_settings_2_data_rate(ow_device* dev, int32_t value);

/* PowerAmp. TxPower. The following settings are possible depending on the frequency band.  (-30  -20  -15  -10  -6    0    5    7    10   11   12) Default is max.  Wire: h\s\t\g */
ow_status ow_hardware_settings_home_radio_settings_2_power_amp(ow_device* dev, int32_t value);

/* SyncMode. Combined sync-word qualifier mode. 0 = No preamble/sync. 1 = 16 sync word bits detected. 2 = 16/16 sync word bits detected. 3 = 30/32 sync word bits detected. 4 = No preamble/sync- carrier-sense above threshold. 5 = 15/16 + carrier-sense above threshold. 6 = 16/16 + carrier-sense above threshold. 7 = 30/32 + carrier-sense above threshold..  Wire: h\s\t\1 */
ow_status ow_hardware_settings_home_radio_settings_2_sync_mode(ow_device* dev, int32_t value);

/* SyncWord. sync word. Must be the same for the transmitter and receiver. (Syncword high, Syncword low).  Wire: h\s\t\i */
ow_status ow_hardware_settings_home_radio_settings_2_sync_word(ow_device* dev, int32_t value);

/* AddrCheck. Controls address check configuration of received packages. 0 = No address check. 1 = Address check, no broadcast. 2 = Address check and 0 (0x00) broadcast. 3 = Address check and 0 (0x00) and 255 (0xFF) broadcast..  Wire: h\s\t\j */
ow_status ow_hardware_settings_home_radio_settings_2_addr_check(ow_device* dev, int32_t value);

/* Address. Address used for packet filtration. Optional broadcast addresses are 0 (0x00) and 255 (0xFF)..  Wire: h\s\t\k */
ow_status ow_hardware_settings_home_radio_settings_2_address(ow_device* dev, int32_t value);

/* WhiteData. Turn data whitening on / off. 0 = Whitening off. 1 = Whitening on..  Wire: h\s\t\l */
ow_status ow_hardware_settings_home_radio_settings_2_white_data(ow_device* dev);

/* PacketFormat. Format of RX and TX data. 0 = Normal mode, use FIFOs for RX and TX. 1 = Synchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins. 2 = Random TX mode; sends random data using PN9 generator. Used for test. Works as normal mode, setting 0 (00), in RX. 3 = Asynchronous serial mode, Data in on GDO0 and data out on either of the GDOx pins..  Wire: h\s\t\n */
ow_status ow_hardware_settings_home_radio_settings_2_packet_format(ow_device* dev, int32_t value);

/* LengthConfig. 0 = Fixed packet length mode. 1 = Variable packet length mode. 2 = Infinite packet length mode. 3 = Reserved.  Wire: h\s\t\o */
ow_status ow_hardware_settings_home_radio_settings_2_length_config(ow_device* dev, int32_t value);

/* PacketLength. Indicates the packet length when fixed packet length mode is enabled. If variable packet length mode is used, this value indicates the maximum packet length allowed..  Wire: h\s\t\p */
ow_status ow_hardware_settings_home_radio_settings_2_packet_length(ow_device* dev, int32_t value);

/* CRCEnabled. 1 = CRC calculation in TX and CRC check in RX enabled. 0 = CRC disabled for TX and RX..  Wire: h\s\t\x */
ow_status ow_hardware_settings_home_radio_settings_2_c_rc_enabled(ow_device* dev);

/* CRCAutoFlush. Enable automatic flush of RX FIFO when CRC is not OK. This requires that only one packet is in the RXIFIFO and that packet length is limited to the RX FIFO size..  Wire: h\s\t\0 */
ow_status ow_hardware_settings_home_radio_settings_2_c_rc_auto_flush(ow_device* dev);

/* DCBlockingFilter. Disable digital DC blocking filter before demodulator. Only for data rates <= 250 kBaud The recommended IF frequency changes when the DC blocking is disabled. 1 = Disable (current optimized). 0 = Enable (better sensitivity)..  Wire: h\s\t\r */
ow_status ow_hardware_settings_home_radio_settings_2_d_c_blocking_filter(ow_device* dev);

/* Manchester. Enables Manchester encoding/decoding. 0 = Disable. 1 = Enable..  Wire: h\s\t\s */
ow_status ow_hardware_settings_home_radio_settings_2_manchester(ow_device* dev);

/* ForwordErrorCorrection. Enable Forward Error Correction (FEC) with interleaving for packet payload (Only supported for fixed packet length mode. 0 = Disable. 1 = Enable..  Wire: h\s\t\t */
ow_status ow_hardware_settings_home_radio_settings_2_forword_error_correction(ow_device* dev);

/* PreambleBytes. Sets the minimum number of preamble bytes to be transmitted. Values: 0 : 2, 1 : 3, 2 : 4, 3 : 6, 4 : 8, 5 : 12, 6 : 16, 7 : 24.  Wire: h\s\t\u */
ow_status ow_hardware_settings_home_radio_settings_2_preamble_bytes(ow_device* dev, int32_t value);

/* PQT. Preamble quality estimator threshold. The preamble quality estimator increases an internal counter by one each time a bit is received that is different from the previous bit, and decreases the counter by 8 each time a bit is received that is the same as the last bit. A threshold of 4-PQT for this counter is used to gate sync word detection. When PQT=0 a sync word is always accepted..  Wire: h\s\t\v */
ow_status ow_hardware_settings_home_radio_settings_2_p_qt(ow_device* dev, int32_t value);

/* AppendStatus. When enabled, two status bytes will be appended to the payload of the packet. The status bytes contain RSSI and LQI values, as well as CRC OK..  Wire: h\s\t\w */
ow_status ow_hardware_settings_home_radio_settings_2_append_status(ow_device* dev);

/* Default View. Default view for the RF Analyzer.  Wire: h\s\a\a */
ow_status ow_hardware_settings_home_radio_fa_settings_default_view(ow_device* dev, int32_t value);

/* Year. Set the year on the real-time clock.  Wire: h\s\c\y */
ow_status ow_hardware_settings_home_rtc_settings_year(ow_device* dev, int32_t value);

/* Month. Set the month on the real-time clock.  Wire: h\s\c\n */
ow_status ow_hardware_settings_home_rtc_settings_month(ow_device* dev, int32_t value);

/* Day. Set the day of the month on the real-time clock.  Wire: h\s\c\e */
ow_status ow_hardware_settings_home_rtc_settings_day(ow_device* dev, int32_t value);

/* Day Of Week. Set the day of the week on the real-time clock.  Wire: h\s\c\w */
ow_status ow_hardware_settings_home_rtc_settings_day_of_week(ow_device* dev, int32_t value);

/* Hours. Set the hour on the real-time clock (24-hour format).  Wire: h\s\c\o */
ow_status ow_hardware_settings_home_rtc_settings_hours(ow_device* dev, int32_t value);

/* Minutes. Set the minutes on the real-time clock.  Wire: h\s\c\m */
ow_status ow_hardware_settings_home_rtc_settings_minutes(ow_device* dev, int32_t value);

/* Seconds. Set the seconds on the real-time clock.  Wire: h\s\c\s */
ow_status ow_hardware_settings_home_rtc_settings_seconds(ow_device* dev, int32_t value);

/* Trim. Add or subtract n*2 clock cycles every minute.  Wire: h\s\c\t */
ow_status ow_hardware_settings_home_rtc_settings_trim(ow_device* dev, int32_t value);

/* Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it.  Wire: h\s\w\s */
ow_status ow_hardware_settings_home_wifi_settings_enable_station_mode(ow_device* dev);

/* SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode.  Wire: h\s\w\e */
ow_status ow_hardware_settings_home_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value);

/* Password for Station Mode. Set the password used to join the Wi-Fi network in station mode.  Wire: h\s\w\p */
ow_status ow_hardware_settings_home_wifi_settings_password_for_station_mode(ow_device* dev, const char* value);

/* Enable AP Mode. Turn the device's own Wi-Fi access point on or off.  Wire: h\s\w\a */
ow_status ow_hardware_settings_home_wifi_settings_enable_ap_mode(ow_device* dev);

/* AP Auth. Choose the Wi-Fi security type used by the device's own access point.  Wire: h\s\w\u */
ow_status ow_hardware_settings_home_wifi_settings_a_p_auth(ow_device* dev, int32_t value);

/* AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices.  Wire: h\s\w\i */
ow_status ow_hardware_settings_home_wifi_settings_a_p_hide_ssid(ow_device* dev);

/* SSID for AP. Set the network name (SSID) broadcast by the device's own access point.  Wire: h\s\w\g */
ow_status ow_hardware_settings_home_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value);

/* Password for AP. Set the password required to join the device's own access point.  Wire: h\s\w\x */
ow_status ow_hardware_settings_home_wifi_settings_password_for_ap(ow_device* dev, const char* value);

/* Enable BT. Turn Bluetooth LE on or off.  Wire: h\s\b\s */
ow_status ow_hardware_settings_home_ble_settings_enable_bt(ow_device* dev);

/* BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead.  Wire: h\s\b\t */
ow_status ow_hardware_settings_home_ble_settings_b_t_terminal(ow_device* dev);

/* BT Advert Name. Set the name the device advertises over Bluetooth LE.  Wire: h\s\b\a */
ow_status ow_hardware_settings_home_ble_settings_b_t_advert_name(ow_device* dev, const char* value);

/* Orca Com over UART. Set Communication protocol for connected Orca device over UART.  Wire: h\s\g\u */
ow_status ow_hardware_settings_home_orca_settings_orca_com_over_uart(ow_device* dev, int32_t value);

/* Start WS Server. Turn the websocket server on or off.  Wire: h\s\k\r */
ow_status ow_hardware_settings_home_websocket_settings_start_ws_server(ow_device* dev);

/* WS Server Port. Set the TCP port the websocket server listens on.  Wire: h\s\k\p */
ow_status ow_hardware_settings_home_websocket_settings_w_s_server_port(ow_device* dev, int32_t value);

/* Auth Mode. Choose whether the websocket server allows open access or requires a username and password.  Wire: h\s\k\m */
ow_status ow_hardware_settings_home_websocket_settings_auth_mode(ow_device* dev, int32_t value);

/* Auth Username. Set the username required to connect to the websocket server when basic authentication is enabled.  Wire: h\s\k\u */
ow_status ow_hardware_settings_home_websocket_settings_auth_username(ow_device* dev, const char* value);

/* Auth Password. Set the password required to connect to the websocket server when basic authentication is enabled.  Wire: h\s\k\e */
ow_status ow_hardware_settings_home_websocket_settings_auth_password(ow_device* dev, const char* value);

/* CAN1 Mode. CAN Type or UART over CAN PHY.  Wire: h\s\p\a */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_mode(ow_device* dev, int32_t value);

/* CAN1 Rate. Baudrate of CAN or UART over CAN PHY.  Wire: h\s\p\b */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_rate(ow_device* dev, int32_t value);

/* CAN1 FD D Rate. Baud Rate for CANFD Data section.  Wire: h\s\p\c */
ow_status ow_hardware_settings_home_neptune_settings_c_an1fdd_rate(ow_device* dev, int32_t value);

/* CAN1 Listen Only. Enables Listen Only mode.  Wire: h\s\p\y */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_listen_only(ow_device* dev);

/* CAN1 Tx Retry. CAN Transmit retry options.  Wire: h\s\p\e */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_tx_retry(ow_device* dev, int32_t value);

/* CAN1 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable..  Wire: h\s\p\f */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_baud(ow_device* dev, const char* value);

/* CAN1 Cust Data Baud. Hex Value String for Register C1DBTCFG.  Wire: h\s\p\g */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_data_baud(ow_device* dev, const char* value);

/* CAN1 Termination. Enables termination for network..  Wire: h\s\p\1 */
ow_status ow_hardware_settings_home_neptune_settings_c_an1_termination(ow_device* dev);

/* CAN1 API Enabled. Set Wili API Base ID.  Wire: h\s\p\i */
ow_status ow_hardware_settings_home_neptune_settings_c_an1api_enabled(ow_device* dev);

/* CAN API ID. Enables Terminal over CANFD.  Wire: h\s\p\j */
ow_status ow_hardware_settings_home_neptune_settings_c_anapiid(ow_device* dev, int32_t value);

/* CAN2 Mode. CAN Type or UART over CAN PHY.  Wire: h\s\p\k */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_mode(ow_device* dev, int32_t value);

/* CAN2 Rate. Baudrate of CAN or UART over CAN PHY.  Wire: h\s\p\l */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_rate(ow_device* dev, int32_t value);

/* CAN2 FD D Rate. Baud Rate for CANFD Data section.  Wire: h\s\p\m */
ow_status ow_hardware_settings_home_neptune_settings_c_an2fdd_rate(ow_device* dev, int32_t value);

/* CAN2 Listen Only. Enables Listen Only mode.  Wire: h\s\p\n */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_listen_only(ow_device* dev);

/* CAN2 Tx Retry. CAN Transmit retry options.  Wire: h\s\p\o */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_tx_retry(ow_device* dev, int32_t value);

/* CAN2 Cust Baud. Hex Value String for Register C1NBTCFG. Blank to disable..  Wire: h\s\p\p */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_baud(ow_device* dev, const char* value);

/* CAN2 Cust Data Baud. Hex Value String for Register C1DBTCFG.  Wire: h\s\p\r */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_data_baud(ow_device* dev, const char* value);

/* CAN2 Termination. Enables termination for network..  Wire: h\s\p\s */
ow_status ow_hardware_settings_home_neptune_settings_c_an2_termination(ow_device* dev);

/* CAN2 API Enabled. Enables Wili API over CANFD.  Wire: h\s\p\t */
ow_status ow_hardware_settings_home_neptune_settings_c_an2api_enabled(ow_device* dev);

/* LIN Master En. Enables LIN Master Pull Resistor.  Wire: h\s\p\u */
ow_status ow_hardware_settings_home_neptune_settings_l_in_master_en(ow_device* dev);

/* LIN Baud Rate. Baud Rate for LIN.  Wire: h\s\p\v */
ow_status ow_hardware_settings_home_neptune_settings_l_in_baud_rate(ow_device* dev, int32_t value);

/* Analog In En. Enables analog input measurement.  Wire: h\s\p\x */
ow_status ow_hardware_settings_home_neptune_settings_analog_in_en(ow_device* dev);

/* Startup Wasm Script. Path to wasm or RTHON script..  Wire: h\s\e\a */
ow_status ow_hardware_settings_home_general_settings_startup_wasm_script(ow_device* dev, const char* value);

/* Startup Zoom Script. Path to zoom script..  Wire: h\s\e\b */
ow_status ow_hardware_settings_home_general_settings_startup_zoom_script(ow_device* dev, const char* value);

/* Default FPGA Script. Path to FPGA bit file.  Wire: h\s\e\c */
ow_status ow_hardware_settings_home_general_settings_default_fpga_script(ow_device* dev, const char* value);

/* Wasm debug level. Debug messaging from WiliWasm.  Wire: h\s\e\f */
ow_status ow_hardware_settings_home_general_settings_wasm_debug_level(ow_device* dev, int32_t value);

/* Ch0 Input. TLA2024 channel 0 input mux.  Wire: h\s\j\0 */
ow_status ow_hardware_settings_home_analog_in_settings_ch0_input(ow_device* dev, int32_t value);

/* Ch1 Input. TLA2024 channel 1 input mux.  Wire: h\s\j\1 */
ow_status ow_hardware_settings_home_analog_in_settings_ch1_input(ow_device* dev, int32_t value);

/* Ch2 Input. TLA2024 channel 2 input mux.  Wire: h\s\j\2 */
ow_status ow_hardware_settings_home_analog_in_settings_ch2_input(ow_device* dev, int32_t value);

/* Ch3 Input. TLA2024 channel 3 input mux.  Wire: h\s\j\3 */
ow_status ow_hardware_settings_home_analog_in_settings_ch3_input(ow_device* dev, int32_t value);

/* Ch0 Range. TLA2024 channel 0 full-scale range.  Wire: h\s\j\4 */
ow_status ow_hardware_settings_home_analog_in_settings_ch0_range(ow_device* dev, int32_t value);

/* Ch1 Range. TLA2024 channel 1 full-scale range.  Wire: h\s\j\5 */
ow_status ow_hardware_settings_home_analog_in_settings_ch1_range(ow_device* dev, int32_t value);

/* Ch2 Range. TLA2024 channel 2 full-scale range.  Wire: h\s\j\6 */
ow_status ow_hardware_settings_home_analog_in_settings_ch2_range(ow_device* dev, int32_t value);

/* Ch3 Range. TLA2024 channel 3 full-scale range.  Wire: h\s\j\7 */
ow_status ow_hardware_settings_home_analog_in_settings_ch3_range(ow_device* dev, int32_t value);

/* Data Rate. TLA2024 conversion data rate.  Wire: h\s\j\8 */
ow_status ow_hardware_settings_home_analog_in_settings_data_rate(ow_device* dev, int32_t value);

/* Stream Battery Info. Enables or disables streaming of battery info to the host..  Wire: h\a\o */
ow_status ow_hardware_system_enable_battery_stream(ow_device* dev, int32_t enable);

/* Read OTP Info. Reads bytes from the fused OTP identity blob (bl_otp_info v3). An unprovisioned device reads all zeros. Read in chunks of 256 bytes or less..  Wire: h\a\b */
ow_status ow_hardware_system_read_otp_info(ow_device* dev, int32_t offset, int32_t length, uint8_t* otp_blob, size_t otp_blob_cap, size_t* otp_blob_len);

/* Device State. Report the device state for host sync: SD card host (none|main|usb), event host-streaming gate (0|1), active-stream mask (hex, bit index = event id). More space-separated fields may be appended later..  Wire: h\a\g */
ow_status ow_hardware_system_device_state(ow_device* dev, char* sd, size_t sd_cap, bool* hoststream, char* activemask, size_t activemask_cap);

/* Event Host Streaming. Enables or disables streaming of events to the host. When disabled, stream-class events are suppressed at the host output; protocol events still flow. Same gate as control bytes 0x05 (off) and 0x06 (on)..  Wire: h\a\e */
ow_status ow_hardware_system_event_host_streaming(ow_device* dev, int32_t enable, bool* enabled);


/* Events emitted by System Functions: */
/*   battery (text) - data=string - Battery charger status text */
#define OW_EVENT_HARDWARE_SYSTEM_BATTERY "battery"
/* Change Directory. Changes current directory.  Wire: h\x\a */
ow_status ow_hardware_file_system_change_directory(ow_device* dev, const char* path);

/* Create Directory. Creates a new directory.  Wire: h\x\c */
ow_status ow_hardware_file_system_create_directory(ow_device* dev, const char* path);

/* Remove File or Directory. Removes a file or directory.  Wire: h\x\r */
ow_status ow_hardware_file_system_remove_file_or_directory(ow_device* dev, const char* path);

/* Get File From PC. Downloads file to Free Wili.  Wire: h\x\f */
ow_status ow_hardware_file_system_get_file_from_pc(ow_device* dev, const char* path, int32_t size, int32_t crc32);

/* Send File To PC. Sends file to PC.  Wire: h\x\u */
ow_status ow_hardware_file_system_send_file_to_pc(ow_device* dev, const char* path);

/* Print File. Prints the File Content.  Wire: h\x\p */
ow_status ow_hardware_file_system_print_file(ow_device* dev, const char* path);

/* Create Blank File. Creates a blank file.  Wire: h\x\b */
ow_status ow_hardware_file_system_create_blank_file(ow_device* dev, const char* path);

/* Edit File. Edits a text file.  Wire: h\x\e */
ow_status ow_hardware_file_system_edit_file(ow_device* dev, const char* path);

/* Rename or Move File Or Directory. Renames or Moves a File or Directory.  Wire: h\x\n */
ow_status ow_hardware_file_system_rename_or_move_file_directory(ow_device* dev, const char* path, const char* new_path);

/* List Directory. lists the contents of a directory. Blank for current directory..  Wire: h\x\l */
ow_status ow_hardware_file_system_list_directory(ow_device* dev, const char* path);

/* Format File System. reformats the internal flash.  Wire: h\x\t */
ow_status ow_hardware_file_system_format_file_system(ow_device* dev, const char* confirm);

/* Toggle SDCard Host. Toggles which host controls the SD card..  Wire: h\x\s */
ow_status ow_hardware_file_system_toggle_sd_card_host_select(ow_device* dev);

/* Load Wili Project. Loads a fwcom .wili project (panels, blocks, app signals) and shows the Panels app..  Wire: h\x\w */
ow_status ow_hardware_file_system_load_wili_project(ow_device* dev, const char* path);

/* SDCard Host Select. Connects the SD card to the main CPU (0) or the USB reader / PC (1)..  Wire: h\x\k */
ow_status ow_hardware_file_system_set_sd_card_host(ow_device* dev, int32_t host);


/* Events emitted by File System: */
/*   fdir (text) - kind=string, name=string, size=decU32 - directory listing entry; kind is dir/fil, or end with size as the entry count */
#define OW_EVENT_HARDWARE_FILE_SYSTEM_FDIR "fdir"
/*   filedl (text) - data=string - File download progress ('complete N bytes') */
#define OW_EVENT_HARDWARE_FILE_SYSTEM_FILEDL "filedl"
/*   fpgadl (text) - data=string - FPGA bitstream download progress ('complete N bytes') */
#define OW_EVENT_HARDWARE_FILE_SYSTEM_FPGADL "fpgadl"
/* List Zones. Lists all 17 power zones with their name and rail, then the three control lines (18-20)..  Wire: h\p\l */
ow_status ow_hardware_power_management_list_zones(ow_device* dev);

/* Get Zones. Shows which power zones are currently on, then the reset state of the three control lines..  Wire: h\p\g */
ow_status ow_hardware_power_management_get_zones(ow_device* dev);

/* Set Zone. Switches one power zone on or off. Zone 9 is the board-manager LED, not a power rail; zones 18-20 are reset lines with their own commands..  Wire: h\p\s */
ow_status ow_hardware_power_management_set_zone(ow_device* dev, int32_t zone, int32_t on);

/* Set Zone Mask. Sets every user-controllable zone at once from a bit mask; bit 0 is zone 1..  Wire: h\p\m */
ow_status ow_hardware_power_management_set_zone_mask(ow_device* dev, int32_t mask);

/* Get Power State. Prints the most recent power telemetry sample..  Wire: h\p\t */
ow_status ow_hardware_power_management_get_power_state(ow_device* dev);

/* Stream Power. Streams battery, charger and power-zone telemetry to the host at the given rate. 0 stops the stream..  Wire: h\p\o */
ow_status ow_hardware_power_management_enable_power_stream(ow_device* dev, int32_t stream_rate_ms);

/* Set WIO Reset Line. Holds or releases the LoRa module's reset line (zone 18, WIO_RST). 1 lets the module run, 0 holds it in reset..  Wire: h\p\w */
ow_status ow_hardware_power_management_set_wio_reset_line(ow_device* dev, ow_reset_line_state state);

/* Set CM0 Run Line. Holds or releases the Linux CPU's run line (zone 19, CM0_RUNPG). 1 lets the module run, 0 holds it in reset..  Wire: h\p\c */
ow_status ow_hardware_power_management_set_cm0_run_line(ow_device* dev, ow_reset_line_state state);

/* Get Control Lines. Reads back the pin levels of the three control lines, WIO_RST, CM0_RUNPG and MAIN_PWR_RST..  Wire: h\p\n */
ow_status ow_hardware_power_management_get_control_lines(ow_device* dev, bool* wio_released, bool* cm0_released, bool* main_rst_high);


/* Events emitted by Power Management: */
/*   power (text) - soc=decS32, current_ma=decS32, remain_mah=decS32, full_mah=decS32, vbus_mv=decS32, vsys_mv=decS32, vbat_mv=decS32, ichg_ma=decS32, chg_stat=decS32, vbus_stat=decS32, fault=decS32, zone_mask=decU32, tier_main=decS32, tier_display=decS32, backlight=decS32, idle_ms=decS32, valid=bool - Power Telemetry */
#define OW_EVENT_HARDWARE_POWER_MANAGEMENT_POWER "power"
/* Enable Reader. Enable/disable NFC reader with auto tag streaming.  Wire: w\n\r */
ow_status ow_wireless_nfc_enable_reader(ow_device* dev, int32_t enable);

/* Print Card Info. Display detailed info about detected card.  Wire: w\n\c */
ow_status ow_wireless_nfc_print_card_info(ow_device* dev);

/* Get Status (debug). Display NFC hardware state and debug info.  Wire: w\n\g */
ow_status ow_wireless_nfc_get_status(ow_device* dev);


/* Events emitted by NFC Functions: */
/*   nfc (text) - data=string - NFC card status text (card detected / removed) */
#define OW_EVENT_WIRELESS_NFC_NFC "nfc"
/* List Saved Cards. List all .nfc files in the saved cards directory.  Wire: w\n\s\l */
ow_status ow_wireless_nfc_saved_cards_list_saved_cards(ow_device* dev);

/* Load Card. Load card data from .nfc file.  Wire: w\n\s\o */
ow_status ow_wireless_nfc_saved_cards_load_card(ow_device* dev, const char* path);

/* Save Current Card. Save currently detected card to .nfc file.  Wire: w\n\s\s */
ow_status ow_wireless_nfc_saved_cards_save_current_card(ow_device* dev, const char* path);

/* Read with Keys. Authenticate and read sectors using known keys.  Wire: w\n\m\r */
ow_status ow_wireless_nfc_mifare_classic_read_with_keys(ow_device* dev);

/* Dictionary Attack. Try keys from dictionary file to recover unknown keys.  Wire: w\n\m\a */
ow_status ow_wireless_nfc_mifare_classic_dictionary_attack(ow_device* dev, const char* path);

/* Dump Card. Read all sectors with known keys and display contents.  Wire: w\n\m\u */
ow_status ow_wireless_nfc_mifare_classic_dump_card(ow_device* dev);

/* Begin. Initialize the ST25R3916 and take ownership of the NFC front-end.  Wire: w\n\k\b */
ow_status ow_wireless_nfc_raw_begin(ow_device* dev);

/* End. Release the ST25R3916 back to the normal reader/writer state machine.  Wire: w\n\k\e */
ow_status ow_wireless_nfc_raw_end(ow_device* dev);

/* Field. Turn the RF field on or off.  Wire: w\n\k\f */
ow_status ow_wireless_nfc_raw_field(ow_device* dev, int32_t on);

/* Write Register. Write a single ST25R3916 register.  Wire: w\n\k\w */
ow_status ow_wireless_nfc_raw_reg_write(ow_device* dev, uint32_t addr, uint32_t value);

/* Read Register. Read a single ST25R3916 register.  Wire: w\n\k\r */
ow_status ow_wireless_nfc_raw_reg_read(ow_device* dev, uint32_t addr, uint32_t* value);

/* Send Command. Send a direct command to the ST25R3916.  Wire: w\n\k\c */
ow_status ow_wireless_nfc_raw_cmd(ow_device* dev, uint32_t command);

/* Transceive. Transmit bytes and receive the response over the RF field.  Wire: w\n\k\t */
ow_status ow_wireless_nfc_raw_transceive(ow_device* dev, uint32_t flags, int32_t timeout_ms, const uint8_t* tx, size_t tx_len, uint32_t* status, uint8_t* rx, size_t rx_cap, size_t* rx_len);

/* Halt Card. Send HLTA command to put card in HALT state.  Wire: w\n\x\a */
ow_status ow_wireless_nfc_extra_halt_card(ow_device* dev);

/* Connect To Bootloader. Instruct the ESP32 to enter into bootloader.  Wire: w\a\b */
ow_status ow_wireless_esp32_flasher_enter_bootloader(ow_device* dev, int32_t upgrade_transmission_rate);

/* Reset. Instruct the ESP32 to enter into application.  Wire: w\a\r */
ow_status ow_wireless_esp32_flasher_enter_application(ow_device* dev);

/* Read Chip ID And Security Info. Toggle ESP32's Enable Pin.  Wire: w\a\i */
ow_status ow_wireless_esp32_flasher_get_i_dand_security(ow_device* dev, int32_t* esp_chip_id, int32_t* version, bool* sb_en, bool* sbar_en, bool* sdm_en, bool* sbrk_1, bool* sbrk_2, bool* sbrk_3, bool* jtag_sw_dis, bool* jtag_hw_dis, bool* flash_enc_en, bool* dcache_dis, bool* icache_dis);

/* Read Flash Size. Toggle ESP32's Enable Pin.  Wire: w\a\k */
ow_status ow_wireless_esp32_flasher_read_flash_size(ow_device* dev, int32_t* flash_size_bytes);

/* Read MAC. Returns MAC of esp32.  Wire: w\a\m */
ow_status ow_wireless_esp32_flasher_read_esp32mac(ow_device* dev, char* esp32_mac, size_t esp32_mac_cap);

/* Erase All Flash. Toggle ESP32's Enable Pin.  Wire: w\a\e */
ow_status ow_wireless_esp32_flasher_erase_all_flash(ow_device* dev);

/* Start Writing Flash Operations. Prepares ESP32 to write flash at offset and expected size. Block size can be up to 128 bytes.  Wire: w\a\f */
ow_status ow_wireless_esp32_flasher_start_flash_operations(ow_device* dev, uint32_t offset, int32_t size, int32_t block_size);

/* Finish Flash Writing Operations. Ends ESP32 Flashing Operations..  Wire: w\a\p */
ow_status ow_wireless_esp32_flasher_stop_flash_operation(ow_device* dev, bool reboot);

/* Write Flash. Writes Binary Blob into flash.  Wire: w\a\o */
ow_status ow_wireless_esp32_flasher_flash_write(ow_device* dev, const uint8_t* flash_data, size_t flash_data_len);

/* Read Flash. Reads binary blob from flash with given address and size..  Wire: w\a\j */
ow_status ow_wireless_esp32_flasher_flash_read(ow_device* dev, uint32_t offset, int32_t size);

/* Start Memory Write Operations. Perpares memeory write operations on the esp32. Max Block Size size is 128.  Wire: w\a\y */
ow_status ow_wireless_esp32_flasher_start_write_memory_operations(ow_device* dev, uint32_t offset, uint32_t memory_block, int32_t block_size);

/* Write Memory. Perpares memeory write operations on the esp32. Max Block Size size is 128.  Wire: w\a\0 */
ow_status ow_wireless_esp32_flasher_memory_write(ow_device* dev, uint32_t offset, uint32_t memory_block, int32_t block_size);

/* Stop Memory Write Operations. Disables memory write operations on esp32 and sets entry point in ram.  Wire: w\a\t */
ow_status ow_wireless_esp32_flasher_stop_memory_operation(ow_device* dev, uint32_t entry_address);

/* Write Register. Writes a 4 byte value onto a register in the esp32.  Wire: w\a\g */
ow_status ow_wireless_esp32_flasher_register_write(ow_device* dev, uint32_t offset, uint32_t value);

/* Read Register. Reads a 4 byte value from a register in the esp32.  Wire: w\a\c */
ow_status ow_wireless_esp32_flasher_register_read(ow_device* dev, uint32_t offset, uint32_t* memory_block);

/* Flash Default App. Flash default application onto ESP32.  Wire: w\a\n */
ow_status ow_wireless_esp32_flasher_flash_default(ow_device* dev);

/* Enable Wifi Events. Toggle Wifi Event Streaming.  Wire: w\w\r */
ow_status ow_wireless_wifi_toggle_events(ow_device* dev);

/* Start Access Point. Starts up Access Point with provided SSID and Password.  Wire: w\w\a */
ow_status ow_wireless_wifi_on_start_access_point(ow_device* dev, const char* ssid, const char* password, int32_t authmode, bool hidessid);

/* Stop Access Point. Turns off Access Point.  Wire: w\w\t */
ow_status ow_wireless_wifi_on_discconect_from_station(ow_device* dev);

/* Get Stations connected to AP. Turns off Access Point.  Wire: w\w\g */
ow_status ow_wireless_wifi_get_connected_devices(ow_device* dev);

/* Connect to a Wifi Access Point. Connect to a WAP with provided SSID and Password.  Wire: w\w\c */
ow_status ow_wireless_wifi_on_connect_to_station(ow_device* dev, const char* ssid, const char* password);

/* Disconnect From Wifi Access Point. Disconnect from Wifi Stations.  Wire: w\w\f */
ow_status ow_wireless_wifi_on_discconect_from_station_2(ow_device* dev);

/* Scan for Access Points. Scans for available WIFI networks.  Wire: w\w\s */
ow_status ow_wireless_wifi_on_scan_for_access_points(ow_device* dev);

/* Print out Wifi Info. Scans for available Wifi networks.  Wire: w\w\p */
ow_status ow_wireless_wifi_on_get_wif_info(ow_device* dev);


/* Events emitted by Wifi Functions: */
/*   wifistaInfo (text) - ip=string, gateway=string, mask=string - Station IP configuration (got IP) */
#define OW_EVENT_WIRELESS_WIFI_WIFISTA_INFO "wifistaInfo"
/*   wifiapInfo (text) - ip=string, gateway=string, mask=string - Access point IP configuration */
#define OW_EVENT_WIRELESS_WIFI_WIFIAP_INFO "wifiapInfo"
/*   wifiscan (text) - bssid=string, rssi=decS32, channel=decU32, band=decU32, authmode=decU32, ssid=string - Wifi scan record (SSID last so consumers can bounded-split) */
#define OW_EVENT_WIRELESS_WIFI_WIFISCAN "wifiscan"
/*   wifiapdevcon (text) - ip=string, mac=string - Device connected to the access point */
#define OW_EVENT_WIRELESS_WIFI_WIFIAPDEVCON "wifiapdevcon"
/*   wifiapdevdc (text) - mac=string - Device disconnected from the access point */
#define OW_EVENT_WIRELESS_WIFI_WIFIAPDEVDC "wifiapdevdc"
/*   wsclientcon (text) - ip=string - Websocket client connected */
#define OW_EVENT_WIRELESS_WIFI_WSCLIENTCON "wsclientcon"
/*   wsclientdc (text) - ip=string - Websocket client disconnected */
#define OW_EVENT_WIRELESS_WIFI_WSCLIENTDC "wsclientdc"
/*   wifistations (text) - ip=string, mac=string - Connected access-point station record (one event per device) */
#define OW_EVENT_WIRELESS_WIFI_WIFISTATIONS "wifistations"
/* Enable Station Mode. Connect the device to an existing Wi-Fi network in station mode, or disconnect from it.  Wire: w\w\e\s */
ow_status ow_wireless_wifi_settings_enable_station_mode(ow_device* dev);

/* SSID for Station Mode. Set the name (SSID) of the Wi-Fi network to join in station mode.  Wire: w\w\e\e */
ow_status ow_wireless_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value);

/* Password for Station Mode. Set the password used to join the Wi-Fi network in station mode.  Wire: w\w\e\p */
ow_status ow_wireless_wifi_settings_password_for_station_mode(ow_device* dev, const char* value);

/* Enable AP Mode. Turn the device's own Wi-Fi access point on or off.  Wire: w\w\e\a */
ow_status ow_wireless_wifi_settings_enable_ap_mode(ow_device* dev);

/* AP Auth. Choose the Wi-Fi security type used by the device's own access point.  Wire: w\w\e\u */
ow_status ow_wireless_wifi_settings_a_p_auth(ow_device* dev, int32_t value);

/* AP hide SSID. Hide the access point's network name (SSID) so it isn't broadcast to nearby devices.  Wire: w\w\e\i */
ow_status ow_wireless_wifi_settings_a_p_hide_ssid(ow_device* dev);

/* SSID for AP. Set the network name (SSID) broadcast by the device's own access point.  Wire: w\w\e\g */
ow_status ow_wireless_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value);

/* Password for AP. Set the password required to join the device's own access point.  Wire: w\w\e\x */
ow_status ow_wireless_wifi_settings_password_for_ap(ow_device* dev, const char* value);

/* Start BT Advertising. Sets the Host Name for the Bluetooth LE.  Wire: w\b\a */
ow_status ow_wireless_bluetooth_le_on_start_bt_advertising(ow_device* dev, const char* hostname);

/* Stop BT Advertising. Stops BT Advertising.  Wire: w\b\t */
ow_status ow_wireless_bluetooth_le_on_stop_bt_advertising(ow_device* dev);

/* Scan for BT Devices. Scans for BT devices for a given duration.  Wire: w\b\s */
ow_status ow_wireless_bluetooth_le_on_scan_bt_devices(ow_device* dev, int32_t durationms);

/* Toggle Enable Terminal API Mode. Enables BLE to FreeWili Terminal API Mode.  Wire: w\b\e */
ow_status ow_wireless_bluetooth_le_on_enable_terminal(ow_device* dev);


/* Events emitted by BT Functions: */
/*   btscan (text) - name=string, mac=string, rssi=decS32 - BLE scan result (device name, MAC address, RSSI) */
#define OW_EVENT_WIRELESS_BLUETOOTH_LE_BTSCAN "btscan"
/* Enable BT. Turn Bluetooth LE on or off.  Wire: w\b\b\s */
ow_status ow_wireless_bluetooth_le_ble_settings_enable_bt(ow_device* dev);

/* BT <-> Terminal. Shown in Bluetooth LE status, but not currently used: the firmware always follows the Enable BT setting instead.  Wire: w\b\b\t */
ow_status ow_wireless_bluetooth_le_ble_settings_b_t_terminal(ow_device* dev);

/* BT Advert Name. Set the name the device advertises over Bluetooth LE.  Wire: w\b\b\a */
ow_status ow_wireless_bluetooth_le_ble_settings_b_t_advert_name(ow_device* dev, const char* value);

/* Stream IR. Enables or disables streaming of received IR codes to the host..  Wire: w\i\o */
ow_status ow_wireless_ir_enable_ir_stream(ow_device* dev, int32_t enable);

/* Send IR. Transmits a 4-byte IR code..  Wire: w\i\a */
ow_status ow_wireless_ir_send_ir_data(ow_device* dev, int32_t ir_code);


/* Events emitted by IR Functions: */
/*   irrx (text) - code=hexU32 - Received IR code */
#define OW_EVENT_WIRELESS_IR_IRRX "irrx"
/* Configure. LoRa modem params:.  Wire: w\l\c */
ow_status ow_wireless_lo_ra_configure(ow_device* dev, int32_t freq_hz, int32_t sf, int32_t bw_enc, int32_t cr, int32_t power, int32_t preamble, uint8_t sync);

/* Send. Transmits a LoRa packet.  Wire: w\l\s */
ow_status ow_wireless_lo_ra_send_payload(ow_device* dev, const uint8_t* data, size_t data_len);

/* RX Enable. RX control:.  Wire: w\l\r */
ow_status ow_wireless_lo_ra_rx_enable(ow_device* dev, int32_t mode);

/* Status. WIO-E5 bridge status (a 'lora' STATUS event):.  Wire: w\l\t */
ow_status ow_wireless_lo_ra_status(ow_device* dev);

/* Raw Frame. Sends a raw framed command to the bridge (advanced).  Wire: w\l\f */
ow_status ow_wireless_lo_ra_raw_frame(ow_device* dev, uint8_t cmd_, const uint8_t* payload, size_t payload_len);


/* Events emitted by LoRa: */
/*   lora (text) - data=string - LoRa RX / status / event line (free-form text) */
#define OW_EVENT_WIRELESS_LO_RA_LORA "lora"
/* Select Circuit. Claims the sub-GHz front end for this client and holds it until Release..  Wire: w\r\s */
ow_status ow_wireless_radio_select_circuit(ow_device* dev, int32_t band);

/* Release Circuit. Ends this client's circuit hold and hands the antenna back to LoRa..  Wire: w\r\e */
ow_status ow_wireless_radio_release_circuit(ow_device* dev);

/* State. Reads the mux and radio state back, in this order:.  Wire: w\r\t */
ow_status ow_wireless_radio_read_state(ow_device* dev, int32_t* owner, int32_t* holder, int32_t* band, int32_t* want_v1, int32_t* want_v2, int32_t* have_valid, int32_t* have_v1, int32_t* have_v2, int32_t* lora_paused, int32_t* freq_hz, int32_t* active, int32_t* status, uint8_t* version);

/* Band. Forces the matched antenna path now: 1 low, 2 mid, 3 high..  Wire: w\r\b */
ow_status ow_wireless_radio_select_band(ow_device* dev, int32_t band);

/* Comm Check. Reads the CC1101 version register and returns it..  Wire: w\r\c */
ow_status ow_wireless_radio_comm_check(ow_device* dev, uint8_t* version);

/* Frequency. Tunes the CC1101 and selects the matched antenna path for that band..  Wire: w\r\f */
ow_status ow_wireless_radio_set_frequency(ow_device* dev, int32_t freq_hz, int32_t* band);

/* RSSI. Samples received signal strength once, in dBm..  Wire: w\r\i */
ow_status ow_wireless_radio_read_rssi(ow_device* dev, int32_t* rssi);

/* Carrier. Keys or unkeys an unmodulated carrier at the current frequency..  Wire: w\r\o */
ow_status ow_wireless_radio_carrier(ow_device* dev, int32_t on);

/* RX Enable. Puts the CC1101 into continuous receive, or back to idle..  Wire: w\r\r */
ow_status ow_wireless_radio_rx_enable(ow_device* dev, int32_t on);

/* Idle. Returns the CC1101 to idle from receive, transmit or carrier..  Wire: w\r\w */
ow_status ow_wireless_radio_idle(ow_device* dev);

/* Packet Send. Transmits one GFSK packet through the CC1101 packet engine..  Wire: w\r\x */
ow_status ow_wireless_radio_packet_send(ow_device* dev, int32_t freq_hz, const uint8_t* data, size_t data_len);

/* Packet RX. Opens or closes the GFSK packet receiver at the given frequency..  Wire: w\r\y */
ow_status ow_wireless_radio_packet_rx(ow_device* dev, int32_t on, int32_t freq_hz);

/* Packet Read. Reads the last received GFSK packet: RSSI in dBm, a sequence counter that.  Wire: w\r\k */
ow_status ow_wireless_radio_packet_read(ow_device* dev, int32_t* rssi, int32_t* seq, uint8_t* data, size_t data_cap, size_t* data_len);

/* Capture Start. Arms a raw pulse-duration capture at the given frequency..  Wire: w\r\g */
ow_status ow_wireless_radio_capture_start(ow_device* dev, int32_t freq_hz, int32_t preset);

/* Capture Stop. Ends a capture and returns how many pulse durations it recorded..  Wire: w\r\j */
ow_status ow_wireless_radio_capture_stop(ow_device* dev, int32_t* durations);

/* Replay. Re-transmits the last capture out the transmit path..  Wire: w\r\p */
ow_status ow_wireless_radio_replay(ow_device* dev);

/* Analyzer. Starts or stops the background frequency-analyzer sweep..  Wire: w\r\a */
ow_status ow_wireless_radio_analyzer(ow_device* dev, int32_t on);

/* Spectrum. Reads the analyzer's results: the peak frequency in Hz, its level in dBm,.  Wire: w\r\n */
ow_status ow_wireless_radio_spectrum(ow_device* dev, int32_t* peak_freq_hz, int32_t* peak_rssi, uint8_t* bins, size_t bins_cap, size_t* bins_len);

/* Squelch. Sets the level a capture must see before it starts recording, and below.  Wire: w\r\u */
ow_status ow_wireless_radio_squelch(ow_device* dev, int32_t dbm);

/* Replay Invert. Flips the captured low/high phase before re-keying it on Replay..  Wire: w\r\v */
ow_status ow_wireless_radio_replay_invert(ow_device* dev, int32_t on);

/* Transmit Sub. Transmits a Flipper .sub file from the card..  Wire: w\r\m */
ow_status ow_wireless_radio_transmit_sub_file(ow_device* dev, const char* path);

/* Monitor. Keeps the receiver open and samples signal strength continuously, so.  Wire: w\r\l */
ow_status ow_wireless_radio_monitor(ow_device* dev, int32_t on);


/* Events emitted by Radio: */
/*   radio1 (text) - data_bytes=hexbytes - Radio 1 received data (hex bytes) */
#define OW_EVENT_WIRELESS_RADIO_RADIO1 "radio1"
/*   radio2 (text) - data_bytes=hexbytes - Radio 2 received data (hex bytes) */
#define OW_EVENT_WIRELESS_RADIO_RADIO2 "radio2"
/*   radioasync (text) - data=string - Async sub-file transmit/capture status (free-form text) */
#define OW_EVENT_WIRELESS_RADIO_RADIOASYNC "radioasync"
/* Launch Script. Not yet implemented; always reports failure.  Wire: s\a */
ow_status ow_scripting_launch_script(ow_device* dev);


/* Events emitted by Scripting Functions: */
/*   script (text) - data=string - Script engine output / status line (wasm and rThon runners) */
#define OW_EVENT_SCRIPTING_SCRIPT "script"
/* Add. Adds an app signal..  Wire: s\i\a */
ow_status ow_scripting_app_signals_app_signal_add(ow_device* dev, const char* name);

/* Remove. Removes an app signal..  Wire: s\i\x */
ow_status ow_scripting_app_signals_app_signal_remove(ow_device* dev, const char* name);

/* Rename. Renames an app signal..  Wire: s\i\r */
ow_status ow_scripting_app_signals_app_signal_rename(ow_device* dev, const char* name, const char* new_name);

/* Set Value. Sets an app signal value..  Wire: s\i\s */
ow_status ow_scripting_app_signals_app_signal_set(ow_device* dev, const char* name, double value);

/* Get Value. Gets an app signal value..  Wire: s\i\g */
ow_status ow_scripting_app_signals_app_signal_get(ow_device* dev, const char* name, char* name_out, size_t name_out_cap, double* value);

/* Apply Wave. Applies wave mode 0-8 (0 off; sine, triangle, square and saw at 0.5/2 Hz)..  Wire: s\i\w */
ow_status ow_scripting_app_signals_app_signal_wave(ow_device* dev, const char* name, int32_t wave);

/* Stream. Streams every defined app signal; 0 disables streaming..  Wire: s\i\t */
ow_status ow_scripting_app_signals_app_signal_stream(ow_device* dev, int32_t stream_rate_ms);


/* Events emitted by App Signals: */
/*   appSignal (text) - name=string, value=float - Streamed app-signal value. */
#define OW_EVENT_SCRIPTING_APP_SIGNALS_APP_SIGNAL "appSignal"
/* Load. Loads a .wili project..  Wire: s\f\l */
ow_status ow_scripting_wili_files_wili_load(ow_device* dev, const char* filepath);

/* Save Current. Saves the current Wili project to its source path..  Wire: s\f\s */
ow_status ow_scripting_wili_files_wili_save(ow_device* dev);

/* Reset. Clears the live panels, Wili Blocks, and app signals..  Wire: s\f\r */
ow_status ow_scripting_wili_files_wili_reset(ow_device* dev);

/* Make Default. Sets the Wili project loaded at boot..  Wire: s\f\m */
ow_status ow_scripting_wili_files_wili_default(ow_device* dev, const char* filepath);

/* Remove Default. Removes the configured boot Wili project..  Wire: s\f\x */
ow_status ow_scripting_wili_files_wili_remove_default(ow_device* dev);

/* Stream ZoomIO Data. Enables or disables streaming of ZoomIO receive data to the host..  Wire: s\b\o */
ow_status ow_scripting_zoom_io_enable_rx_stream(ow_device* dev, int32_t enable);

/* Write to FIFO. Sends a single ZoomIO message after the given delay (us)..  Wire: s\b\w */
ow_status ow_scripting_zoom_io_send_data(ow_device* dev, int32_t delay, const uint8_t* data, size_t data_len);

/* Update Schedule Table. Updates a schedule-table transmit message..  Wire: s\b\u */
ow_status ow_scripting_zoom_io_update_table_data(ow_device* dev, int32_t table_index, int32_t delay, const uint8_t* data, size_t data_len);

/* Setup Schedule Table. Sets up the schedule table size (0 to disable)..  Wire: s\b\p */
ow_status ow_scripting_zoom_io_enable_schedule_table(ow_device* dev, int32_t number_of_entries);

/* Compile test. Compiles built-in ZoomIO milestone program and launches it on core1 as RISC-V.  Wire: s\b\c */
ow_status ow_scripting_zoom_io_compile_test(ow_device* dev);

/* Run ZoomIO. Compile and run a ZoomIO program on the RISC-V core1.  Wire: s\b\r */
ow_status ow_scripting_zoom_io_run_zio(ow_device* dev, const char* path);

/* Stop ZoomIO. Reset core1 to stop the running program.  Wire: s\b\s */
ow_status ow_scripting_zoom_io_stop_zio(ow_device* dev);


/* Events emitted by ZoomIO Functions: */
/*   zoomio (text) - data_bytes=hexbytes - ZoomIO received packet (hex bytes) */
#define OW_EVENT_SCRIPTING_ZOOM_IO_ZOOMIO "zoomio"
/* WASM Debug Start. Loads a .wilwasm for debugging..  Wire: s\w\c */
ow_status ow_scripting_wasm_debug_debug_start(ow_device* dev, const char* path);

/* WASM Debug Breakpoints. Replaces the byte-PC breakpoint set..  Wire: s\w\j */
ow_status ow_scripting_wasm_debug_debug_breakpoints(ow_device* dev, const char* pcs);

/* WASM Debug Step. Steps one opcode, or until the PC leaves [lo,hi)..  Wire: s\w\e */
ow_status ow_scripting_wasm_debug_debug_step(ow_device* dev, const char* range);

/* WASM Debug Continue. Resumes until the next breakpoint..  Wire: s\w\f */
ow_status ow_scripting_wasm_debug_debug_continue(ow_device* dev);

/* WASM Debug Pause. Pauses at the next opcode..  Wire: s\w\g */
ow_status ow_scripting_wasm_debug_debug_pause(ow_device* dev);

/* WASM Debug Stop. Stops the active wasm debug session..  Wire: s\w\t */
ow_status ow_scripting_wasm_debug_debug_stop(ow_device* dev);

/* WASM Debug Locals. Dumps stack frames and raw frame-0 locals..  Wire: s\w\i */
ow_status ow_scripting_wasm_debug_debug_locals(ow_device* dev);

/* WASM Debug Memory Read. Reads up to 64 bytes of wasm linear memory (hex)..  Wire: s\w\r */
ow_status ow_scripting_wasm_debug_debug_mem_read(ow_device* dev, const char* addr);

/* Debug Start. Loads and compiles a script for debugging..  Wire: s\r\c */
ow_status ow_scripting_rthon_debug_debug_start(ow_device* dev, const char* path);

/* Debug Breakpoints. Replaces the breakpoint set for the active debug session..  Wire: s\r\j */
ow_status ow_scripting_rthon_debug_debug_breakpoints(ow_device* dev, const char* lines);

/* Debug Step. Single-steps the active debug session..  Wire: s\r\e */
ow_status ow_scripting_rthon_debug_debug_step(ow_device* dev);

/* Debug Continue. Resumes the active debug session until the next breakpoint..  Wire: s\r\f */
ow_status ow_scripting_rthon_debug_debug_continue(ow_device* dev);

/* Debug Pause. Pauses the active debug session at the next statement..  Wire: s\r\g */
ow_status ow_scripting_rthon_debug_debug_pause(ow_device* dev);

/* Debug Stop. Stops the active debug session..  Wire: s\r\t */
ow_status ow_scripting_rthon_debug_debug_stop(ow_device* dev);

/* Debug Locals. Dumps the local variables of the active debug session..  Wire: s\r\i */
ow_status ow_scripting_rthon_debug_debug_locals(ow_device* dev);

/* Launch App. Switch the built-in display to the app with the given app ID.  Wire: a\a */
ow_status ow_apps_launch_app(ow_device* dev, int32_t app_id);

/* Enable Linux CPU. Not yet implemented; always reports failure.  Wire: l\a */
ow_status ow_linux_enable_linux_cpu(ow_device* dev);

/* Open Shell.  Wire: l\b */
ow_status ow_linux_open_shell(ow_device* dev);


#ifdef __cplusplus
}
#endif
#endif /* ONEWILI_H */
