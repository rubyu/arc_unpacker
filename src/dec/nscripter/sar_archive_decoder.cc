#include "dec/nscripter/sar_archive_decoder.h"
#include "algo/range.h"

using namespace au;
using namespace au::dec::nscripter;

bool SarArchiveDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.path.has_extension("sar");
}

std::unique_ptr<dec::ArchiveMeta> SarArchiveDecoder::read_meta_impl(
    const Logger &logger, io::File &input_file) const
{
    const auto file_count = input_file.stream.read_be<u16>();
    const auto offset_to_data = input_file.stream.read_be<u32>();
    auto meta = std::make_unique<ArchiveMeta>();
    for (const auto i : algo::range(file_count))
    {
        auto entry = std::make_unique<PlainArchiveEntry>();
        entry->path = input_file.stream.read_to_zero().str();
        entry->offset = input_file.stream.read_be<u32>() + offset_to_data;
        entry->size = input_file.stream.read_be<u32>();
        meta->entries.push_back(std::move(entry));
    }
    return meta;
}

std::unique_ptr<io::File> SarArchiveDecoder::read_file_impl(
    const Logger &logger,
    io::File &input_file,
    const dec::ArchiveMeta &m,
    const dec::ArchiveEntry &e) const
{
    const auto entry = static_cast<const PlainArchiveEntry*>(&e);
    const auto data = input_file.stream.seek(entry->offset).read(entry->size);
    return std::make_unique<io::File>(entry->path, data);
}

static auto _ = dec::register_decoder<SarArchiveDecoder>("nscripter/sar");
