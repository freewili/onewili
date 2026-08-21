//! Response-frame parsing: `[<path> <hexTsNs> <seq> <response...> <ok>]`.

pub struct ResponseFrame {
    pub path: String,
    pub response: String,
    pub success: bool,
}

pub fn is_event(line: &str) -> bool {
    line.starts_with("[*")
}

pub fn is_frame(line: &str) -> bool {
    let b = line.as_bytes();
    b.len() > 2 && b[0] == b'[' && (b[1].is_ascii_alphabetic() || b[1] == b'?')
}

pub fn parse(line: &str) -> Option<ResponseFrame> {
    let body = line.strip_prefix('[')?.strip_suffix(']')?;
    let tokens: Vec<&str> = body.split(' ').collect();
    if tokens.len() < 4 {
        return None;
    }
    Some(ResponseFrame {
        path: tokens[0].to_string(),
        response: tokens[3..tokens.len() - 1].join(" "),
        success: tokens[tokens.len() - 1] == "1",
    })
}
