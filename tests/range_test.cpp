#include <refrange/text/text_reader.h>
#include <gtest/gtest.h>


static bool is_space(const unsigned char *p)
{
    return *p==' ';
}

static bool is_not_space(const unsigned char *p)
{
    return !is_space(p);
}


TEST(RangeTest, range) 
{
    auto buf="  abc ef ghi  ";

    auto range=refrange::strrange(buf);
    auto found=range.find_range_if(&is_not_space);

    EXPECT_EQ(refrange::immutable_range(
		(const unsigned char*)(buf+2), 
		(const unsigned char*)(buf+5)), 
		found);
}

TEST(RangeTest, trim) 
{
    auto buf=
        "1\r\n"
        "\r\n"
        "  2\r\n"
		" 3\n"
        ;

    auto range=refrange::strrange(buf);
    refrange::text::line_reader reader(range);

	EXPECT_EQ(refrange::strrange("1"), reader.get_line());
	{
		auto line = reader.get_line();
		auto expected = refrange::strrange("  2");
		EXPECT_EQ(expected, line);
	}
	EXPECT_EQ(refrange::strrange("3"), reader.get_line().ltrim());
}

TEST(RangeTest, text_reader) 
{
    auto buf=" 1 2 3";
    auto range=refrange::strrange(buf);
    refrange::text::text_reader reader(range);


    EXPECT_EQ(1, reader.get_int());
    EXPECT_EQ(2, reader.get_int());
    EXPECT_EQ(3, reader.get_int());
}


TEST(TextTest, text) 
{
    EXPECT_TRUE(refrange::text::is_space(" "));

    EXPECT_EQ(1, refrange::strrange("1").to_int());
    EXPECT_EQ(-1, refrange::strrange("-1").to_int());

    EXPECT_EQ(1.5, refrange::strrange("1.5").to_double());
}

