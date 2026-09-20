//! System Functions menu - generated from fwMenuSystem. Do not edit.
//! Each method packs its args into a Vec<u8> and calls the host
//! import `ow_call(cmd_index, args, args_len, ret, ret_cap)` via
//! crate::transport::call - the firmware assembles/decodes the
//! wire command natively; there is no encoding/framing here.

use crate::transport::OwError;

pub struct System<'a> {
    #[allow(dead_code)]
    pub(crate) t: &'a mut (),
}

impl<'a> System<'a> {
    /// Stream Battery Info. Enables or disables streaming of battery info to the host.. Wire: `h\a\o`
    pub fn enable_battery_stream(&mut self, enable: i32) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let _r = crate::transport::call(291 /* CMD_HARDWARE_SYSTEM_ENABLE_BATTERY_STREAM */, &a)?;
        Ok(())
    }

    /// Read OTP Info. Reads bytes from the fused OTP identity blob (bl_otp_info v3). An unprovisioned device reads all zeros. Read in chunks of 256 bytes or less.. Wire: `h\a\b`
    pub fn read_otp_info(&mut self, offset: i32, length: i32) -> Result<Vec<u8>, OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(offset);
        a.i32(length);
        let mut _r = crate::transport::call(393 /* CMD_HARDWARE_SYSTEM_READ_OTP_INFO */, &a)?;
        Ok(_r.bytes())
    }

    /// Boot UF2. Reboots into the SBL bootloader, which chain-loads the named RAM-app UF2 from the SD card /update directory (card root as fallback). No response is sent on success — the device resets.. Wire: `h\a\u`
    pub fn boot_uf2(&mut self, filename: &str) -> Result<(), OwError> {
        let mut a = crate::transport::Args::new();
        a.str(filename);
        let _r = crate::transport::call(467 /* CMD_HARDWARE_SYSTEM_BOOT_UF2 */, &a)?;
        Ok(())
    }

    /// Device State. Report the device state for host sync: SD card host (none|main|usb), event host-streaming gate (0|1), active-stream mask (hex, bit index = event id), clk_sys in Hz. More space-separated fields may be appended later.. Wire: `h\a\g`
    pub fn device_state(&mut self) -> Result<(String, bool, String, i32), OwError> {
        let a = crate::transport::Args::new();
        let mut _r = crate::transport::call(437 /* CMD_HARDWARE_SYSTEM_DEVICE_STATE */, &a)?;
        Ok((_r.string(), _r.u8() != 0, _r.string(), _r.i32()))
    }

    /// Event Host Streaming. Enables or disables streaming of events to the host. When disabled, stream-class events are suppressed at the host output; protocol events still flow. Same gate as control bytes 0x05 (off) and 0x06 (on).. Wire: `h\a\e`
    pub fn event_host_streaming(&mut self, enable: i32) -> Result<bool, OwError> {
        let mut a = crate::transport::Args::new();
        a.i32(enable);
        let mut _r = crate::transport::call(438 /* CMD_HARDWARE_SYSTEM_EVENT_HOST_STREAMING */, &a)?;
        Ok(_r.u8() != 0)
    }
}
