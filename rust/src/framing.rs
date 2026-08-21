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

/// Does `line` end with the closing " <ok>]" token? Judged on the whole token
/// and never on a bare ']', because a payload can end a line with a ']' of its
/// own and would otherwise close the frame early.
pub fn is_frame_closed(line: &str) -> bool {
    let b = line.as_bytes();
    b.len() >= 3
        && b[b.len() - 1] == b']'
        && (b[b.len() - 2] == b'0' || b[b.len() - 2] == b'1')
        && b[b.len() - 3].is_ascii_whitespace()
}

/// Rejoin a frame the firmware split across physical lines by printing a
/// newline into its payload. Newlines inside the payload survive; the one
/// abutting the closing token is a line terminator, not payload, so it
/// collapses into that token's separating space.
pub fn join_frame_lines(pieces: &[String]) -> String {
    let mut joined = pieces.join("\n");
    if is_frame_closed(&joined) {
        let tail = joined.split_off(joined.len() - 2);
        while joined.ends_with(char::is_whitespace) {
            joined.pop();
        }
        joined.push(' ');
        joined.push_str(&tail);
    }
    joined
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
