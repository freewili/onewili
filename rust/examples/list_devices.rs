//! List serial ports, flagging FreeWili devices (USB VID 0x093C).
use onewili::transport::FREEWILI_VID;

fn main() {
    match serialport::available_ports() {
        Err(e) => eprintln!("could not enumerate ports: {e}"),
        Ok(ports) => {
            for p in ports {
                match &p.port_type {
                    serialport::SerialPortType::UsbPort(info) => {
                        let mark = if info.vid == FREEWILI_VID { "  <-- FreeWili" } else { "" };
                        println!(
                            "{}  vid={:04X} pid={:04X} product={:?}{}",
                            p.port_name, info.vid, info.pid, info.product, mark
                        );
                    }
                    _ => println!("{}", p.port_name),
                }
            }
        }
    }
}
