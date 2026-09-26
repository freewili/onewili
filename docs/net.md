# Network (TCP/IP)

`dev.io.net` - wire path `i\w` - generated from `fwMenuNet`.

## net_status

Status. Prints one line of key=value TCP/IP stack status: lwip host up ncm_link ncm_mode ncm_ip ncm_mask ncm_gw ncm_mac ncm_dhcp t1s_link t1s_ip t1s_mask t1s_mac ncm_rx ncm_tx ncm_rxdrop ncm_txdrop t1s_rx t1s_tx t1s_rxdrop t1s_txdrop rx_nomem tcp_pcbs udp_pcbs tcp_echo udp_echo http mem_used mem_max pbuf_used pbuf_max bridge tcp_echo_bytes udp_sink echo_on http_on t1s_rxfilt. New keys are only ever APPENDED (host parsers key on names, never positions). lwip=0 means the stack is not compiled in; ncm_mode is static or dhcp, ncm_dhcp is off/init/discover/request/bound/renew/rebind/backoff/autoip/autoip-probe. Wire-parseable, append-only

Wire command: `i\w\s`

Returns: status (string)

```python
dev.io.net.net_status() -> Result
```
```c
ow_status ow_io_net_net_status(ow_device* dev, char* status, size_t status_cap);
```
```rust
dev.io().net().net_status() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.net_status()   # returns value; check dev.ok
```

## net_link_status

Link Status. Reports the USB network adapter (NCM) link as lwIP sees it, the addressing mode, the current IP, the DHCP hostname and whether the NCM<->T1S bridge (which takes lwIP off both wires) is on

Wire command: `i\w\l`

Returns: info (string)

```python
dev.io.net.net_link_status() -> Result
```
```c
ow_status ow_io_net_net_link_status(ow_device* dev, char* info, size_t info_cap);
```
```rust
dev.io().net().net_link_status() -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.net_link_status()   # returns value; check dev.ok
```

## net_dhcp_renew

DHCP Renew. Asks the DHCP client on the USB network adapter to renew its lease now. Fails when NCM Mode is static or DHCP is not running

Wire command: `i\w\r`

Returns: none (Ok/Err only)

```python
dev.io.net.net_dhcp_renew() -> Result
```
```c
ow_status ow_io_net_net_dhcp_renew(ow_device* dev);
```
```rust
dev.io().net().net_dhcp_renew() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.net_dhcp_renew()   # check dev.ok
```

## net_ping

Ping. ICMP echo client: sends count (1..5, default 3) echo requests to the dotted-decimal IPv4 address one at a time with a 1 s timeout each and prints seq=N rtt_ms=X or seq=N timeout per request, then sent= recv= min/avg/max. Blocks the console for up to count seconds; the stack keeps being serviced meanwhile

Wire command: `i\w\p`

| Arg | Wire type |
|---|---|
| ip | string |
| count | dec |

Returns: result (string)

```python
dev.io.net.net_ping(ip: str, count: int) -> Result
```
```c
ow_status ow_io_net_net_ping(ow_device* dev, const char* ip, int32_t count, char* result, size_t result_cap);
```
```rust
dev.io().net().net_ping(ip: &str, count: i32) -> Result<String, OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.net_ping(ip, count)   # returns value; check dev.ok
```

## net_clear_counters

Clear Counters. Zeros the net rx/tx/drop counters, ring high-water marks, service counters and the lwIP memory max/error marks. Addresses, links and services are unaffected

Wire command: `i\w\c`

Returns: none (Ok/Err only)

```python
dev.io.net.net_clear_counters() -> Result
```
```c
ow_status ow_io_net_net_clear_counters(ow_device* dev);
```
```rust
dev.io().net().net_clear_counters() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.net_clear_counters()   # check dev.ok
```

## net_apply

Apply. Re-pushes the settings below into the stack (every setting change already applies live; this is for scripts and after a rejected address). Fails when an address does not parse -- the previous configuration stays live and the settings are resynced to it

Wire command: `i\w\a`

Returns: none (Ok/Err only)

