/* OneWili WASM thin bindings - generated. Do not edit.
 * Each call packs its args and dispatches through the single ow_call
 * host import; the firmware assembles the wire command natively. */
#include "onewili.h"
#include "onewili_wasm.h"

ow_status ow_io_gpio_set_io_high(ow_device* dev, int32_t pin)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, pin);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_SET_IO_HIGH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_set_io_low(ow_device* dev, int32_t pin)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, pin);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_SET_IO_LOW, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_set_io_toggle(ow_device* dev, int32_t pin)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, pin);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_SET_IO_TOGGLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_set_pwm(ow_device* dev, int32_t gpio_number, double freq, double duty)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, gpio_number);
    ao = ow__pack_f32(args, ao, (float)freq);
    ao = ow__pack_f32(args, ao, (float)duty);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_SET_PWM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_read_all(ow_device* dev, uint32_t* gpiostate)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_READ_ALL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (gpiostate) *gpiostate = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_gpio_stream_io(ow_device* dev, int32_t reportratems)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, reportratems);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_STREAM_IO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_toggle_hsbdio(ow_device* dev, int32_t pin)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, pin);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_TOGGLE_HSBDIO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1_rx12(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1_RX12, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_g_pio2626(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_G_PIO2626, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1cs13(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1CS13, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_g_pio27(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_G_PIO27, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1_rx9(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1_RX9, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1cts10(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1CTS10, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1_tx8(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1_TX8, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_u_art1rts11(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_U_ART1RTS11, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1_tx15(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1_TX15, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_s_pi1sclk14(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_S_PI1SCLK14, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_gpio_io_direction_settings_g_pio2525(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_IO_DIRECTION_SETTINGS_G_PIO2525, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_u_art_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_U_ART_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_toggle_stream(ow_device* dev, uint8_t* data_bytes, size_t data_bytes_cap, size_t* data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_TOGGLE_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_bytes(ret, &ro, data_bytes, (int)data_bytes_cap, (int*)data_bytes_len);
    return OW_OK;
}

ow_status ow_io_uart_uart_enable_api_mode(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_UART_ENABLE_API_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_baud_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_BAUD_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_r_ts_hand_shaking(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_R_TS_HAND_SHAKING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_c_ts_hand_shaking(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_C_TS_HAND_SHAKING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_data_bits(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_DATA_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_parity(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_PARITY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_stop_bits(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_STOP_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_uart_settings_module(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_UART_SETTINGS_MODULE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_poll_sfp(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_POLL_SFP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, uint32_t* sfp_response)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, device_address);
    ao = ow__pack_bytes(args, ao, register_address, (uint32_t)register_address_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_READ_SFP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (sfp_response) *sfp_response = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write_sfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, device_address);
    ao = ow__pack_bytes(args, ao, register_address, (uint32_t)register_address_len);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_WRITE_SFP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormwsfp(ow_device* dev, uint8_t device_address, const uint8_t* register_address, size_t register_address_len, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, device_address);
    ao = ow__pack_bytes(args, ao, register_address, (uint32_t)register_address_len);
    ao = ow__pack_bytes(args, ao, mask_bytes, (uint32_t)mask_bytes_len);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIORMWSFP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_poll(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_POLL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read22(ow_device* dev, uint8_t phy_address, uint8_t register_address, uint32_t* mdio_response)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, register_address);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_READ22, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (mdio_response) *mdio_response = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, register_address);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_WRITE22, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormw22(ow_device* dev, uint8_t phy_address, uint8_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, register_address);
    ao = ow__pack_bytes(args, ao, mask_bytes, (uint32_t)mask_bytes_len);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIORMW22, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, mmd_address);
    ao = ow__pack_u32(args, ao, register_address);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_READ45, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (mdio_response) *mdio_response = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, mmd_address);
    ao = ow__pack_u32(args, ao, register_address);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_WRITE45, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormw45(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, mmd_address);
    ao = ow__pack_u32(args, ao, register_address);
    ao = ow__pack_bytes(args, ao, mask_bytes, (uint32_t)mask_bytes_len);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIORMW45, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdio_read_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, uint32_t* mdio_response)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, mmd_address);
    ao = ow__pack_u32(args, ao, register_address);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_READ_EMU, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (mdio_response) *mdio_response = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_mdio_mdio_write_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, mmd_address);
    ao = ow__pack_u32(args, ao, register_address);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIO_WRITE_EMU, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_mdio_mdiormw_emu(ow_device* dev, uint8_t phy_address, uint8_t mmd_address, uint32_t register_address, const uint8_t* mask_bytes, size_t mask_bytes_len, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, phy_address);
    ao = ow__pack_u8(args, ao, mmd_address);
    ao = ow__pack_u32(args, ao, register_address);
    ao = ow__pack_bytes(args, ao, mask_bytes, (uint32_t)mask_bytes_len);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_MDIO_MDIORMW_EMU, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_sensors_enable_motion_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SENSORS_ENABLE_MOTION_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_sensors_enable_field_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SENSORS_ENABLE_FIELD_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_sensors_enable_env_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SENSORS_ENABLE_ENV_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_sensors_enable_orientation_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SENSORS_ENABLE_ORIENTATION_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_sensors_get_sensors(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SENSORS_GET_SENSORS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_write(ow_device* dev, uint8_t address, uint8_t register_, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, address);
    ao = ow__pack_u8(args, ao, register_);
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_I2C_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_read(ow_device* dev, uint8_t* i2crepsone, size_t i2crepsone_cap, size_t* i2crepsone_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_I2C_READ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_bytes(ret, &ro, i2crepsone, (int)i2crepsone_cap, (int*)i2crepsone_len);
    return OW_OK;
}

ow_status ow_io_i2c_i2c_poll(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_I2C_POLL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_i2c_settings_frequency(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_SETTINGS_FREQUENCY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_i2c_settings_pull_ups(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_SETTINGS_PULL_UPS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_spi_s_pi_write(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len, uint8_t* spi_response, size_t spi_response_cap, size_t* spi_response_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_S_PI_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_bytes(ret, &ro, spi_response, (int)spi_response_cap, (int*)spi_response_len);
    return OW_OK;
}

ow_status ow_io_spi_settings_frequency(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_SETTINGS_FREQUENCY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_spi_settings_chip_select_pin(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_SETTINGS_CHIP_SELECT_PIN, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_spi_settings_data_bits(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_SETTINGS_DATA_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_spi_settings_c_pol(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_SETTINGS_C_POL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_spi_settings_c_pha(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_SETTINGS_C_PHA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_canfd_enable_canfd_stream(ow_device* dev, int32_t channel, int32_t enabled)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_i32(args, ao, enabled);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_CANFD_ENABLE_CANFD_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_canfd_write_canfd(ow_device* dev, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_u32(args, ao, arb_id);
    ao = ow__pack_i32(args, ao, can_fd);
    ao = ow__pack_i32(args, ao, xtd_id);
    ao = ow__pack_bytes(args, ao, data_in, (uint32_t)data_in_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_CANFD_WRITE_CANFD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_canfd_write_canfd_periodic(ow_device* dev, int32_t index, int32_t enable, int32_t period, int32_t channel, uint32_t arb_id, int32_t can_fd, int32_t xtd_id, const uint8_t* data_in, size_t data_in_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, enable);
    ao = ow__pack_i32(args, ao, period);
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_u32(args, ao, arb_id);
    ao = ow__pack_i32(args, ao, can_fd);
    ao = ow__pack_i32(args, ao, xtd_id);
    ao = ow__pack_bytes(args, ao, data_in, (uint32_t)data_in_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_CANFD_WRITE_CANFD_PERIODIC, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_canfd_setup_filter(ow_device* dev, int32_t channel, int32_t index, int32_t enable, int32_t xtd_id, uint32_t mask, uint32_t accept, uint32_t maskb0, uint32_t accept_b0, uint32_t maskb1, uint32_t accept_b1)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, enable);
    ao = ow__pack_i32(args, ao, xtd_id);
    ao = ow__pack_u32(args, ao, mask);
    ao = ow__pack_u32(args, ao, accept);
    ao = ow__pack_u32(args, ao, maskb0);
    ao = ow__pack_u32(args, ao, accept_b0);
    ao = ow__pack_u32(args, ao, maskb1);
    ao = ow__pack_u32(args, ao, accept_b1);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_CANFD_SETUP_FILTER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_canfd_read_can_registers(ow_device* dev, int32_t channel, uint32_t start_address, int32_t word_count, char* registers, size_t registers_cap)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_u32(args, ao, start_address);
    ao = ow__pack_i32(args, ao, word_count);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_CANFD_READ_CAN_REGISTERS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_str(ret, &ro, registers, (int)registers_cap);
    return OW_OK;
}

ow_status ow_io_canfd_set_can_register(ow_device* dev, int32_t channel, uint32_t start_address, int32_t byte_count, uint32_t word_to_write)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_u32(args, ao, start_address);
    ao = ow__pack_i32(args, ao, byte_count);
    ao = ow__pack_u32(args, ao, word_to_write);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_CANFD_SET_CAN_REGISTER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_in_enable_analog_in_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_IN_ENABLE_ANALOG_IN_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_in_read_analog_in2024(ow_device* dev, double* v0, double* v1, double* v2, double* v3)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_IN_READ_ANALOG_IN2024, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (v0) *v0 = ow__take_f32(ret, &ro); else ow__take_f32(ret, &ro);
    if (v1) *v1 = ow__take_f32(ret, &ro); else ow__take_f32(ret, &ro);
    if (v2) *v2 = ow__take_f32(ret, &ro); else ow__take_f32(ret, &ro);
    if (v3) *v3 = ow__take_f32(ret, &ro); else ow__take_f32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_analog_in_config_analog_in2024(ow_device* dev, int32_t channel, int32_t mux, int32_t range)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_i32(args, ao, mux);
    ao = ow__pack_i32(args, ao, range);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_IN_CONFIG_ANALOG_IN2024, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_in_set_data_rate2024(ow_device* dev, int32_t rate)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, rate);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_IN_SET_DATA_RATE2024, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_in_enable_analog_in2024_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_IN_ENABLE_ANALOG_IN2024_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_analog_output(ow_device* dev, int32_t channel, double value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_f32(args, ao, (float)value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_ANALOG_OUTPUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_trigger_window(ow_device* dev, double value_low, double value_high)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_f32(args, ao, (float)value_low);
    ao = ow__pack_f32(args, ao, (float)value_high);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_TRIGGER_WINDOW, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_enable_trigger(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_ENABLE_TRIGGER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_v_prog_vout(ow_device* dev, int32_t enable, double set_voltage)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    ao = ow__pack_f32(args, ao, (float)set_voltage);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_V_PROG_VOUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_glitch(ow_device* dev, int32_t nano_seconds)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, nano_seconds);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_GLITCH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_waveform(ow_device* dev, int32_t channel, ow_dac_wave_shape_menu waveform, double frequency_hz, double low_voltage, double high_voltage, ow_dac_wave_phase phase)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, channel);
    ao = ow__pack_i32(args, ao, (int32_t)waveform);
    ao = ow__pack_f32(args, ao, (float)frequency_hz);
    ao = ow__pack_f32(args, ao, (float)low_voltage);
    ao = ow__pack_f32(args, ao, (float)high_voltage);
    ao = ow__pack_i32(args, ao, (int32_t)phase);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_WAVEFORM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_analog_out_set_waveform_run(ow_device* dev, int32_t mask)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, mask);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_ANALOG_OUT_SET_WAVEFORM_RUN, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_player_setup_player(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t start_mode, int32_t trigger_pin, bool loop)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, sample_rate_ns);
    ao = ow__pack_i32(args, ao, sample_count);
    ao = ow__pack_i32(args, ao, pin_start);
    ao = ow__pack_i32(args, ao, pin_stop);
    ao = ow__pack_i32(args, ao, start_mode);
    ao = ow__pack_i32(args, ao, trigger_pin);
    ao = ow__pack_u8(args, ao, loop ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_PLAYER_SETUP_PLAYER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_player_setup_analog(ow_device* dev, int32_t mask, int32_t analog_rate_ns, int32_t analog_resolution)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, mask);
    ao = ow__pack_i32(args, ao, analog_rate_ns);
    ao = ow__pack_i32(args, ao, analog_resolution);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_PLAYER_SETUP_ANALOG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_player_load_file(ow_device* dev, const char* file_path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, file_path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_PLAYER_LOAD_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_player_start(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_PLAYER_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_player_stop(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_PLAYER_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_setup_logic_analyzer(ow_device* dev, int32_t sample_rate_ns, int32_t sample_count, int32_t pin_start, int32_t pin_stop, int32_t trigger_pin, int32_t trigger_type, int32_t rearm)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, sample_rate_ns);
    ao = ow__pack_i32(args, ao, sample_count);
    ao = ow__pack_i32(args, ao, pin_start);
    ao = ow__pack_i32(args, ao, pin_stop);
    ao = ow__pack_i32(args, ao, trigger_pin);
    ao = ow__pack_i32(args, ao, trigger_type);
    ao = ow__pack_i32(args, ao, rearm);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_ANALYZER_SETUP_LOGIC_ANALYZER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_setup_analog(ow_device* dev, int32_t analog_mask, int32_t analog_rate_ns, int32_t analog_res)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, analog_mask);
    ao = ow__pack_i32(args, ao, analog_rate_ns);
    ao = ow__pack_i32(args, ao, analog_res);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_ANALYZER_SETUP_ANALOG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_start(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_ANALYZER_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_stop(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_ANALYZER_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_logic_analyzer_trigger(ow_device* dev, int32_t trigger_type)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, trigger_type);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_LOGIC_ANALYZER_TRIGGER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_take_picture(ow_device* dev, int32_t destination, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, destination);
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_TAKE_PICTURE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_start_recording_video(ow_device* dev, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_START_RECORDING_VIDEO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_stop_recording_video(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_STOP_RECORDING_VIDEO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_toggle_ai_detection_stream(ow_device* dev, int32_t ai_stream_mode)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, ai_stream_mode);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_TOGGLE_AI_DETECTION_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_zoom_level(ow_device* dev, int32_t zoom)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, zoom);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_ZOOM_LEVEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_contrast(ow_device* dev, int32_t contrast)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, contrast);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_CONTRAST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_saturation(ow_device* dev, int32_t saturation)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, saturation);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_SATURATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_brightness(ow_device* dev, int32_t brightness)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, brightness);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_BRIGHTNESS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_hue(ow_device* dev, int32_t hue)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, hue);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_HUE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_resolution(ow_device* dev, int32_t resolutionstate)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, resolutionstate);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_RESOLUTION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_wil_eye_set_flash_state(ow_device* dev, bool flash)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, flash ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_WIL_EYE_SET_FLASH_STATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_play_audio_file(ow_device* dev, const char* file_path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, file_path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_PLAY_AUDIO_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_record_audio_file(ow_device* dev, const char* file_name)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, file_name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_RECORD_AUDIO_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_play_audio_asset(ow_device* dev, const char* asset_name)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, asset_name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_PLAY_AUDIO_ASSET, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_enable_audio_stream(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_ENABLE_AUDIO_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_numbers_to_speech(ow_device* dev, double number)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_f32(args, ao, (float)number);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_NUMBERS_TO_SPEECH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_tone(ow_device* dev, double frequency, double duration_ms, double amplitude)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_f32(args, ao, (float)frequency);
    ao = ow__pack_f32(args, ao, (float)duration_ms);
    ao = ow__pack_f32(args, ao, (float)amplitude);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_TONE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_audio_speak(ow_device* dev, const char* text)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, text);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_AUDIO_SPEAK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_set_led_color(ow_device* dev, int32_t ledindex, int32_t red, int32_t green, int32_t blue, int32_t duration, ow_ow_led_manager_led_mode mode)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, ledindex);
    ao = ow__pack_i32(args, ao, red);
    ao = ow__pack_i32(args, ao, green);
    ao = ow__pack_i32(args, ao, blue);
    ao = ow__pack_i32(args, ao, duration);
    ao = ow__pack_i32(args, ao, (int32_t)mode);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_SET_LED_COLOR, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_show_fwi_image(ow_device* dev, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_SHOW_FWI_IMAGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_clear_display(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CLEAR_DISPLAY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_show_text(ow_device* dev, const char* texttodisplay)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, texttodisplay);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_SHOW_TEXT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_read_all(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_READ_ALL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_stream_io(ow_device* dev, int32_t pin)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, pin);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_STREAM_IO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_show_image_asset_by_id(ow_device* dev, int32_t image_id)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, image_id);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_SHOW_IMAGE_ASSET_BY_ID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_panels_add_panel(ow_device* dev, bool use_tile, int32_t tile_id, const char* color, bool show_menu)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, use_tile ? 1 : 0);
    ao = ow__pack_i32(args, ao, tile_id);
    ao = ow__pack_str(args, ao, color);
    ao = ow__pack_u8(args, ao, show_menu ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_PANELS_ADD_PANEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_panels_add_panel_picklist(ow_device* dev, bool use_tile, int32_t tile_id, int32_t icon_id, int32_t log_index, const char* back_color, const char* fore_color, const char* caption)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, use_tile ? 1 : 0);
    ao = ow__pack_i32(args, ao, tile_id);
    ao = ow__pack_i32(args, ao, icon_id);
    ao = ow__pack_i32(args, ao, log_index);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_str(args, ao, fore_color);
    ao = ow__pack_str(args, ao, caption);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_PANELS_ADD_PANEL_PICKLIST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_panels_show_panel(ow_device* dev, int32_t index)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_PANELS_SHOW_PANEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_led(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t color, int32_t size, bool inital_value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, color);
    ao = ow__pack_i32(args, ao, size);
    ao = ow__pack_u8(args, ao, inital_value ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_LED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_log_list(ow_device* dev, int32_t index, int32_t log, int32_t x, int32_t y, int32_t width, int32_t height, int32_t font_type, int32_t font_size, const char* back_color, const char* fore_color, bool list_mode)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, log);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_i32(args, ao, font_type);
    ao = ow__pack_i32(args, ao, font_size);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_str(args, ao, fore_color);
    ao = ow__pack_u8(args, ao, list_mode ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_LOG_LIST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_plot(ow_device* dev, int32_t index, int32_t plot_data_index_bit_field, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min_y, int32_t max_y, const char* back_color)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, plot_data_index_bit_field);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_i32(args, ao, min_y);
    ao = ow__pack_i32(args, ao, max_y);
    ao = ow__pack_str(args, ao, back_color);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_PLOT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_number(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, bool is_float, int32_t float_digit_count, bool is_hex_format, bool is_unsigned)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, font_type);
    ao = ow__pack_i32(args, ao, font_size);
    ao = ow__pack_str(args, ao, fore_color);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_u8(args, ao, is_float ? 1 : 0);
    ao = ow__pack_i32(args, ao, float_digit_count);
    ao = ow__pack_u8(args, ao, is_hex_format ? 1 : 0);
    ao = ow__pack_u8(args, ao, is_unsigned ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_NUMBER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_text(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t font_type, int32_t font_size, const char* fore_color, const char* back_color, const char* text)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, font_type);
    ao = ow__pack_i32(args, ao, font_size);
    ao = ow__pack_str(args, ao, fore_color);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_str(args, ao, text);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_TEXT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_bargraph(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* bar_color)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_i32(args, ao, min);
    ao = ow__pack_i32(args, ao, max);
    ao = ow__pack_str(args, ao, bar_color);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_BARGRAPH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_meter(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t min, int32_t max, const char* needle_color)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_i32(args, ao, min);
    ao = ow__pack_i32(args, ao, max);
    ao = ow__pack_str(args, ao, needle_color);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_METER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_button(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, const char* fore_color, const char* back_color, const char* text)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_str(args, ao, fore_color);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_str(args, ao, text);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_BUTTON, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_picture(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t picture_id)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, picture_id);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_PICTURE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_picture_from_file(ow_device* dev, int32_t index, int32_t x, int32_t y, const char* picture_path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_str(args, ao, picture_path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_PICTURE_FROM_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_waterfall(ow_device* dev, int32_t index, int32_t plot_data_index, int32_t bin_count, int32_t x, int32_t y, int32_t width, int32_t height, const char* back_color)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, plot_data_index);
    ao = ow__pack_i32(args, ao, bin_count);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_str(args, ao, back_color);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_WATERFALL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_text(ow_device* dev, int32_t index, const char* text)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_str(args, ao, text);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_TEXT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_int(ow_device* dev, int32_t index, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_INT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_float(ow_device* dev, int32_t index, double value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_f32(args, ao, (float)value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_FLOAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_list_item_text(ow_device* dev, int32_t log_index, int32_t list_item, int32_t color, const char* text)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, log_index);
    ao = ow__pack_i32(args, ao, list_item);
    ao = ow__pack_i32(args, ao, color);
    ao = ow__pack_str(args, ao, text);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_LIST_ITEM_TEXT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_min_max_int(ow_device* dev, int32_t index, bool enable, int32_t min, int32_t max)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_u8(args, ao, enable ? 1 : 0);
    ao = ow__pack_i32(args, ao, min);
    ao = ow__pack_i32(args, ao, max);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_MIN_MAX_INT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_value_min_max_float(ow_device* dev, int32_t index, bool enable, double min, double max)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_u8(args, ao, enable ? 1 : 0);
    ao = ow__pack_f32(args, ao, (float)min);
    ao = ow__pack_f32(args, ao, (float)max);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_VALUE_MIN_MAX_FLOAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_plot_data(ow_device* dev, int32_t plot_data_index, int32_t settings, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, plot_data_index);
    ao = ow__pack_i32(args, ao, settings);
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_PLOT_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_list_item_selected(ow_device* dev, int32_t log_index, int32_t list_index)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, log_index);
    ao = ow__pack_i32(args, ao, list_index);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_LIST_ITEM_SELECTED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_list_item_top_index(ow_device* dev, int32_t log_item, int32_t list_index)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, log_item);
    ao = ow__pack_i32(args, ao, list_index);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_LIST_ITEM_TOP_INDEX, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_control_properties_set_control_property(ow_device* dev, int32_t index, int32_t property, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, property);
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROL_PROPERTIES_SET_CONTROL_PROPERTY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_message_box(ow_device* dev, int32_t auto_close_half_sec, bool show_ok, bool show_ok_cancel, bool show_none, int32_t picture_index, const char* message)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, auto_close_half_sec);
    ao = ow__pack_u8(args, ao, show_ok ? 1 : 0);
    ao = ow__pack_u8(args, ao, show_ok_cancel ? 1 : 0);
    ao = ow__pack_u8(args, ao, show_none ? 1 : 0);
    ao = ow__pack_i32(args, ao, picture_index);
    ao = ow__pack_str(args, ao, message);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_MESSAGE_BOX, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_set_dialog_description(ow_device* dev, const char* description)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, description);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_SET_DIALOG_DESCRIPTION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_progress_bar(ow_device* dev, int32_t picture_index, bool ok_to_close, bool auto_close_at100, int32_t auto_close_half_sec, const char* title)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, picture_index);
    ao = ow__pack_u8(args, ao, ok_to_close ? 1 : 0);
    ao = ow__pack_u8(args, ao, auto_close_at100 ? 1 : 0);
    ao = ow__pack_i32(args, ao, auto_close_half_sec);
    ao = ow__pack_str(args, ao, title);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_PROGRESS_BAR, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_number_edit(ow_device* dev, int32_t min, int32_t max, int32_t initial, bool use_min_max, bool is_unsigned, bool hex_fomat, const char* message)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, min);
    ao = ow__pack_i32(args, ao, max);
    ao = ow__pack_i32(args, ao, initial);
    ao = ow__pack_u8(args, ao, use_min_max ? 1 : 0);
    ao = ow__pack_u8(args, ao, is_unsigned ? 1 : 0);
    ao = ow__pack_u8(args, ao, hex_fomat ? 1 : 0);
    ao = ow__pack_str(args, ao, message);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_NUMBER_EDIT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_number_edit_float(ow_device* dev, double min, double max, double initial, bool use_min_max, int32_t digit_count, const char* message)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_f32(args, ao, (float)min);
    ao = ow__pack_f32(args, ao, (float)max);
    ao = ow__pack_f32(args, ao, (float)initial);
    ao = ow__pack_u8(args, ao, use_min_max ? 1 : 0);
    ao = ow__pack_i32(args, ao, digit_count);
    ao = ow__pack_str(args, ao, message);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_NUMBER_EDIT_FLOAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_text_edit(ow_device* dev, const char* message, const char* inital_value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, message);
    ao = ow__pack_str(args, ao, inital_value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_TEXT_EDIT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_pick_list(ow_device* dev, int32_t log_index, const char* message)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, log_index);
    ao = ow__pack_str(args, ao, message);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_PICK_LIST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_show_text_editor(ow_device* dev, int32_t editor_type, const char* message, const char* inital_value, bool* basic)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, editor_type);
    ao = ow__pack_str(args, ao, message);
    ao = ow__pack_str(args, ao, inital_value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_SHOW_TEXT_EDITOR, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (basic) *basic = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    return OW_OK;
}

ow_status ow_gui_dialogs_set_progess_dialog_value(ow_device* dev, int32_t value0_to100)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value0_to100);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_SET_PROGESS_DIALOG_VALUE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_software_reset(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOFTWARE_RESET, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_software_reset_to_bootloader(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOFTWARE_RESET_TO_BOOTLOADER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_all_settings_to_defaults(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ALL_SETTINGS_TO_DEFAULTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_baud_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_BAUD_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_r_ts_hand_shaking(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_R_TS_HAND_SHAKING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_c_ts_hand_shaking(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_C_TS_HAND_SHAKING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_data_bits(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_DATA_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_parity(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_PARITY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_stop_bits(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_STOP_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_uart_settings_module(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_UART_SETTINGS_MODULE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_i2c_settings_frequency(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_I2C_SETTINGS_FREQUENCY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_i2c_settings_pull_ups(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_I2C_SETTINGS_PULL_UPS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_accel_range(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_ACCEL_RANGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_gyro_range(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_GYRO_RANGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_move_threshold(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_MOVE_THRESHOLD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_t_cal_scale(ow_device* dev, double value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_f32(args, ao, (float)value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_T_CAL_SCALE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_t_cal_offset(ow_device* dev, double value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_f32(args, ao, (float)value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_T_CAL_OFFSET, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sensor_settings_stream_defaults(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SENSOR_SETTINGS_STREAM_DEFAULTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_frequency(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_FREQUENCY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_chip_select_pin(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_CHIP_SELECT_PIN, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_data_bits(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_DATA_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_c_pol(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_C_POL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_spi_settings_c_pha(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SPI_SETTINGS_C_PHA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1_rx12(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1_RX12, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_g_pio2626(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_G_PIO2626, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1cs13(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1CS13, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_g_pio27(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_G_PIO27, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1_rx9(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1_RX9, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1cts10(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1CTS10, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1_tx8(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1_TX8, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_u_art1rts11(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_U_ART1RTS11, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1_tx15(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1_TX15, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_s_pi1sclk14(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_S_PI1SCLK14, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_io_direction_settings_g_pio2525(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_IO_DIRECTION_SETTINGS_G_PIO2525, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_clk_source(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_CLK_SOURCE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_int(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_CLK_DIVIDER_INT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_clk_divider_frac(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_CLK_DIVIDER_FRAC, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_fpga_clock_settings_comms_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_FPGA_CLOCK_SETTINGS_COMMS_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_frequency_mhz(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_FREQUENCY_MHZ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_modulation(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_MODULATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_devation(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_DEVATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_channel(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_CHANNEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_channel_spacing(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_CHANNEL_SPACING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_rx_bandwidth(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_RX_BANDWIDTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_data_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_DATA_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_power_amp(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_POWER_AMP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_sync_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_SYNC_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_sync_word(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_SYNC_WORD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_addr_check(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_ADDR_CHECK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_address(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_ADDRESS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_white_data(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_WHITE_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_packet_format(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_PACKET_FORMAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_length_config(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_LENGTH_CONFIG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_packet_length(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_PACKET_LENGTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_c_rc_enabled(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_C_RC_ENABLED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_c_rc_auto_flush(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_C_RC_AUTO_FLUSH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_d_c_blocking_filter(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_D_C_BLOCKING_FILTER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_manchester(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_MANCHESTER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_forword_error_correction(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_FORWORD_ERROR_CORRECTION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_preamble_bytes(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_PREAMBLE_BYTES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_p_qt(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_P_QT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_append_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_APPEND_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_frequency_mhz(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_FREQUENCY_MHZ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_modulation(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_MODULATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_devation(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_DEVATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_channel(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_CHANNEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_channel_spacing(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_CHANNEL_SPACING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_rx_bandwidth(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_RX_BANDWIDTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_data_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_DATA_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_power_amp(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_POWER_AMP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_sync_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_SYNC_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_sync_word(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_SYNC_WORD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_addr_check(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_ADDR_CHECK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_address(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_ADDRESS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_white_data(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_WHITE_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_packet_format(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_PACKET_FORMAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_length_config(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_LENGTH_CONFIG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_packet_length(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_PACKET_LENGTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_c_rc_enabled(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_C_RC_ENABLED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_c_rc_auto_flush(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_C_RC_AUTO_FLUSH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_d_c_blocking_filter(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_D_C_BLOCKING_FILTER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_manchester(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_MANCHESTER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_forword_error_correction(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_FORWORD_ERROR_CORRECTION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_preamble_bytes(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_PREAMBLE_BYTES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_p_qt(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_P_QT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_settings_2_append_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_SETTINGS_2_APPEND_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_radio_fa_settings_default_view(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RADIO_FA_SETTINGS_DEFAULT_VIEW, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_year(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_YEAR, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_month(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_MONTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_day(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_DAY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_day_of_week(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_DAY_OF_WEEK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_hours(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_HOURS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_minutes(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_MINUTES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_seconds(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_SECONDS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_rtc_settings_trim(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_RTC_SETTINGS_TRIM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_enable_station_mode(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_ENABLE_STATION_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_S_SID_FOR_STATION_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_password_for_station_mode(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_PASSWORD_FOR_STATION_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_enable_ap_mode(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_ENABLE_AP_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_a_p_auth(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_A_P_AUTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_a_p_hide_ssid(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_A_P_HIDE_SSID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_S_SID_FOR_AP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_wifi_settings_password_for_ap(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WIFI_SETTINGS_PASSWORD_FOR_AP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_ble_settings_enable_bt(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_BLE_SETTINGS_ENABLE_BT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_ble_settings_b_t_terminal(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_BLE_SETTINGS_B_T_TERMINAL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_ble_settings_b_t_advert_name(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_BLE_SETTINGS_B_T_ADVERT_NAME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_orca_settings_orca_com_over_uart(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ORCA_SETTINGS_ORCA_COM_OVER_UART, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_start_ws_server(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_START_WS_SERVER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_w_s_server_port(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_W_S_SERVER_PORT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_auth_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_AUTH_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_auth_username(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_AUTH_USERNAME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_websocket_settings_auth_password(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_WEBSOCKET_SETTINGS_AUTH_PASSWORD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1fdd_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1FDD_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_listen_only(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_LISTEN_ONLY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_tx_retry(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_TX_RETRY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_baud(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_CUST_BAUD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_cust_data_baud(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_CUST_DATA_BAUD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1_termination(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1_TERMINATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an1api_enabled(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN1API_ENABLED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_anapiid(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_ANAPIID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2fdd_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2FDD_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_listen_only(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_LISTEN_ONLY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_tx_retry(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_TX_RETRY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_baud(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_CUST_BAUD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_cust_data_baud(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_CUST_DATA_BAUD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2_termination(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2_TERMINATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_c_an2api_enabled(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_C_AN2API_ENABLED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_l_in_master_en(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_L_IN_MASTER_EN, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_l_in_baud_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_L_IN_BAUD_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_neptune_settings_analog_in_en(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_NEPTUNE_SETTINGS_ANALOG_IN_EN, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_startup_wasm_script(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_STARTUP_WASM_SCRIPT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_startup_zoom_script(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_STARTUP_ZOOM_SCRIPT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_default_fpga_script(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_DEFAULT_FPGA_SCRIPT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_general_settings_wasm_debug_level(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_GENERAL_SETTINGS_WASM_DEBUG_LEVEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch0_input(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH0_INPUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch1_input(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH1_INPUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch2_input(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH2_INPUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch3_input(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH3_INPUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch0_range(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH0_RANGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch1_range(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH1_RANGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch2_range(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH2_RANGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_ch3_range(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_CH3_RANGE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_analog_in_settings_data_rate(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_ANALOG_IN_SETTINGS_DATA_RATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_system_enable_battery_stream(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SYSTEM_ENABLE_BATTERY_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_change_directory(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_CHANGE_DIRECTORY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_create_directory(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_CREATE_DIRECTORY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_remove_file_or_directory(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_REMOVE_FILE_OR_DIRECTORY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_get_file_from_pc(ow_device* dev, const char* path, int32_t size, int32_t crc32)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    ao = ow__pack_i32(args, ao, size);
    ao = ow__pack_i32(args, ao, crc32);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_GET_FILE_FROM_PC, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_send_file_to_pc(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_SEND_FILE_TO_PC, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_print_file(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_PRINT_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_create_blank_file(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_CREATE_BLANK_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_edit_file(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_EDIT_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_rename_or_move_file_directory(ow_device* dev, const char* path, const char* new_path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    ao = ow__pack_str(args, ao, new_path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_RENAME_OR_MOVE_FILE_DIRECTORY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_list_directory(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_LIST_DIRECTORY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_format_file_system(ow_device* dev, const char* confirm)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, confirm);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_FORMAT_FILE_SYSTEM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_toggle_sd_card_host_select(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_TOGGLE_SD_CARD_HOST_SELECT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_set_sd_card_host(ow_device* dev, int32_t host)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, host);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_SET_SD_CARD_HOST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_list_zones(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_LIST_ZONES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_get_zones(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_GET_ZONES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_zone(ow_device* dev, int32_t zone, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, zone);
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_SET_ZONE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_zone_mask(ow_device* dev, int32_t mask)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, mask);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_SET_ZONE_MASK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_get_power_state(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_GET_POWER_STATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_enable_power_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_ENABLE_POWER_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_enable_reader(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_ENABLE_READER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_enter_bootloader(ow_device* dev, int32_t upgrade_transmission_rate)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, upgrade_transmission_rate);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_ENTER_BOOTLOADER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_enter_application(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_ENTER_APPLICATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_get_i_dand_security(ow_device* dev, int32_t* esp_chip_id, int32_t* version, bool* sb_en, bool* sbar_en, bool* sdm_en, bool* sbrk_1, bool* sbrk_2, bool* sbrk_3, bool* jtag_sw_dis, bool* jtag_hw_dis, bool* flash_enc_en, bool* dcache_dis, bool* icache_dis)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_GET_I_DAND_SECURITY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (esp_chip_id) *esp_chip_id = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (version) *version = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (sb_en) *sb_en = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (sbar_en) *sbar_en = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (sdm_en) *sdm_en = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (sbrk_1) *sbrk_1 = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (sbrk_2) *sbrk_2 = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (sbrk_3) *sbrk_3 = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (jtag_sw_dis) *jtag_sw_dis = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (jtag_hw_dis) *jtag_hw_dis = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (flash_enc_en) *flash_enc_en = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (dcache_dis) *dcache_dis = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (icache_dis) *icache_dis = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_read_flash_size(ow_device* dev, int32_t* flash_size_bytes)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_READ_FLASH_SIZE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (flash_size_bytes) *flash_size_bytes = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_read_esp32mac(ow_device* dev, char* esp32_mac, size_t esp32_mac_cap)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_READ_ESP32MAC, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_str(ret, &ro, esp32_mac, (int)esp32_mac_cap);
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_erase_all_flash(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_ERASE_ALL_FLASH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_start_flash_operations(ow_device* dev, uint32_t offset, int32_t size, int32_t block_size)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, offset);
    ao = ow__pack_i32(args, ao, size);
    ao = ow__pack_i32(args, ao, block_size);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_START_FLASH_OPERATIONS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_stop_flash_operation(ow_device* dev, bool reboot)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, reboot ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_STOP_FLASH_OPERATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_write(ow_device* dev, const uint8_t* flash_data, size_t flash_data_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, flash_data, (uint32_t)flash_data_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_FLASH_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_read(ow_device* dev, uint32_t offset, int32_t size)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, offset);
    ao = ow__pack_i32(args, ao, size);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_FLASH_READ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_start_write_memory_operations(ow_device* dev, uint32_t offset, uint32_t memory_block, int32_t block_size)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, offset);
    ao = ow__pack_u32(args, ao, memory_block);
    ao = ow__pack_i32(args, ao, block_size);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_START_WRITE_MEMORY_OPERATIONS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_memory_write(ow_device* dev, uint32_t offset, uint32_t memory_block, int32_t block_size)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, offset);
    ao = ow__pack_u32(args, ao, memory_block);
    ao = ow__pack_i32(args, ao, block_size);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_MEMORY_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_stop_memory_operation(ow_device* dev, uint32_t entry_address)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, entry_address);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_STOP_MEMORY_OPERATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_register_write(ow_device* dev, uint32_t offset, uint32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, offset);
    ao = ow__pack_u32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_REGISTER_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_register_read(ow_device* dev, uint32_t offset, uint32_t* memory_block)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, offset);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_REGISTER_READ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (memory_block) *memory_block = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_default(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_FLASH_DEFAULT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_toggle_events(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_TOGGLE_EVENTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_start_access_point(ow_device* dev, const char* ssid, const char* password, int32_t authmode, bool hidessid)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, ssid);
    ao = ow__pack_str(args, ao, password);
    ao = ow__pack_i32(args, ao, authmode);
    ao = ow__pack_u8(args, ao, hidessid ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_START_ACCESS_POINT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_discconect_from_station(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_DISCCONECT_FROM_STATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_get_connected_devices(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_GET_CONNECTED_DEVICES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_connect_to_station(ow_device* dev, const char* ssid, const char* password)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, ssid);
    ao = ow__pack_str(args, ao, password);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_CONNECT_TO_STATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_discconect_from_station_2(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_DISCCONECT_FROM_STATION_2, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_scan_for_access_points(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_SCAN_FOR_ACCESS_POINTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_get_wif_info(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_GET_WIF_INFO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_enable_station_mode(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_ENABLE_STATION_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_s_sid_for_station_mode(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_S_SID_FOR_STATION_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_password_for_station_mode(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_PASSWORD_FOR_STATION_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_enable_ap_mode(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_ENABLE_AP_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_a_p_auth(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_A_P_AUTH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_a_p_hide_ssid(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_A_P_HIDE_SSID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_s_sid_for_ap(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_S_SID_FOR_AP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_settings_password_for_ap(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_SETTINGS_PASSWORD_FOR_AP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_start_bt_advertising(ow_device* dev, const char* hostname)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, hostname);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_ON_START_BT_ADVERTISING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_stop_bt_advertising(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_ON_STOP_BT_ADVERTISING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_scan_bt_devices(ow_device* dev, int32_t durationms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, durationms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_ON_SCAN_BT_DEVICES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_enable_terminal(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_ON_ENABLE_TERMINAL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_ble_settings_enable_bt(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_BLE_SETTINGS_ENABLE_BT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_ble_settings_b_t_terminal(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_BLE_SETTINGS_B_T_TERMINAL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_ble_settings_b_t_advert_name(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_BLE_SETTINGS_B_T_ADVERT_NAME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_enable_ir_stream(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_ENABLE_IR_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_send_ir_data(ow_device* dev, int32_t ir_code)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, ir_code);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_SEND_IR_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_configure(ow_device* dev, int32_t freq_hz, int32_t sf, int32_t bw_enc, int32_t cr, int32_t power, int32_t preamble, uint8_t sync)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, freq_hz);
    ao = ow__pack_i32(args, ao, sf);
    ao = ow__pack_i32(args, ao, bw_enc);
    ao = ow__pack_i32(args, ao, cr);
    ao = ow__pack_i32(args, ao, power);
    ao = ow__pack_i32(args, ao, preamble);
    ao = ow__pack_u8(args, ao, sync);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_LO_RA_CONFIGURE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_send_payload(ow_device* dev, const uint8_t* data, size_t data_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, data, (uint32_t)data_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_LO_RA_SEND_PAYLOAD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_rx_enable(ow_device* dev, int32_t mode)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, mode);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_LO_RA_RX_ENABLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_LO_RA_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_lo_ra_raw_frame(ow_device* dev, uint8_t cmd_, const uint8_t* payload, size_t payload_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, cmd_);
    ao = ow__pack_bytes(args, ao, payload, (uint32_t)payload_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_LO_RA_RAW_FRAME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_launch_script(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_LAUNCH_SCRIPT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_enable_rx_stream(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_ENABLE_RX_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_send_data(ow_device* dev, int32_t delay, const uint8_t* data, size_t data_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, delay);
    ao = ow__pack_bytes(args, ao, data, (uint32_t)data_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_SEND_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_update_table_data(ow_device* dev, int32_t table_index, int32_t delay, const uint8_t* data, size_t data_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, table_index);
    ao = ow__pack_i32(args, ao, delay);
    ao = ow__pack_bytes(args, ao, data, (uint32_t)data_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_UPDATE_TABLE_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_enable_schedule_table(ow_device* dev, int32_t number_of_entries)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, number_of_entries);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_ENABLE_SCHEDULE_TABLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_compile_test(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_COMPILE_TEST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_run_zio(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_RUN_ZIO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_zoom_io_stop_zio(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_ZOOM_IO_STOP_ZIO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_start(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_breakpoints(ow_device* dev, const char* pcs)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, pcs);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_BREAKPOINTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_step(ow_device* dev, const char* range)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, range);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_STEP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_continue(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_CONTINUE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_pause(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_PAUSE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_stop(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_locals(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_LOCALS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wasm_debug_debug_mem_read(ow_device* dev, const char* addr)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, addr);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WASM_DEBUG_DEBUG_MEM_READ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_start(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_breakpoints(ow_device* dev, const char* lines)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, lines);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_BREAKPOINTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_step(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_STEP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_continue(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_CONTINUE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_pause(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_PAUSE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_stop(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_rthon_debug_debug_locals(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_RTHON_DEBUG_DEBUG_LOCALS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_apps_launch_app(ow_device* dev, int32_t app_id)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, app_id);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_APPS_LAUNCH_APP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_linux_enable_linux_cpu(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LINUX_ENABLE_LINUX_CPU, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_wili8(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t scale, const char* back_color, int32_t animation, const char* script_path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_i32(args, ao, scale);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_i32(args, ao, animation);
    ao = ow__pack_str(args, ao, script_path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_WILI8, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_controls_add_file_list(ow_device* dev, int32_t index, int32_t x, int32_t y, int32_t width, int32_t height, int32_t mode, const char* back_color, const char* start_path, const char* filter)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_i32(args, ao, x);
    ao = ow__pack_i32(args, ao, y);
    ao = ow__pack_i32(args, ao, width);
    ao = ow__pack_i32(args, ao, height);
    ao = ow__pack_i32(args, ao, mode);
    ao = ow__pack_str(args, ao, back_color);
    ao = ow__pack_str(args, ao, start_path);
    ao = ow__pack_str(args, ao, filter);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_CONTROLS_ADD_FILE_LIST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_dialogs_file_picker(ow_device* dev, int32_t mode, const char* start_path, const char* filter)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, mode);
    ao = ow__pack_str(args, ao, start_path);
    ao = ow__pack_str(args, ao, filter);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_DIALOGS_FILE_PICKER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_file_system_load_wili_project(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_FILE_SYSTEM_LOAD_WILI_PROJECT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_serial_leds_configure_strip(ow_device* dev, int32_t strip, int32_t gpio, int32_t length, ow_ow_serial_led_type led_type, bool inverted)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, strip);
    ao = ow__pack_i32(args, ao, gpio);
    ao = ow__pack_i32(args, ao, length);
    ao = ow__pack_i32(args, ao, (int32_t)led_type);
    ao = ow__pack_u8(args, ao, inverted ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SERIAL_LEDS_CONFIGURE_STRIP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_serial_leds_show_config(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SERIAL_LEDS_SHOW_CONFIG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_serial_leds_set_leds(ow_device* dev, int32_t strip, int32_t start, int32_t count, int32_t red, int32_t green, int32_t blue, int32_t white)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, strip);
    ao = ow__pack_i32(args, ao, start);
    ao = ow__pack_i32(args, ao, count);
    ao = ow__pack_i32(args, ao, red);
    ao = ow__pack_i32(args, ao, green);
    ao = ow__pack_i32(args, ao, blue);
    ao = ow__pack_i32(args, ao, white);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SERIAL_LEDS_SET_LEDS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_serial_leds_set_show(ow_device* dev, int32_t strip, ow_ow_led_light_show show)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, strip);
    ao = ow__pack_i32(args, ao, (int32_t)show);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SERIAL_LEDS_SET_SHOW, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_serial_leds_enable_jambu_orca(ow_device* dev, int32_t num_strips)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, num_strips);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SERIAL_LEDS_ENABLE_JAMBU_ORCA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_serial_leds_auto_show(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SERIAL_LEDS_AUTO_SHOW, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_system_read_otp_info(ow_device* dev, int32_t offset, int32_t length, uint8_t* otp_blob, size_t otp_blob_cap, size_t* otp_blob_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, offset);
    ao = ow__pack_i32(args, ao, length);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SYSTEM_READ_OTP_INFO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_bytes(ret, &ro, otp_blob, (int)otp_blob_cap, (int*)otp_blob_len);
    return OW_OK;
}

ow_status ow_io_gpio_set_io_voltage_source(ow_device* dev, int32_t source)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, source);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_GPIO_SET_IO_VOLTAGE_SOURCE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_set_cm0_run_line(ow_device* dev, ow_reset_line_state state)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, (int32_t)state);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_SET_CM0_RUN_LINE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_power_management_get_control_lines(ow_device* dev, bool* wio_released, bool* cm0_released, bool* main_rst_high)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_GET_CONTROL_LINES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (wio_released) *wio_released = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (cm0_released) *cm0_released = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (main_rst_high) *main_rst_high = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    return OW_OK;
}

ow_status ow_hardware_power_management_set_wio_reset_line(ow_device* dev, ow_reset_line_state state)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, (int32_t)state);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_POWER_MANAGEMENT_SET_WIO_RESET_LINE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_linux_open_shell(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LINUX_OPEN_SHELL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_print_card_info(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_PRINT_CARD_INFO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_get_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_GET_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_begin(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_BEGIN, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_cmd(ow_device* dev, uint32_t command)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, command);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_CMD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_end(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_END, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_field(ow_device* dev, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_FIELD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_reg_read(ow_device* dev, uint32_t addr, uint32_t* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, addr);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_REG_READ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (value) *value = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_transceive(ow_device* dev, uint32_t flags, int32_t timeout_ms, const uint8_t* tx, size_t tx_len, uint32_t* status, uint8_t* rx, size_t rx_cap, size_t* rx_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, flags);
    ao = ow__pack_i32(args, ao, timeout_ms);
    ao = ow__pack_bytes(args, ao, tx, (uint32_t)tx_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_TRANSCEIVE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (status) *status = ow__take_u32(ret, &ro); else ow__take_u32(ret, &ro);
    ow__take_bytes(ret, &ro, rx, (int)rx_cap, (int*)rx_len);
    return OW_OK;
}

ow_status ow_wireless_nfc_raw_reg_write(ow_device* dev, uint32_t addr, uint32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u32(args, ao, addr);
    ao = ow__pack_u32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_RAW_REG_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_mifare_classic_dictionary_attack(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_MIFARE_CLASSIC_DICTIONARY_ATTACK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_mifare_classic_read_with_keys(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_MIFARE_CLASSIC_READ_WITH_KEYS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_mifare_classic_dump_card(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_MIFARE_CLASSIC_DUMP_CARD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_list_saved_cards(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_SAVED_CARDS_LIST_SAVED_CARDS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_load_card(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_SAVED_CARDS_LOAD_CARD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_save_current_card(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_SAVED_CARDS_SAVE_CURRENT_CARD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_extra_halt_card(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_EXTRA_HALT_CARD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_analyzer(ow_device* dev, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_ANALYZER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_select_band(ow_device* dev, int32_t band)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, band);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_SELECT_BAND, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_comm_check(ow_device* dev, uint8_t* version)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_COMM_CHECK, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (version) *version = ow__take_u8(ret, &ro); else ow__take_u8(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_radio_release_circuit(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_RELEASE_CIRCUIT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_set_frequency(ow_device* dev, int32_t freq_hz, int32_t* band)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, freq_hz);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_SET_FREQUENCY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (band) *band = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_radio_capture_start(ow_device* dev, int32_t freq_hz, int32_t preset)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, freq_hz);
    ao = ow__pack_i32(args, ao, preset);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_CAPTURE_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_read_rssi(ow_device* dev, int32_t* rssi)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_READ_RSSI, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (rssi) *rssi = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_radio_capture_stop(ow_device* dev, int32_t* durations)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_CAPTURE_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (durations) *durations = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_radio_packet_read(ow_device* dev, int32_t* rssi, int32_t* seq, uint8_t* data, size_t data_cap, size_t* data_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_PACKET_READ, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (rssi) *rssi = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (seq) *seq = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    ow__take_bytes(ret, &ro, data, (int)data_cap, (int*)data_len);
    return OW_OK;
}

ow_status ow_wireless_radio_monitor(ow_device* dev, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_MONITOR, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_transmit_sub_file(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_TRANSMIT_SUB_FILE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_spectrum(ow_device* dev, int32_t* peak_freq_hz, int32_t* peak_rssi, uint8_t* bins, size_t bins_cap, size_t* bins_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_SPECTRUM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (peak_freq_hz) *peak_freq_hz = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (peak_rssi) *peak_rssi = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    ow__take_bytes(ret, &ro, bins, (int)bins_cap, (int*)bins_len);
    return OW_OK;
}

ow_status ow_wireless_radio_carrier(ow_device* dev, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_CARRIER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_replay(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_REPLAY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_rx_enable(ow_device* dev, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_RX_ENABLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_select_circuit(ow_device* dev, int32_t band)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, band);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_SELECT_CIRCUIT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_read_state(ow_device* dev, int32_t* owner, int32_t* holder, int32_t* band, int32_t* want_v1, int32_t* want_v2, int32_t* have_valid, int32_t* have_v1, int32_t* have_v2, int32_t* lora_paused, int32_t* freq_hz, int32_t* active, int32_t* status, uint8_t* version)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_READ_STATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (owner) *owner = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (holder) *holder = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (band) *band = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (want_v1) *want_v1 = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (want_v2) *want_v2 = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (have_valid) *have_valid = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (have_v1) *have_v1 = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (have_v2) *have_v2 = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (lora_paused) *lora_paused = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (freq_hz) *freq_hz = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (active) *active = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (status) *status = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (version) *version = ow__take_u8(ret, &ro); else ow__take_u8(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_radio_squelch(ow_device* dev, int32_t dbm)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, dbm);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_SQUELCH, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_replay_invert(ow_device* dev, int32_t on)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_REPLAY_INVERT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_idle(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_IDLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_packet_send(ow_device* dev, int32_t freq_hz, const uint8_t* data, size_t data_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, freq_hz);
    ao = ow__pack_bytes(args, ao, data, (uint32_t)data_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_PACKET_SEND, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_radio_packet_rx(ow_device* dev, int32_t on, int32_t freq_hz)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, on);
    ao = ow__pack_i32(args, ao, freq_hz);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RADIO_PACKET_RX, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_system_device_state(ow_device* dev, char* sd, size_t sd_cap, bool* hoststream, char* activemask, size_t activemask_cap)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SYSTEM_DEVICE_STATE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_str(ret, &ro, sd, (int)sd_cap);
    if (hoststream) *hoststream = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    ow__take_str(ret, &ro, activemask, (int)activemask_cap);
    return OW_OK;
}

ow_status ow_hardware_system_event_host_streaming(ow_device* dev, int32_t enable, bool* enabled)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SYSTEM_EVENT_HOST_STREAMING, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (enabled) *enabled = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    return OW_OK;
}

ow_status ow_gui_screenshot(ow_device* dev, const char* filename, ow_ow_screenshot_file_type filetype, bool counter, bool timestamp)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    ao = ow__pack_i32(args, ao, (int32_t)filetype);
    ao = ow__pack_u8(args, ao, counter ? 1 : 0);
    ao = ow__pack_u8(args, ao, timestamp ? 1 : 0);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_SCREENSHOT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_gui_simulate_keypress(ow_device* dev, ow_ow_gui_button button, ow_ow_button_press_type presstype)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, (int32_t)button);
    ao = ow__pack_i32(args, ao, (int32_t)presstype);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_SIMULATE_KEYPRESS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_load(ow_device* dev, const char* filepath)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filepath);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WILI_FILES_WILI_LOAD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_default(ow_device* dev, const char* filepath)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filepath);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WILI_FILES_WILI_DEFAULT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_reset(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WILI_FILES_WILI_RESET, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_save(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WILI_FILES_WILI_SAVE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_wili_files_wili_remove_default(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_WILI_FILES_WILI_REMOVE_DEFAULT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_add(ow_device* dev, const char* name)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_ADD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_get(ow_device* dev, const char* name, char* name_out, size_t name_out_cap, double* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_GET, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    ow__take_str(ret, &ro, name_out, (int)name_out_cap);
    if (value) *value = ow__take_f32(ret, &ro); else ow__take_f32(ret, &ro);
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_rename(ow_device* dev, const char* name, const char* new_name)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    ao = ow__pack_str(args, ao, new_name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_RENAME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_set(ow_device* dev, const char* name, double value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    ao = ow__pack_f32(args, ao, (float)value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_SET, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_stream(ow_device* dev, int32_t stream_rate_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, stream_rate_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_STREAM, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_wave(ow_device* dev, const char* name, int32_t wave)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    ao = ow__pack_i32(args, ao, wave);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_WAVE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_app_signals_app_signal_remove(ow_device* dev, const char* name)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_APP_SIGNALS_APP_SIGNAL_REMOVE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_trigger_button(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_TRIGGER_BUTTON, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_stop(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_file_format(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_FILE_FORMAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_trigger_mode(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_TRIGGER_MODE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_post_trigger_ms(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_POST_TRIGGER_MS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_pre_trigger_ms(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_PRE_TRIGGER_MS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_start(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_trigger(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_TRIGGER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_events(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_EVENTS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_trigger_expression(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_TRIGGER_EXPRESSION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_active_instance(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_ACTIVE_INSTANCE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_logger_name(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_LOGGER_NAME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_apps_run_app(ow_device* dev, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_APPS_RUN_APP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_system_boot_uf2(ow_device* dev, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SYSTEM_BOOT_UF2, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_power_cycle_display_cpu(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_POWER_CYCLE_DISPLAY_CPU, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_set_ram_app_arg(ow_device* dev, const char* text)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, text);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_SET_RAM_APP_ARG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_list_display_apps(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_LIST_DISPLAY_APPS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_restore_display_firmware(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_RESTORE_DISPLAY_FIRMWARE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_display_bl_version(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_DISPLAY_BL_VERSION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_reset_display_cpu(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_RESET_DISPLAY_CPU, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_run_psram_app(ow_device* dev, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_RUN_PSRAM_APP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_scripting_power_cycle_debugger(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_SCRIPTING_POWER_CYCLE_DEBUGGER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_get_time(ow_device* dev, int32_t* year, int32_t* month, int32_t* day, int32_t* weekday, int32_t* hour, int32_t* min, int32_t* sec)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_GET_TIME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (year) *year = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (month) *month = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (day) *day = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (weekday) *weekday = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (hour) *hour = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (min) *min = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (sec) *sec = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_hardware_set_time(ow_device* dev, int32_t year, int32_t month, int32_t day, int32_t hour, int32_t min, int32_t sec)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, year);
    ao = ow__pack_i32(args, ao, month);
    ao = ow__pack_i32(args, ao, day);
    ao = ow__pack_i32(args, ao, hour);
    ao = ow__pack_i32(args, ao, min);
    ao = ow__pack_i32(args, ao, sec);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SET_TIME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_self_test(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_IR_SELF_TEST, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_list_dir(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_IR_LIST_DIR, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_list_buttons(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_IR_LIST_BUTTONS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_send_button(ow_device* dev, int32_t index, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, index);
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_IR_SEND_BUTTON, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_save_capture(ow_device* dev, const char* name)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, name);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_IR_SAVE_CAPTURE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_ir_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_IR_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_i_r_carrier(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_I_R_CARRIER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_ir_i_r_repeat(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_IR_I_R_REPEAT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_load_psram_data(ow_device* dev, const char* filename, uint32_t offset)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    ao = ow__pack_u32(args, ao, offset);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_LOAD_PSRAM_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_from_folder(ow_device* dev, const char* folder)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, folder);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_FLASH_FROM_FOLDER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_esp32_flasher_flash_status(ow_device* dev, bool* flashing, int32_t* progress, int32_t* partition_index, int32_t* partition_count)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_ESP32_FLASHER_FLASH_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (flashing) *flashing = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (progress) *progress = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (partition_index) *partition_index = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (partition_count) *partition_count = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_io_spi_s_pi_slave_enable(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_S_PI_SLAVE_ENABLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_slave_enable(ow_device* dev, uint8_t address)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_u8(args, ao, address);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_I2C_SLAVE_ENABLE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_spi_s_pi_slave_set_data(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_SPI_S_PI_SLAVE_SET_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_i2c_i2c_slave_set_data(ow_device* dev, const uint8_t* data_bytes, size_t data_bytes_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, data_bytes, (uint32_t)data_bytes_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_I2C_I2C_SLAVE_SET_DATA, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_start(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICE_USB_START, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_stop(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICE_USB_STOP, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_status(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICE_USB_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_nice_usb_run_test_script(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICE_USB_RUN_TEST_SCRIPT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_script(ow_device* dev, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICEUSB_SCRIPT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_force_vidpid(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICEUSB_FORCE_VIDPID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_vid(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICEUSB_VID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_io_nice_usb_niceusb_pid(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_IO_NICE_USB_NICEUSB_PID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_emulate_card(ow_device* dev, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_SAVED_CARDS_EMULATE_CARD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_nfc_saved_cards_stop_emulation(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_NFC_SAVED_CARDS_STOP_EMULATION, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_display_functions_run_app_on_display(ow_device* dev, const char* filename)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, filename);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_DISPLAY_FUNCTIONS_RUN_APP_ON_DISPLAY, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_get_attribute(ow_device* dev, int32_t slot)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, slot);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_ON_GET_ATTRIBUTE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_bluetooth_le_on_set_attribute(ow_device* dev, int32_t slot, const char* value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, slot);
    ao = ow__pack_str(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_BLUETOOTH_LE_ON_SET_ATTRIBUTE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_http_get_to_sd(ow_device* dev, const char* url, const char* path)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_str(args, ao, url);
    ao = ow__pack_str(args, ao, path);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_HTTP_GET_TO_SD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_wifi_on_http_get_abort(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_WIFI_ON_HTTP_GET_ABORT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_quiet_threshold(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_QUIET_THRESHOLD, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_speaker_volume(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_SPEAKER_VOLUME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_recording_volume(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_RECORDING_VOLUME, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_record_len_sec(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_RECORD_LEN_SEC, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_sound_settings_system_sounds(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_SOUND_SETTINGS_SYSTEM_SOUNDS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_powered(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_POWERED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_batt_gt70(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_BATT_GT70, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_batt_gt30(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_BATT_GT30, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_brightness_batt_lt30(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BRIGHTNESS_BATT_LT30, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_battery_timeout(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_BATTERY_TIMEOUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_powered_timeout(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_POWERED_TIMEOUT, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_wake_on_sound(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_WAKE_ON_SOUND, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_wake_on_move(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_WAKE_ON_MOVE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_power_settings_auto_power_zones(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_POWER_SETTINGS_AUTO_POWER_ZONES, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_default_show(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_LIGHT_SHOW_SETTINGS_DEFAULT_SHOW, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_l_ed_strips_enabled(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_LIGHT_SHOW_SETTINGS_L_ED_STRIPS_ENABLED, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_light_show_settings_roku_led_control(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_LIGHT_SHOW_SETTINGS_ROKU_LED_CONTROL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_interface_settings_double_click_ms(ow_device* dev, int32_t value)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_INTERFACE_SETTINGS_DOUBLE_CLICK_MS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_hardware_settings_home_interface_settings_roku_gui_control(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_HARDWARE_SETTINGS_HOME_INTERFACE_SETTINGS_ROKU_GUI_CONTROL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_raw_bits(ow_device* dev, int32_t* modulation, int32_t* length, uint8_t* bits, size_t bits_cap, size_t* bits_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_RAW_BITS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (modulation) *modulation = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (length) *length = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    ow__take_bytes(ret, &ro, bits, (int)bits_cap, (int*)bits_len);
    return OW_OK;
}

ow_status ow_wireless_rfid_clear_stats(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_CLEAR_STATS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_get_status(ow_device* dev, int32_t* state, uint8_t* flags, int32_t* carrier_hz, int32_t* env_min, int32_t* env_max, int32_t* threshold, int32_t* frames, int32_t* tags)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_GET_STATUS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (state) *state = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (flags) *flags = ow__take_u8(ret, &ro); else ow__take_u8(ret, &ro);
    if (carrier_hz) *carrier_hz = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (env_min) *env_min = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (env_max) *env_max = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (threshold) *threshold = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (frames) *frames = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (tags) *tags = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_rfid_carrier_info(ow_device* dev, int32_t* carrier_hz, bool* psk_active, int32_t* psk_events, int32_t* poll_count, int32_t* clk_hz, bool* clock_ok, int32_t* env_samples, int32_t* overruns, int32_t* restarts, int32_t* psk_period)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_CARRIER_INFO, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (carrier_hz) *carrier_hz = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (psk_active) *psk_active = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (psk_events) *psk_events = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (poll_count) *poll_count = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (clk_hz) *clk_hz = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (clock_ok) *clock_ok = ow__take_u8(ret, &ro) != 0; else ow__take_u8(ret, &ro);
    if (env_samples) *env_samples = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (overruns) *overruns = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (restarts) *restarts = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (psk_period) *psk_period = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_rfid_clone_write(ow_device* dev)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_CLONE_WRITE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_clone_capture(ow_device* dev, int32_t timeout_ms)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, timeout_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_CLONE_CAPTURE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_enroll_id(ow_device* dev, const uint8_t* id, size_t id_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_bytes(args, ao, id, (uint32_t)id_len);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_ENROLL_ID, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_enable_reader(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_ENABLE_READER, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_stream_tags(ow_device* dev, int32_t enable)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, enable);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_STREAM_TAGS, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

ow_status ow_wireless_rfid_read_tag(ow_device* dev, int32_t timeout_ms, int32_t* format, int32_t* modulation, uint8_t* id, size_t id_cap, size_t* id_len)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, timeout_ms);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_READ_TAG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (format) *format = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (modulation) *modulation = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    ow__take_bytes(ret, &ro, id, (int)id_cap, (int*)id_len);
    return OW_OK;
}

ow_status ow_wireless_rfid_tune(ow_device* dev, int32_t param, int32_t value, int32_t* param_out, int32_t* value_out)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, param);
    ao = ow__pack_i32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_TUNE, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (param_out) *param_out = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (value_out) *value_out = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_wireless_rfid_write_tag(ow_device* dev, int32_t block, uint32_t value, int32_t* result, int32_t* block_out)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, block);
    ao = ow__pack_u32(args, ao, value);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_WIRELESS_RFID_WRITE_TAG, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    int ro = 0;
    if (result) *result = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    if (block_out) *block_out = ow__take_i32(ret, &ro); else ow__take_i32(ret, &ro);
    return OW_OK;
}

ow_status ow_gui_reinit_lcd_panel(ow_device* dev, int32_t mode)
{
    (void)dev;
    uint8_t args[512]; int ao = 0;
    ao = ow__pack_i32(args, ao, mode);
    uint8_t ret[512]; int rc;
    rc = ow_call(CMD_GUI_REINIT_LCD_PANEL, args, ao, ret, (int)sizeof ret);
    if (rc < 0) return OW_ERR_FAILED;
    return OW_OK;
}

