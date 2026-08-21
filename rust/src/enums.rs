//! Named enumerations (fwMenuEnums.h) - generated. Do not edit.
//! Lenient newtypes: unknown values pass through untouched.

/// owLEDManagerLEDMode (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct owLEDManagerLEDMode(pub i64);

impl owLEDManagerLEDMode {
    pub const SIMPLEVALUE: owLEDManagerLEDMode = owLEDManagerLEDMode(0);
    pub const FLASH: owLEDManagerLEDMode = owLEDManagerLEDMode(1);
    pub const PULSE: owLEDManagerLEDMode = owLEDManagerLEDMode(2);
    pub const FLASHFADE: owLEDManagerLEDMode = owLEDManagerLEDMode(3);
    pub const PULSEFADE: owLEDManagerLEDMode = owLEDManagerLEDMode(4);
}

impl std::fmt::Debug for owLEDManagerLEDMode {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "owLEDManagerLEDMode::SIMPLEVALUE"),
            1 => write!(f, "owLEDManagerLEDMode::FLASH"),
            2 => write!(f, "owLEDManagerLEDMode::PULSE"),
            3 => write!(f, "owLEDManagerLEDMode::FLASHFADE"),
            4 => write!(f, "owLEDManagerLEDMode::PULSEFADE"),
            v => write!(f, "owLEDManagerLEDMode({})", v),
        }
    }
}

/// IOVoltageSource (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct IOVoltageSource(pub i64);

impl IOVoltageSource {
    pub const NONE: IOVoltageSource = IOVoltageSource(0);
    pub const V3_3: IOVoltageSource = IOVoltageSource(1);
    pub const V5_0: IOVoltageSource = IOVoltageSource(2);
    pub const EXT_PIN: IOVoltageSource = IOVoltageSource(3);
    pub const PROG_VOUT: IOVoltageSource = IOVoltageSource(4);
}

impl std::fmt::Debug for IOVoltageSource {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "IOVoltageSource::NONE"),
            1 => write!(f, "IOVoltageSource::V3_3"),
            2 => write!(f, "IOVoltageSource::V5_0"),
            3 => write!(f, "IOVoltageSource::EXT_PIN"),
            4 => write!(f, "IOVoltageSource::PROG_VOUT"),
            v => write!(f, "IOVoltageSource({})", v),
        }
    }
}

/// dacWaveShapeMenu (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct dacWaveShapeMenu(pub i64);

impl dacWaveShapeMenu {
    pub const OFF: dacWaveShapeMenu = dacWaveShapeMenu(0);
    pub const TRIANGLE: dacWaveShapeMenu = dacWaveShapeMenu(1);
    pub const SAWTOOTH: dacWaveShapeMenu = dacWaveShapeMenu(2);
    pub const INVSAWTOOTH: dacWaveShapeMenu = dacWaveShapeMenu(3);
    pub const SINE: dacWaveShapeMenu = dacWaveShapeMenu(4);
}

impl std::fmt::Debug for dacWaveShapeMenu {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "dacWaveShapeMenu::OFF"),
            1 => write!(f, "dacWaveShapeMenu::TRIANGLE"),
            2 => write!(f, "dacWaveShapeMenu::SAWTOOTH"),
            3 => write!(f, "dacWaveShapeMenu::INVSAWTOOTH"),
            4 => write!(f, "dacWaveShapeMenu::SINE"),
            v => write!(f, "dacWaveShapeMenu({})", v),
        }
    }
}

/// dacWavePhase (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct dacWavePhase(pub i64);

impl dacWavePhase {
    pub const DEG0: dacWavePhase = dacWavePhase(0);
    pub const DEG120: dacWavePhase = dacWavePhase(1);
    pub const DEG240: dacWavePhase = dacWavePhase(2);
    pub const DEG90: dacWavePhase = dacWavePhase(3);
}

impl std::fmt::Debug for dacWavePhase {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "dacWavePhase::DEG0"),
            1 => write!(f, "dacWavePhase::DEG120"),
            2 => write!(f, "dacWavePhase::DEG240"),
            3 => write!(f, "dacWavePhase::DEG90"),
            v => write!(f, "dacWavePhase({})", v),
        }
    }
}

/// resetLineState (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct resetLineState(pub i64);

impl resetLineState {
    pub const HOLD_IN_RESET: resetLineState = resetLineState(0);
    pub const RELEASE: resetLineState = resetLineState(1);
}

impl std::fmt::Debug for resetLineState {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "resetLineState::HOLD_IN_RESET"),
            1 => write!(f, "resetLineState::RELEASE"),
            v => write!(f, "resetLineState({})", v),
        }
    }
}

/// owLEDLightShow (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct owLEDLightShow(pub i64);

impl owLEDLightShow {
    pub const MANUAL: owLEDLightShow = owLEDLightShow(0);
    pub const RAINBOW: owLEDLightShow = owLEDLightShow(1);
    pub const SNOWSTORM: owLEDLightShow = owLEDLightShow(2);
    pub const REDCHASE: owLEDLightShow = owLEDLightShow(3);
    pub const RAINBOWCHASE: owLEDLightShow = owLEDLightShow(4);
    pub const BLUECHASE: owLEDLightShow = owLEDLightShow(5);
    pub const GREENDOT: owLEDLightShow = owLEDLightShow(6);
    pub const BLUEDOT: owLEDLightShow = owLEDLightShow(7);
    pub const BLUESIN: owLEDLightShow = owLEDLightShow(8);
    pub const WHITEFADE: owLEDLightShow = owLEDLightShow(9);
    pub const BARGRAPH: owLEDLightShow = owLEDLightShow(10);
    pub const ZYLON: owLEDLightShow = owLEDLightShow(11);
    pub const AUDIO: owLEDLightShow = owLEDLightShow(12);
    pub const ACCEL: owLEDLightShow = owLEDLightShow(13);
}

