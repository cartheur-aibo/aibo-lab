#ifndef APERIOS_HOST_OPENR_OSUBJECT_H
#define APERIOS_HOST_OPENR_OSUBJECT_H

#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

class OSubject {
public:
    void Enable() { enabled_ = true; }
    void Disable() { enabled_ = false; }
    bool IsEnabled() const { return enabled_; }

    template <typename T>
    void SetData(const T* value, std::size_t size) {
        const auto* begin = reinterpret_cast<const std::uint8_t*>(value);
        buffer_.assign(begin, begin + size);
    }

    template <typename T>
    void SetData(const T& value) {
        SetData(&value, sizeof(value));
    }

    bool HasBufferedData() const { return !buffer_.empty(); }
    std::size_t BufferedSize() const { return buffer_.size(); }
    void ClearBuffer() { buffer_.clear(); }

private:
    bool enabled_ = false;
    std::vector<std::uint8_t> buffer_;
};

#endif
