#include "onewili_cm0.h"
#include <chrono>
#include <stdexcept>
namespace fwcm0 {
OneWiliLink::OneWiliLink(ConsoleClient& client) : client_(client) {
  if (!client_.connect(1000, true))
    throw std::runtime_error("MAIN did not complete OneWili mailbox handshake");
  auto rx = rx_;  // capture a shared_ptr copy, NOT `this`
  client_.on_console([rx](const std::vector<uint8_t>& body) {
    std::lock_guard<std::mutex> l(rx->m);
    rx->q.insert(rx->q.end(), body.begin(), body.end());
    rx->cv.notify_all();
  });
}
OneWiliLink::~OneWiliLink() {
  client_.on_console(nullptr);
  try { client_.set_stream(false); } catch (...) { }
}
ow_transport OneWiliLink::transport() {
  ow_transport t; t.ctx = this; t.write = &OneWiliLink::c_write; t.read = &OneWiliLink::c_read; return t;
}
int OneWiliLink::c_write(void* ctx, const uint8_t* data, size_t len) {
  auto* self = static_cast<OneWiliLink*>(ctx);
  self->client_.send_console(data, len);
  return static_cast<int>(len);
}
int OneWiliLink::c_read(void* ctx, uint8_t* buf, size_t cap, uint32_t timeout_ms) {
  auto* self = static_cast<OneWiliLink*>(ctx);
  Rx& rx = *self->rx_;
  std::unique_lock<std::mutex> l(rx.m);
  if (rx.q.empty())
    rx.cv.wait_for(l, std::chrono::milliseconds(timeout_ms), [&]{ return !rx.q.empty(); });
  size_t k = 0;
  while (k < cap && !rx.q.empty()) { buf[k++] = rx.q.front(); rx.q.pop_front(); }
  return static_cast<int>(k);
}
} // namespace fwcm0
