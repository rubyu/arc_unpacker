#include "dec/active_soft/edt_image_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"
#include "test_support/image_support.h"

using namespace au;
using namespace au::dec::active_soft;

static const std::string dir = "tests/dec/active_soft/files/edt/";

static void do_test(
    const std::string &input_path, const std::string &expected_path)
{
    const auto decoder = EdtImageDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto expected_file = tests::file_from_path(dir + expected_path);
    const auto actual_image = tests::decode(decoder, *input_file);
    tests::compare_images(actual_image, *expected_file);
}

TEST_CASE("ActiveSoft EDT images", "[dec]")
{
    SECTION("Plain")
    {
        do_test("nowprit.edt", "nowprit-out.png");
    }

    SECTION("Difference-based")
    {
        do_test("nowprit.edt", "nowprit-out.png");
    }
}
