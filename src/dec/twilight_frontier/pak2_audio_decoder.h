#pragma once

#include "dec/base_audio_decoder.h"

namespace au {
namespace dec {
namespace twilight_frontier {

    class Pak2AudioDecoder final : public BaseAudioDecoder
    {
    protected:
        bool is_recognized_impl(io::File &input_file) const override;
        res::Audio decode_impl(
            const Logger &logger, io::File &input_file) const override;
    };

} } }
