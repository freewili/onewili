//! Toggle GPIO 25 on a FreeWili (auto-discovers the device by USB VID).
use onewili::OneWili;

fn main() -> Result<(), onewili::OwError> {
    let mut dev = OneWili::connect()?;
    dev.io().gpio().set_io_toggle(25)?;
    println!("toggled GPIO 25");
    Ok(())
}
