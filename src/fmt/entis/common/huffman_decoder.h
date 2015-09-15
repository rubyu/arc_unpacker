#pragma once

#include "fmt/entis/common/decoder.h"
#include "fmt/entis/common/huffman_tree.h"

namespace au {
namespace fmt {
namespace entis {
namespace common {

    int get_huffman_code(io::BitReader &bit_reader, HuffmanTree &tree);
    int get_huffman_size(io::BitReader &bit_reader, HuffmanTree &tree);

    class HuffmanDecoder final : public Decoder
    {
    public:
        HuffmanDecoder();
        ~HuffmanDecoder();

        virtual void reset() override;
        virtual void decode(u8 *output, size_t output_size) override;

    private:
        struct Priv;
        std::unique_ptr<Priv> p;
    };

} } } }