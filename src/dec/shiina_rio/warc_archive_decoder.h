#pragma once

#include "dec/base_archive_decoder.h"
#include "dec/shiina_rio/warc/plugin.h"
#include "plugin_manager.h"

namespace au {
namespace dec {
namespace shiina_rio {

    class WarcArchiveDecoder final : public BaseArchiveDecoder
    {
    public:
        WarcArchiveDecoder();
        std::vector<std::string> get_linked_formats() const override;

    protected:
        bool is_recognized_impl(io::File &input_file) const override;

        std::unique_ptr<ArchiveMeta> read_meta_impl(
            const Logger &logger,
            io::File &input_file) const override;

        std::unique_ptr<io::File> read_file_impl(
            const Logger &logger,
            io::File &input_file,
            const ArchiveMeta &m,
            const ArchiveEntry &e) const override;

    private:
        PluginManager<warc::PluginBuilder> plugin_manager;
    };

} } }
