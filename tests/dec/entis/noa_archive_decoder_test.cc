#include "dec/entis/noa_archive_decoder.h"
#include "test_support/catch.h"
#include "test_support/decoder_support.h"
#include "test_support/file_support.h"

using namespace au;
using namespace au::dec::entis;

static const std::string dir = "tests/dec/entis/files/noa/";

static void do_test(
    const std::string &input_path,
    const std::vector<std::shared_ptr<io::File>> expected_files)
{
    const auto decoder = NoaArchiveDecoder();
    const auto input_file = tests::file_from_path(dir + input_path);
    const auto actual_files = tests::unpack(decoder, *input_file);
    tests::compare_files(actual_files, expected_files, true);
}

TEST_CASE("Entis NOA archives", "[dec]")
{
    SECTION("Unencrypted")
    {
        do_test("unencrypted.noa",
            {
                tests::stub_file("123.txt", "1234567890"_b),
                tests::stub_file("abc.txt", "abcdefghijklmnopqrstuvwxyz"_b),
            });
    }

    SECTION("Compressed with ErisaN")
    {
        do_test(
            "encrypted-erisan.noa",
            {
                tests::file_from_path(dir + "system.inf", "system.inf"),
            });
    }
}
