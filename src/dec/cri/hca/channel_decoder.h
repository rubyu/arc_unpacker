#pragma once

#include "dec/cri/hca/ath_table.h"
#include "io/base_bit_stream.h"

namespace au {
namespace dec {
namespace cri {
namespace hca {

    class ChannelDecoder final
    {
    public:
        ChannelDecoder(const int type, const int idx, const int count);

        void decode1(
            io::BaseBitStream &bit_stream,
            const unsigned int a,
            const int b,
            const AthTable &ath_table);

        void decode2(io::BaseBitStream &bit_stream);

        void decode3(
            const unsigned int a,
            const unsigned int b,
            const unsigned int c,
            const unsigned int d);

        void decode4(
            const int index,
            const unsigned int a,
            const unsigned int b,
            const unsigned int c,
            ChannelDecoder &next_decoder);

        void decode5(const int index);

        f32 wave[8][128];

    private:
        int type;
        unsigned int count;
        u8 scale[128];
        u8 value[128];
        u8 value2[8];
        u8 *value3;
        f32 block[128];
        f32 base[128];
        f32 wav1[128];
        f32 wav2[128];
        f32 wav3[128];
    };

} } } }
