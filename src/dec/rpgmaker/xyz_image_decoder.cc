#include "dec/rpgmaker/xyz_image_decoder.h"
#include "algo/pack/zlib.h"
#include "algo/range.h"
#include "io/memory_byte_stream.h"

using namespace au;
using namespace au::dec::rpgmaker;

static const bstr magic = "XYZ1"_b;

bool XyzImageDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.stream.read(magic.size()) == magic;
}

res::Image XyzImageDecoder::decode_impl(
    const Logger &logger, io::File &input_file) const
{
    input_file.stream.skip(magic.size());
    const auto width = input_file.stream.read_le<u16>();
    const auto height = input_file.stream.read_le<u16>();
    const auto data = algo::pack::zlib_inflate(input_file.stream.read_to_eof());
    io::MemoryByteStream data_stream(data);
    const auto pal_data = data_stream.read(256 * 3);
    const auto pix_data = data_stream.read_to_eof();
    res::Palette palette(256, pal_data, res::PixelFormat::RGB888);
    return res::Image(width, height, pix_data, palette);
}

static auto _ = dec::register_decoder<XyzImageDecoder>("rpgmaker/xyz");
