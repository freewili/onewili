//! WASM transport - generated. Do not edit.
//! One host import `ow_call`; the firmware does all wire assembly/decoding.
use core::convert::TryInto;

#[derive(Debug, Clone, PartialEq)]
pub enum OwError { Failed(i32) }
impl core::fmt::Display for OwError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        match self { OwError::Failed(c) => write!(f, "ow_call failed ({c})") }
    }
}

#[link(wasm_import_module = "wiliwasm")]
extern "C" {
    #[link_name = "ow_call"]
    fn ow_call(cmd_index: i32, args: *const u8, args_len: i32,
               ret: *mut u8, ret_cap: i32) -> i32;
}

/// Pack helpers used by the generated menu methods.
pub struct Args(pub Vec<u8>);
impl Args {
    pub fn new() -> Self { Args(Vec::new()) }
    pub fn i32(&mut self, v: i32)  { self.0.extend_from_slice(&v.to_le_bytes()); }
    pub fn u32(&mut self, v: u32)  { self.0.extend_from_slice(&v.to_le_bytes()); }
    pub fn u8(&mut self, v: u8)    { self.0.push(v); }
    pub fn f32(&mut self, v: f32)  { self.0.extend_from_slice(&v.to_le_bytes()); }
    pub fn bytes(&mut self, b: &[u8]) {
        self.0.extend_from_slice(&(b.len() as u32).to_le_bytes());
        self.0.extend_from_slice(b);
    }
    pub fn str(&mut self, s: &str) { self.bytes(s.as_bytes()); }
}

/// Reader for the packed return blob.
pub struct Ret { buf: Vec<u8>, off: usize }
impl Ret {
    pub fn i32(&mut self) -> i32 { let v = i32::from_le_bytes(self.buf[self.off..self.off+4].try_into().unwrap()); self.off += 4; v }
    pub fn u32(&mut self) -> u32 { let v = u32::from_le_bytes(self.buf[self.off..self.off+4].try_into().unwrap()); self.off += 4; v }
    pub fn u8(&mut self) -> u8 { let v = self.buf[self.off]; self.off += 1; v }
    pub fn f32(&mut self) -> f32 { let v = f32::from_le_bytes(self.buf[self.off..self.off+4].try_into().unwrap()); self.off += 4; v }
    pub fn bytes(&mut self) -> Vec<u8> {
        let n = u32::from_le_bytes(self.buf[self.off..self.off+4].try_into().unwrap()) as usize;
        self.off += 4; let v = self.buf[self.off..self.off+n].to_vec(); self.off += n; v
    }
    pub fn string(&mut self) -> String { String::from_utf8_lossy(&self.bytes()).into_owned() }
}

/// The one entry point the generated methods call.
pub fn call(cmd_index: i32, args: &Args) -> Result<Ret, OwError> {
    let mut ret = vec![0u8; 512];
    let rc = unsafe {
        ow_call(cmd_index, args.0.as_ptr(), args.0.len() as i32,
                ret.as_mut_ptr(), ret.len() as i32)
    };
    if rc < 0 { return Err(OwError::Failed(rc)); }
    Ok(Ret { buf: ret, off: 0 })
}
