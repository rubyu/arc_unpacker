#include "dec/active_soft/adpack_archive_decoder.h"
#include "algo/range.h"

using namespace au;
using namespace au::dec::active_soft;

static const bstr magic = "ADPACK32"_b;

bool AdpackArchiveDecoder::is_recognized_impl(io::File &input_file) const
{
    return input_file.stream.read(magic.size()) == magic;
}

std::unique_ptr<dec::ArchiveMeta> AdpackArchiveDecoder::read_meta_impl(
    const Logger &logger, io::File &input_file) const
{
    input_file.stream.seek(magic.size() + 4);
    const auto file_count = input_file.stream.read_le<u32>() - 1;
    input_file.stream.seek(0x10);
    PlainArchiveEntry *last_entry = nullptr;
    auto meta = std::make_unique<ArchiveMeta>();
    for (const auto i : algo::range(file_count))
    {
        auto entry = std::make_unique<PlainArchiveEntry>();
        entry->path = input_file.stream.read_to_zero(0x18).str();
        input_file.stream.skip(4);
        entry->offset = input_file.stream.read_le<u32>();
        if (last_entry)
            last_entry->size = entry->offset - last_entry->offset;
        last_entry = entry.get();
        meta->entries.push_back(std::move(entry));
    }
    if (last_entry)
        last_entry->size = input_file.stream.size() - last_entry->offset;
    return meta;
}

std::unique_ptr<io::File> AdpackArchiveDecoder::read_file_impl(
    const Logger &logger,
    io::File &input_file,
    const dec::ArchiveMeta &m,
    const dec::ArchiveEntry &e) const
{
    const auto entry = static_cast<const PlainArchiveEntry*>(&e);
    const auto data = input_file.stream.seek(entry->offset).read(entry->size);
    return std::make_unique<io::File>(entry->path, data);
}

std::vector<std::string> AdpackArchiveDecoder::get_linked_formats() const
{
    return {"active-soft/ed8", "active-soft/edt"};
}

static auto _
    = dec::register_decoder<AdpackArchiveDecoder>("active-soft/adpack");
