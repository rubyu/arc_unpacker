#pragma once

#include <array>
#include <memory>
#include "types.h"

namespace au {
namespace fmt {
namespace glib {
namespace glib2  {

    struct Decoder final
    {
        std::array<size_t, 4> src_permutation;
        std::array<size_t, 4> dst_permutation;
        std::function<u8(u8, size_t)> func1;
        std::function<u8(u8, size_t)> func2;
    };

    class Plugin
    {
    public:
        virtual ~Plugin();
        virtual std::unique_ptr<Decoder> create_header_decoder() const = 0;
        virtual std::unique_ptr<Decoder> create_decoder(
            const std::array<u32, 4> &keys) const = 0;
    };

} } } }
