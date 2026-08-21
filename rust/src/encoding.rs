//! Argument encoders / response decoders for OneWili wire types.

use crate::transport::OwError;

pub fn push_int(cmd: &mut String, v: i64) {
    cmd.push(' ');
    cmd.push_str(&v.to_string());
}

pub fn push_hex(cmd: &mut String, v: u64, width: usize) {
    cmd.push(' ');
    cmd.push_str(&format!("{v:0width$X}"));
}

pub fn push_bytes(cmd: &mut String, data: &[u8]) {
    for b in data {
        cmd.push_str(&format!(" {b:02X}"));
    }
}

pub fn push_bool(cmd: &mut String, v: bool) {
    cmd.push_str(if v { " 1" } else { " 0" });
}

pub fn push_float(cmd: &mut String, v: f64) {
    cmd.push(' ');
    cmd.push_str(&v.to_string());
}

pub fn push_str(cmd: &mut String, v: &str) {
    cmd.push(' ');
    cmd.push_str(v);
}

fn next<'a>(toks: &mut impl Iterator<Item = &'a str>) -> Result<&'a str, OwError> {
    toks.next()
        .ok_or_else(|| OwError::Protocol("missing response token".into()))
}

pub fn tok_int<'a>(toks: &mut impl Iterator<Item = &'a str>) -> Result<i64, OwError> {
    let t = next(toks)?;
    t.parse::<i64>()
        .map_err(|_| OwError::Protocol(format!("bad int token {t:?}")))
}

pub fn tok_hex<'a>(toks: &mut impl Iterator<Item = &'a str>) -> Result<u64, OwError> {
    let t = next(toks)?;
    u64::from_str_radix(t, 16).map_err(|_| OwError::Protocol(format!("bad hex token {t:?}")))
}

pub fn tok_bool<'a>(toks: &mut impl Iterator<Item = &'a str>) -> Result<bool, OwError> {
    Ok(next(toks)? == "1")
}

pub fn tok_float<'a>(toks: &mut impl Iterator<Item = &'a str>) -> Result<f64, OwError> {
    let t = next(toks)?;
    t.parse::<f64>()
        .map_err(|_| OwError::Protocol(format!("bad float token {t:?}")))
}

pub fn rest_bytes<'a>(toks: &mut impl Iterator<Item = &'a str>) -> Result<Vec<u8>, OwError> {
    let mut out = Vec::new();
    for t in toks {
        let v = u8::from_str_radix(t, 16)
            .map_err(|_| OwError::Protocol(format!("bad byte token {t:?}")))?;
        out.push(v);
    }
    Ok(out)
}

pub fn rest_str<'a>(toks: &mut impl Iterator<Item = &'a str>) -> String {
    toks.collect::<Vec<_>>().join(" ")
}
