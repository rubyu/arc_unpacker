#include "dec/yumemiru/epf_image_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::dec::yumemiru;

static const std::string dir = "tests/dec/yumemiru/files/epf/";

static void do_test(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = EpfImageDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_image = tests::decode(decoder, *input_file);
    tests::compare_images(actual_image, *expected_file);
}

TEST_CASE("Yumemiru EPF images", "[dec]")
{
    do_test("IF_cg_bg_minyusyu_c.epf", "IF_cg_bg_minyusyu_c-out.png");
}