```python
dev.io.net.net_apply() -> Result
```
```c
ow_status ow_io_net_net_apply(ow_device* dev);
```
```rust
dev.io().net().net_apply() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.net_apply()   # check dev.ok
```

## n_cm_mode

NCM Mode. Addressing mode of the USB network adapter: static uses NCM IP/Netmask/Gateway; dhcp runs the DHCP client (hostname freewili-xxxx) and falls back to an AutoIP 169.254.x.x address after 3 unanswered discovers. Applies live and persists

Wire command: `i\w\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.net.n_cm_mode(value: int) -> Result
```
```c
ow_status ow_io_net_n_cm_mode(ow_device* dev, int32_t value);
```
```rust
dev.io().net().n_cm_mode(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.n_cm_mode(value)   # check dev.ok
```

## n_cmip

NCM IP. Static IPv4 address of the USB network adapter (dotted decimal, default 10.55.0.2 -- the host tests expect this). Ignored while NCM Mode is dhcp. Rejected (previous kept) if it does not parse

Wire command: `i\w\i`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.net.n_cmip(value: str) -> Result
```
```c
ow_status ow_io_net_n_cmip(ow_device* dev, const char* value);
```
```rust
dev.io().net().n_cmip(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.n_cmip(value)   # check dev.ok
```

## n_cm_netmask

NCM Netmask. Static netmask of the USB network adapter (dotted decimal, default 255.255.255.0). Ignored while NCM Mode is dhcp

Wire command: `i\w\k`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.net.n_cm_netmask(value: str) -> Result
```
```c
ow_status ow_io_net_n_cm_netmask(ow_device* dev, const char* value);
```
```rust
dev.io().net().n_cm_netmask(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.n_cm_netmask(value)   # check dev.ok
```

## n_cm_gateway

NCM Gateway. Static default gateway on the USB network adapter (dotted decimal, default 10.55.0.1 = the host). Ignored while NCM Mode is dhcp

Wire command: `i\w\g`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.net.n_cm_gateway(value: str) -> Result
```
```c
ow_status ow_io_net_n_cm_gateway(ow_device* dev, const char* value);
```
```rust
dev.io().net().n_cm_gateway(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.n_cm_gateway(value)   # check dev.ok
```

## t1sip

T1S IP. Static IPv4 address of the 10BASE-T1S port's own lwIP netif (dotted decimal, default 10.56.0.2; static only, no gateway, never the default route)

Wire command: `i\w\j`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.net.t1sip(value: str) -> Result
```
```c
ow_status ow_io_net_t1sip(ow_device* dev, const char* value);
```
```rust
dev.io().net().t1sip(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.t1sip(value)   # check dev.ok
```

## t1s_netmask

T1S Netmask. Static netmask of the 10BASE-T1S port's own netif (dotted decimal, default 255.255.255.0)

Wire command: `i\w\u`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.net.t1s_netmask(value: str) -> Result
```
```c
ow_status ow_io_net_t1s_netmask(ow_device* dev, const char* value);
```
```rust
dev.io().net().t1s_netmask(value: &str) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.t1s_netmask(value)   # check dev.ok
```

## echo_servers

Echo Servers. When on (default), the device runs the UDP echo server on port 5556, the UDP 5555 sink (swallows stray FWET test datagrams) and the TCP echo server on port 7 on every netif. Turning it off aborts live echo connections

Wire command: `i\w\e`

Returns: none (Ok/Err only)

```python
dev.io.net.echo_servers() -> Result
```
```c
ow_status ow_io_net_echo_servers(ow_device* dev);
```
```rust
dev.io().net().echo_servers() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.echo_servers()   # check dev.ok
```

## h_ttp_server

HTTP Server. When on (default), the device serves an HTML status page on port 80 (GET /) and the plain-text Status line (GET /status)

Wire command: `i\w\t`

Returns: none (Ok/Err only)

```python
dev.io.net.h_ttp_server() -> Result
```
```c
ow_status ow_io_net_h_ttp_server(ow_device* dev);
```
```rust
dev.io().net().h_ttp_server() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.net.h_ttp_server()   # check dev.ok
```
