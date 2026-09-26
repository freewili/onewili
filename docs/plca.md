# PLCA Settings

`dev.io.t1s.plca` - wire path `i\r\p` - generated from `fwMenuPLCA`.

## p_lca_enabled

PLCAEnabled. When on, the PHY runs PLCA (collision-free round-robin transmit opportunities; the node with Local ID 0 coordinates the cycle). When off, the PHY falls back to CSMA/CD. Applied live to a running PHY

Wire command: `i\r\p\a`

Returns: none (Ok/Err only)

```python
dev.io.t1s.plca.p_lca_enabled() -> Result
```
```c
ow_status ow_io_t1s_plca_p_lca_enabled(ow_device* dev);
```
```rust
dev.io().t1s().plca().p_lca_enabled() -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.plca.p_lca_enabled()   # check dev.ok
```

## local_id

Local ID. This node's PLCA ID (0..254). ID 0 is the cycle coordinator -- exactly one node on the segment must be 0. Applied live to a running PHY

Wire command: `i\r\p\l`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.plca.local_id(value: int) -> Result
```
```c
ow_status ow_io_t1s_plca_local_id(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().plca().local_id(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.plca.local_id(value)   # check dev.ok
```

## node_count

Node Count. Number of transmit opportunities in each PLCA cycle (1..255); only meaningful on the coordinator (Local ID 0). Applied live to a running PHY

Wire command: `i\r\p\n`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.plca.node_count(value: int) -> Result
```
```c
ow_status ow_io_t1s_plca_node_count(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().plca().node_count(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.plca.node_count(value)   # check dev.ok
```

## t_o_timer

TO Timer. PLCA transmit-opportunity timer in bit times (1..255, silicon default 32). Written directly to the PHY's PLCA_TOTMR register when it differs from 32. Applied live to a running PHY

Wire command: `i\r\p\t`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.plca.t_o_timer(value: int) -> Result
```
```c
ow_status ow_io_t1s_plca_t_o_timer(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().plca().t_o_timer(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.plca.t_o_timer(value)   # check dev.ok
```

## burst_max

Burst Max. Maximum extra packets this node may send in one transmit opportunity (0..255, 0 = burst off). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply

Wire command: `i\r\p\m`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.plca.burst_max(value: int) -> Result
```
```c
ow_status ow_io_t1s_plca_burst_max(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().plca().burst_max(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.plca.burst_max(value)   # check dev.ok
```

## burst_timer

Burst Timer. Idle time in bit times the PHY waits between burst packets before giving up the transmit opportunity (1..255). Takes effect at the next PHY (re)init -- use Reinit PHY (i\r\i) to apply

Wire command: `i\r\p\b`

| Arg | Wire type |
|---|---|
| value |  |

Returns: none (Ok/Err only)

```python
dev.io.t1s.plca.burst_timer(value: int) -> Result
```
```c
ow_status ow_io_t1s_plca_burst_timer(ow_device* dev, int32_t value);
```
```rust
dev.io().t1s().plca().burst_timer(value: i32) -> Result<(), OwError>
```

The C and Rust signatures above are also the WASM guest signatures - the device API surface is identical; only the transport differs (`ow_open_wasm(&dev)` in C, `OneWili::open()` in Rust).
```rthon
dev.io.t1s.plca.burst_timer(value)   # check dev.ok
```
