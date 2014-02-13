#include <gtest/gtest.h>
#include <mpack.h>

// Tests factorial of negative numbers.
TEST(MsgpackTest, Sample) {

    mpack::packer p;
    p << 1;

    auto buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    mpack::reference_unpacker u(&buffer[0], buffer.size());

    int n=-1;
    u >> n;
    EXPECT_EQ(1, n);
}