impl std::fmt::Debug for owLEDLightShow {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "owLEDLightShow::MANUAL"),
            1 => write!(f, "owLEDLightShow::RAINBOW"),
            2 => write!(f, "owLEDLightShow::SNOWSTORM"),
            3 => write!(f, "owLEDLightShow::REDCHASE"),
            4 => write!(f, "owLEDLightShow::RAINBOWCHASE"),
            5 => write!(f, "owLEDLightShow::BLUECHASE"),
            6 => write!(f, "owLEDLightShow::GREENDOT"),
            7 => write!(f, "owLEDLightShow::BLUEDOT"),
            8 => write!(f, "owLEDLightShow::BLUESIN"),
            9 => write!(f, "owLEDLightShow::WHITEFADE"),
            10 => write!(f, "owLEDLightShow::BARGRAPH"),
            11 => write!(f, "owLEDLightShow::ZYLON"),
            12 => write!(f, "owLEDLightShow::AUDIO"),
            13 => write!(f, "owLEDLightShow::ACCEL"),
            v => write!(f, "owLEDLightShow({})", v),
        }
    }
}

/// owSerialLEDType (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct owSerialLEDType(pub i64);

impl owSerialLEDType {
    pub const RGB: owSerialLEDType = owSerialLEDType(0);
    pub const RGBW: owSerialLEDType = owSerialLEDType(1);
}

impl std::fmt::Debug for owSerialLEDType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "owSerialLEDType::RGB"),
            1 => write!(f, "owSerialLEDType::RGBW"),
            v => write!(f, "owSerialLEDType({})", v),
        }
    }
}

/// owScreenshotFileType (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct owScreenshotFileType(pub i64);

impl owScreenshotFileType {
    pub const PNG: owScreenshotFileType = owScreenshotFileType(0);
    pub const FWI: owScreenshotFileType = owScreenshotFileType(1);
}

impl std::fmt::Debug for owScreenshotFileType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "owScreenshotFileType::PNG"),
            1 => write!(f, "owScreenshotFileType::FWI"),
            v => write!(f, "owScreenshotFileType({})", v),
        }
    }
}

/// owGUIButton (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct owGUIButton(pub i64);

impl owGUIButton {
    pub const GRAY: owGUIButton = owGUIButton(0);
    pub const YELLOW: owGUIButton = owGUIButton(1);
    pub const GREEN: owGUIButton = owGUIButton(2);
    pub const BLUE: owGUIButton = owGUIButton(3);
    pub const RED: owGUIButton = owGUIButton(4);
    pub const UP: owGUIButton = owGUIButton(33);
    pub const DOWN: owGUIButton = owGUIButton(34);
    pub const LEFT: owGUIButton = owGUIButton(35);
    pub const RIGHT: owGUIButton = owGUIButton(36);
    pub const CENTER: owGUIButton = owGUIButton(37);
    pub const OK: owGUIButton = owGUIButton(38);
    pub const CANCEL: owGUIButton = owGUIButton(39);
    pub const HOME: owGUIButton = owGUIButton(40);
    pub const PAGE: owGUIButton = owGUIButton(41);
}

impl std::fmt::Debug for owGUIButton {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "owGUIButton::GRAY"),
            1 => write!(f, "owGUIButton::YELLOW"),
            2 => write!(f, "owGUIButton::GREEN"),
            3 => write!(f, "owGUIButton::BLUE"),
            4 => write!(f, "owGUIButton::RED"),
            33 => write!(f, "owGUIButton::UP"),
            34 => write!(f, "owGUIButton::DOWN"),
            35 => write!(f, "owGUIButton::LEFT"),
            36 => write!(f, "owGUIButton::RIGHT"),
            37 => write!(f, "owGUIButton::CENTER"),
            38 => write!(f, "owGUIButton::OK"),
            39 => write!(f, "owGUIButton::CANCEL"),
            40 => write!(f, "owGUIButton::HOME"),
            41 => write!(f, "owGUIButton::PAGE"),
            v => write!(f, "owGUIButton({})", v),
        }
    }
}

/// owButtonPressType (uint8_t).
#[derive(Clone, Copy, PartialEq, Eq)]
pub struct owButtonPressType(pub i64);

impl owButtonPressType {
    pub const PRESS: owButtonPressType = owButtonPressType(0);
    pub const LONGPRESS: owButtonPressType = owButtonPressType(1);
    pub const PRESSANDSTAY: owButtonPressType = owButtonPressType(2);
    pub const RELEASE: owButtonPressType = owButtonPressType(3);
}

impl std::fmt::Debug for owButtonPressType {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.0 {
            0 => write!(f, "owButtonPressType::PRESS"),
            1 => write!(f, "owButtonPressType::LONGPRESS"),
            2 => write!(f, "owButtonPressType::PRESSANDSTAY"),
            3 => write!(f, "owButtonPressType::RELEASE"),
            v => write!(f, "owButtonPressType({})", v),
        }
    }
}
