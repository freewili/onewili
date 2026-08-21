//! Toggle GPIO 25 once a second (default 10 times; pass a count argument).
use onewili::OneWili;

fn main() -> Result<(), onewili::OwError> {
    let count: u32 = std::env::args().nth(1).and_then(|a| a.parse().ok()).unwrap_or(10);
    let mut dev = OneWili::connect()?;
    for i in 1..=count {
        dev.io().gpio().set_io_toggle(25)?;
        println!("[{i}/{count}] toggled GPIO 25");
        if i < count {
            std::thread::sleep(std::time::Duration::from_secs(1));
        }
    }
    Ok(())
}
