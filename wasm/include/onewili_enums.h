/* OneWili named enumerations (fwMenuEnums.h) - generated. Do not edit. */
#ifndef ONEWILI_ENUMS_H
#define ONEWILI_ENUMS_H
#ifdef __cplusplus
extern "C" {
#endif

/* owLEDManagerLEDMode (uint8_t) */
typedef enum ow_ow_led_manager_led_mode {
    OW_OW_LED_MANAGER_LED_MODE_SIMPLEVALUE = 0,
    OW_OW_LED_MANAGER_LED_MODE_FLASH = 1,
    OW_OW_LED_MANAGER_LED_MODE_PULSE = 2,
    OW_OW_LED_MANAGER_LED_MODE_FLASHFADE = 3,
    OW_OW_LED_MANAGER_LED_MODE_PULSEFADE = 4
} ow_ow_led_manager_led_mode;

/* IOVoltageSource (uint8_t) */
typedef enum ow_io_voltage_source {
    OW_IO_VOLTAGE_SOURCE_NONE = 0,
    OW_IO_VOLTAGE_SOURCE_V3_3 = 1,
    OW_IO_VOLTAGE_SOURCE_V5_0 = 2,
    OW_IO_VOLTAGE_SOURCE_EXT_PIN = 3,
    OW_IO_VOLTAGE_SOURCE_PROG_VOUT = 4
} ow_io_voltage_source;

/* dacWaveShapeMenu (uint8_t) */
typedef enum ow_dac_wave_shape_menu {
    OW_DAC_WAVE_SHAPE_MENU_OFF = 0,
    OW_DAC_WAVE_SHAPE_MENU_TRIANGLE = 1,
    OW_DAC_WAVE_SHAPE_MENU_SAWTOOTH = 2,
    OW_DAC_WAVE_SHAPE_MENU_INVSAWTOOTH = 3,
    OW_DAC_WAVE_SHAPE_MENU_SINE = 4
} ow_dac_wave_shape_menu;

/* dacWavePhase (uint8_t) */
typedef enum ow_dac_wave_phase {
    OW_DAC_WAVE_PHASE_DEG0 = 0,
    OW_DAC_WAVE_PHASE_DEG120 = 1,
    OW_DAC_WAVE_PHASE_DEG240 = 2,
    OW_DAC_WAVE_PHASE_DEG90 = 3
} ow_dac_wave_phase;

/* resetLineState (uint8_t) */
typedef enum ow_reset_line_state {
    OW_RESET_LINE_STATE_HOLD_IN_RESET = 0,
    OW_RESET_LINE_STATE_RELEASE = 1
} ow_reset_line_state;

/* owLEDLightShow (uint8_t) */
typedef enum ow_ow_led_light_show {
    OW_OW_LED_LIGHT_SHOW_MANUAL = 0,
    OW_OW_LED_LIGHT_SHOW_RAINBOW = 1,
    OW_OW_LED_LIGHT_SHOW_SNOWSTORM = 2,
    OW_OW_LED_LIGHT_SHOW_REDCHASE = 3,
    OW_OW_LED_LIGHT_SHOW_RAINBOWCHASE = 4,
    OW_OW_LED_LIGHT_SHOW_BLUECHASE = 5,
    OW_OW_LED_LIGHT_SHOW_GREENDOT = 6,
    OW_OW_LED_LIGHT_SHOW_BLUEDOT = 7,
    OW_OW_LED_LIGHT_SHOW_BLUESIN = 8,
    OW_OW_LED_LIGHT_SHOW_WHITEFADE = 9,
    OW_OW_LED_LIGHT_SHOW_BARGRAPH = 10,
    OW_OW_LED_LIGHT_SHOW_ZYLON = 11,
    OW_OW_LED_LIGHT_SHOW_AUDIO = 12,
    OW_OW_LED_LIGHT_SHOW_ACCEL = 13
} ow_ow_led_light_show;

/* owSerialLEDType (uint8_t) */
typedef enum ow_ow_serial_led_type {
    OW_OW_SERIAL_LED_TYPE_RGB = 0,
    OW_OW_SERIAL_LED_TYPE_RGBW = 1
} ow_ow_serial_led_type;

/* owScreenshotFileType (uint8_t) */
typedef enum ow_ow_screenshot_file_type {
    OW_OW_SCREENSHOT_FILE_TYPE_PNG = 0,
    OW_OW_SCREENSHOT_FILE_TYPE_FWI = 1
} ow_ow_screenshot_file_type;

/* owGUIButton (uint8_t) */
typedef enum ow_ow_gui_button {
    OW_OW_GUI_BUTTON_GRAY = 0,
    OW_OW_GUI_BUTTON_YELLOW = 1,
    OW_OW_GUI_BUTTON_GREEN = 2,
    OW_OW_GUI_BUTTON_BLUE = 3,
    OW_OW_GUI_BUTTON_RED = 4,
    OW_OW_GUI_BUTTON_UP = 33,
    OW_OW_GUI_BUTTON_DOWN = 34,
    OW_OW_GUI_BUTTON_LEFT = 35,
    OW_OW_GUI_BUTTON_RIGHT = 36,
    OW_OW_GUI_BUTTON_CENTER = 37,
    OW_OW_GUI_BUTTON_OK = 38,
    OW_OW_GUI_BUTTON_CANCEL = 39,
    OW_OW_GUI_BUTTON_HOME = 40,
    OW_OW_GUI_BUTTON_PAGE = 41
} ow_ow_gui_button;

/* owButtonPressType (uint8_t) */
typedef enum ow_ow_button_press_type {
    OW_OW_BUTTON_PRESS_TYPE_PRESS = 0,
    OW_OW_BUTTON_PRESS_TYPE_LONGPRESS = 1,
    OW_OW_BUTTON_PRESS_TYPE_PRESSANDSTAY = 2,
    OW_OW_BUTTON_PRESS_TYPE_RELEASE = 3
} ow_ow_button_press_type;

#ifdef __cplusplus
}
#endif
#endif /* ONEWILI_ENUMS_H */
