#include "dec/bgi/bgi_audio_decoder.h"

using namespace au;
using namespace au::dec::bgi;

static const bstr magic = "bw\x20\x20"_b;

bool BgiAudioDecoder::is_recognized_impl(io::File &input_file) const
{
    input_file.stream.skip(4);
    return input_file.stream.read(magic.size()) == magic;
}

std::unique_ptr<io::File> BgiAudioDecoder::decode_impl(
    const Logger &logger, io::File &input_file) const
{
    input_file.stream.seek(0);
    const auto header_size = input_file.stream.read_le<u32>();
    input_file.stream.skip(magic.size());
    const auto input_file_size = input_file.stream.read_le<u32>();
    input_file.stream.seek(header_size);
    const auto data = input_file.stream.read(input_file_size);

    auto output_file = std::make_unique<io::File>(input_file.path, data);
    output_file->path.change_extension("ogg");
    return output_file;
}

static auto _ = dec::register_decoder<BgiAudioDecoder>("bgi/audio");
