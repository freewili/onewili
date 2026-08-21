//! Watch FreeWili events on both ports for 10 seconds: opens the
//! text + binary (FTDI) ports, starts the GPIO stream, prints every
//! decoded event, stops the stream.
use onewili::OneWili;

fn main() -> Result<(), onewili::OwError> {
    let mut dev = OneWili::connect_binary()?;
    dev.io().gpio().stream_io(10)?;
    let deadline = std::time::Instant::now() + std::time::Duration::from_secs(10);
    let mut count = 0u32;
    while std::time::Instant::now() < deadline {
        match dev.poll_event()? {
            Some(ev) => {
                count += 1;
                println!("{ev:?}");
            }
            None => std::thread::sleep(std::time::Duration::from_millis(5)),
        }
    }
    dev.io().gpio().stream_io(0)?;
    println!("{count} events in 10 s");
    Ok(())
}
