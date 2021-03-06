#include "dec/dogenzaka/bin_archive_decoder.h"
#include "algo/range.h"

using namespace au;
using namespace au::dec::dogenzaka;

bool BinArchiveDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.path.has_extension("bin");
}

std::unique_ptr<dec::ArchiveMeta> BinArchiveDecoder::read_meta_impl(
    const Logger &logger, io::File &input_file) const
{
    const auto file_count = input_file.stream.read_le<u32>();
    auto meta = std::make_unique<ArchiveMeta>();
    const auto first_u32 = input_file.stream.read_le<u32>();
    input_file.stream.skip(-4);

    if (first_u32 == 0)
    {
        const uoff_t data_start = 4 + 4 * file_count;
        PlainArchiveEntry *last_entry = nullptr;
        for (const auto i : algo::range(file_count))
        {
            auto entry = std::make_unique<PlainArchiveEntry>();
            entry->offset = input_file.stream.read_le<u32>() + data_start;
            if (last_entry)
                last_entry->size = entry->offset - last_entry->offset;
            last_entry = entry.get();
            meta->entries.push_back(std::move(entry));
        }
    if (last_entry)
        last_entry->size = input_file.stream.size() - last_entry->offset;
    }
    else
    {
        for (const auto i : algo::range(file_count))
        {
            auto entry = std::make_unique<PlainArchiveEntry>();
            entry->offset = input_file.stream.read_le<u32>() + 12;
            entry->size = input_file.stream.read_le<u32>() - 12;
            meta->entries.push_back(std::move(entry));
        }
    }
    return meta;
}

std::unique_ptr<io::File> BinArchiveDecoder::read_file_impl(
    const Logger &logger,
    io::File &input_file,
    const dec::ArchiveMeta &m,
    const dec::ArchiveEntry &e) const
{
    const auto entry = static_cast<const PlainArchiveEntry*>(&e);
    const auto data = input_file.stream.seek(entry->offset).read(entry->size);
    auto file = std::make_unique<io::File>(entry->path, data);
    file->guess_extension();
    return file;
}

static auto _ = dec::register_decoder<BinArchiveDecoder>("dogenzaka/bin");
