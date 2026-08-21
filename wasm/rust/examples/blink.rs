//! Minimal WASM guest in Rust: toggle GPIO 25 and sleep.
#[link(wasm_import_module = "wiliwasm")]
extern "C" { #[link_name = "waitms"] fn waitms(ms: i32); }

#[no_mangle]
pub extern "C" fn _start() {
    if let Ok(mut dev) = onewili::OneWili::open() {
        for _ in 0..10 {
            // Regenerate against firmware exposing gpio().set_io_toggle().
            let _ = dev.gpio().set_io_toggle(25);
            unsafe { waitms(500); }
        }
    }
}
