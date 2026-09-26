#pragma once
#include "fwcm0/console_client.h"
#include <condition_variable>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <memory>
#include <mutex>
#include "onewili.h"
namespace fwcm0 {
class OneWiliLink {
public:
  explicit OneWiliLink(ConsoleClient& client);
  ~OneWiliLink();
  ow_transport transport();
private:
  // The callback owns the receive buffer and detaches before link destruction.
  struct Rx { std::mutex m; std::condition_variable cv; std::deque<uint8_t> q; };
  static int c_write(void* ctx, const uint8_t* data, size_t len);
  static int c_read(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms);
  ConsoleClient& client_;
  std::shared_ptr<Rx> rx_ = std::make_shared<Rx>();
};
} // namespace fwcm0
