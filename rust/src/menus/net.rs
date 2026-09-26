//! Network (TCP/IP) menu - generated from fwMenuNet. Do not edit.

use crate::encoding;
use crate::transport::{OwError, Transport};

pub struct Net<'a> {
    pub(crate) t: &'a mut Transport,
}

impl<'a> Net<'a> {
    /// Status. Prints one line of key=value TCP/IP stack status: lwip host up ncm_link ncm_mode ncm_ip ncm_mask ncm_gw ncm_mac ncm_dhcp t1s_link t1s_ip t1s_mask t1s_mac ncm_rx ncm_tx ncm_rxdrop ncm_txdrop t1s_rx t1s_tx t1s_rxdrop t1s_txdrop rx_nomem tcp_pcbs udp_pcbs tcp_echo udp_echo http mem_used mem_max pbuf_used pbuf_max bridge tcp_echo_bytes udp_sink echo_on http_on t1s_rxfilt. New keys are only ever APPENDED (host parsers key on names, never positions). lwip=0 means the stack is not compiled in; ncm_mode is static or dhcp, ncm_dhcp is off/init/discover/request/bound/renew/rebind/backoff/autoip/autoip-probe. Wire-parseable, append-only. Wire: `i\w\s`
    pub fn net_status(&mut self) -> Result<String, OwError> {
        let cmd = String::from("i\\w\\s");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let status = encoding::rest_str(&mut toks);
        Ok(status)
    }

    /// Link Status. Reports the USB network adapter (NCM) link as lwIP sees it, the addressing mode, the current IP, the DHCP hostname and whether the NCM<->T1S bridge (which takes lwIP off both wires) is on. Wire: `i\w\l`
    pub fn net_link_status(&mut self) -> Result<String, OwError> {
        let cmd = String::from("i\\w\\l");
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let info = encoding::rest_str(&mut toks);
        Ok(info)
    }

    /// DHCP Renew. Asks the DHCP client on the USB network adapter to renew its lease now. Fails when NCM Mode is static or DHCP is not running. Wire: `i\w\r`
    pub fn net_dhcp_renew(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\w\\r");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Ping. ICMP echo client: sends count (1..5, default 3) echo requests to the dotted-decimal IPv4 address one at a time with a 1 s timeout each and prints seq=N rtt_ms=X or seq=N timeout per request, then sent= recv= min/avg/max. Blocks the console for up to count seconds; the stack keeps being serviced meanwhile. Wire: `i\w\p`
    pub fn net_ping(&mut self, ip: &str, count: i32) -> Result<String, OwError> {
        let mut cmd = String::from("i\\w\\p");
        encoding::push_str(&mut cmd, ip);
        encoding::push_int(&mut cmd, count as i64);
        let resp = self.t.call(&cmd)?;
        let mut toks = resp.split_whitespace();
        let result = encoding::rest_str(&mut toks);
        Ok(result)
    }

    /// Clear Counters. Zeros the net rx/tx/drop counters, ring high-water marks, service counters and the lwIP memory max/error marks. Addresses, links and services are unaffected. Wire: `i\w\c`
    pub fn net_clear_counters(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\w\\c");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Apply. Re-pushes the settings below into the stack (every setting change already applies live; this is for scripts and after a rejected address). Fails when an address does not parse -- the previous configuration stays live and the settings are resynced to it. Wire: `i\w\a`
    pub fn net_apply(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\w\\a");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// NCM Mode. Addressing mode of the USB network adapter: static uses NCM IP/Netmask/Gateway; dhcp runs the DHCP client (hostname freewili-xxxx) and falls back to an AutoIP 169.254.x.x address after 3 unanswered discovers. Applies live and persists. Wire: `i\w\m`
    pub fn n_cm_mode(&mut self, value: i32) -> Result<(), OwError> {
        let mut cmd = String::from("i\\w\\m");
        encoding::push_int(&mut cmd, value as i64);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// NCM IP. Static IPv4 address of the USB network adapter (dotted decimal, default 10.55.0.2 -- the host tests expect this). Ignored while NCM Mode is dhcp. Rejected (previous kept) if it does not parse. Wire: `i\w\i`
    pub fn n_cmip(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\w\\i");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// NCM Netmask. Static netmask of the USB network adapter (dotted decimal, default 255.255.255.0). Ignored while NCM Mode is dhcp. Wire: `i\w\k`
    pub fn n_cm_netmask(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\w\\k");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// NCM Gateway. Static default gateway on the USB network adapter (dotted decimal, default 10.55.0.1 = the host). Ignored while NCM Mode is dhcp. Wire: `i\w\g`
    pub fn n_cm_gateway(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\w\\g");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// T1S IP. Static IPv4 address of the 10BASE-T1S port's own lwIP netif (dotted decimal, default 10.56.0.2; static only, no gateway, never the default route). Wire: `i\w\j`
    pub fn t1sip(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\w\\j");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// T1S Netmask. Static netmask of the 10BASE-T1S port's own netif (dotted decimal, default 255.255.255.0). Wire: `i\w\u`
    pub fn t1s_netmask(&mut self, value: &str) -> Result<(), OwError> {
        let mut cmd = String::from("i\\w\\u");
        encoding::push_str(&mut cmd, value);
        self.t.call(&cmd)?;
        Ok(())
    }

    /// Echo Servers. When on (default), the device runs the UDP echo server on port 5556, the UDP 5555 sink (swallows stray FWET test datagrams) and the TCP echo server on port 7 on every netif. Turning it off aborts live echo connections. Wire: `i\w\e`
    pub fn echo_servers(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\w\\e");
        self.t.call(&cmd)?;
        Ok(())
    }

    /// HTTP Server. When on (default), the device serves an HTML status page on port 80 (GET /) and the plain-text Status line (GET /status). Wire: `i\w\t`
    pub fn h_ttp_server(&mut self) -> Result<(), OwError> {
        let cmd = String::from("i\\w\\t");
        self.t.call(&cmd)?;
        Ok(())
    }
}
