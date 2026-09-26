//! Minimal WASM guest in Rust: echo CAN(FD) frames received on channel 0
//! back out with the arbitration id + 1. On-device scripts cannot see the
//! `[*can0 ...]` stream events, so they poll `receive_canfd` (which services
//! the controller FIFO itself); `waitms` yields to the firmware main loop.
#![no_main]
#[link(wasm_import_module = "wiliwasm")]
extern "C" { #[link_name = "waitms"] fn waitms(ms: i32); }

#[no_mangle]
pub extern "C" fn _start() {
    if let Ok(mut dev) = onewili::OneWili::open() {
        if dev.io().canfd().enable_canfd_receive_queue(0, 1).is_err() { return; }
        loop {
            match dev.io().canfd().receive_canfd(0) {
                Ok((true, _queued, _dropped, arb_id, xtd_id, can_fd, _timestamp_us, _dlc, data)) => {
                    let _ = dev.io().canfd().write_canfd(0, arb_id + 1, can_fd, xtd_id, &data);
                }
                _ => unsafe { waitms(5) },
            }
        }
    }
}
