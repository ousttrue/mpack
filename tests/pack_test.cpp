#include <mpack.h>
#include <gtest/gtest.h>

/// nil:
/// +--------+
/// |  0xc0  |
/// +--------+
TEST(MsgpackTest, nil) 
{
    // packing
    mpack::msgpack::vector_packer p;
    p.pack_nil();
    auto &buffer=p.packed_buffer;

    // check
    ASSERT_EQ(1, buffer.size());
    EXPECT_EQ(0xc0, buffer[0]);
}

/// false:
/// +--------+
/// |  0xc2  |
/// +--------+
TEST(MsgpackTest, false)
{
    // packing
    mpack::msgpack::vector_packer p;
    p.pack_bool(false);
    auto &buffer=p.packed_buffer;

    // check
    ASSERT_EQ(1, buffer.size());
    EXPECT_EQ(0xc2, buffer[0]);
}

/// true:
/// +--------+
/// |  0xc3  |
/// +--------+
TEST(MsgpackTest, true)
{
    // packing
	mpack::msgpack::vector_packer p;
    p.pack_bool(true);
    auto &buffer=p.packed_buffer;

    // check
    ASSERT_EQ(1, buffer.size());
    EXPECT_EQ(0xc3, buffer[0]);
}

/// positive fixnum stores 7-bit positive integer
/// +--------+
/// |0XXXXXXX|
/// +--------+
TEST(MsgpackTest, small_int)
{
    // packing
	mpack::msgpack::vector_packer p;
    p << 1;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(1, buffer.size());
	ASSERT_TRUE(mpack::msgpack::partial_bit_equal<mpack::msgpack::positive_fixint>(buffer[0]));

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    int n=-0;
    u >> n;
    EXPECT_EQ(1, n);
}

/// negative fixnum stores 5-bit negative integer
/// +--------+
/// |111YYYYY|
/// +--------+
TEST(MsgpackTest, small_negative_int)
{
    // packing
	mpack::msgpack::vector_packer p;
    p << -1;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(1, buffer.size());
	ASSERT_TRUE(mpack::msgpack::partial_bit_equal<mpack::msgpack::negative_fixint>(buffer[0]));

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    int n=0;
    u >> n;
    EXPECT_EQ(-1, n);
}

/// uint 8 stores a 8-bit unsigned integer
/// +--------+--------+
/// |  0xcc  |ZZZZZZZZ|
/// +--------+--------+
TEST(MsgpackTest, uint8)
{
    // packing
	mpack::msgpack::vector_packer p;
    p << 128;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(2, buffer.size());
    EXPECT_EQ(0xcc, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    int n=0;
    u >> n;
    EXPECT_EQ(128, n);
}

/// uint 16 stores a 16-bit big-endian unsigned integer
/// +--------+--------+--------+
/// |  0xcd  |ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+
TEST(MsgpackTest, uint16)
{
    // packing
	mpack::msgpack::vector_packer p;
    p << 256;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(3, buffer.size());
    EXPECT_EQ(0xcd, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    int n=0;
    u >> n;
    EXPECT_EQ(256, n);
}

/// uint 32 stores a 32-bit big-endian unsigned integer
/// +--------+--------+--------+--------+--------+
/// |  0xce  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ
/// +--------+--------+--------+--------+--------+
TEST(MsgpackTest, uint32)
{
    {
        int value=65536;

        // packing
		mpack::msgpack::vector_packer p;
        p << value;
        auto &buffer=p.packed_buffer;
        ASSERT_FALSE(buffer.empty());

        // check
        ASSERT_EQ(5, buffer.size());
        EXPECT_EQ(0xce, buffer[0]);

        // unpack
		auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
        int n=0;
        u >> n;
        EXPECT_EQ(value, n);
    }

    {
        unsigned int value=4294967295;

        // packing
		mpack::msgpack::vector_packer p;
        p << value;
        auto &buffer=p.packed_buffer;
        ASSERT_FALSE(buffer.empty());

        // check
        ASSERT_EQ(5, buffer.size());
        EXPECT_EQ(0xce, buffer[0]);

        // unpack
		auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
        int n=0;
        u >> n;
        EXPECT_EQ(value, n);
    }
}

/// uint 64 stores a 64-bit big-endian unsigned integer
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
/// |  0xcf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
TEST(MsgpackTest, uint64)
{
    unsigned long long value=4294967296;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(9, buffer.size());
    EXPECT_EQ(0xcf, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    unsigned long long n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// int 8 stores a 8-bit signed integer
/// +--------+--------+
/// |  0xd0  |ZZZZZZZZ|
/// +--------+--------+
TEST(MsgpackTest, int8)
{
    char value=-32;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(2, buffer.size());
    EXPECT_EQ(0xd0, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    char n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// int 16 stores a 16-bit big-endian signed integer
/// +--------+--------+--------+
/// |  0xd1  |ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+
TEST(MsgpackTest, int16)
{
    int value=-256;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(3, buffer.size());
    EXPECT_EQ(0xd1, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    int n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// int 32 stores a 32-bit big-endian signed integer
/// +--------+--------+--------+--------+--------+
/// |  0xd2  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+--------+--------+
TEST(MsgpackTest, int32)
{
    int value=-65535;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(5, buffer.size());
    EXPECT_EQ(0xd2, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    int n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// int 64 stores a 64-bit big-endian signed integer
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
/// |  0xd3  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
TEST(MsgpackTest, int64)
{
    long long value=-4294967296;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(9, buffer.size());
    EXPECT_EQ(0xd3, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    long long n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// float 32 stores a floating point number in IEEE 754 single precision floating point number format:
/// +--------+--------+--------+--------+--------+
/// |  0xca  |XXXXXXXX|XXXXXXXX|XXXXXXXX|XXXXXXXX
/// +--------+--------+--------+--------+--------+
TEST(MsgpackTest, float32)
{
    float value=1.5f;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(5, buffer.size());
    EXPECT_EQ(0xca, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    float n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// float 64 stores a floating point number in IEEE 754 double precision floating point number format:
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
/// |  0xcb  |YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|YYYYYYYY|
/// +--------+--------+--------+--------+--------+--------+--------+--------+--------+
TEST(MsgpackTest, float64)
{
    double value=1.111111111111111111111111111111111111111111111111;

    // packing
	mpack::msgpack::vector_packer p;
    p << value;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(9, buffer.size());
    EXPECT_EQ(0xcb, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    double n=0;
    u >> n;
    EXPECT_EQ(value, n);
}

/// fixstr stores a byte array whose length is upto 31 bytes:
/// +--------+========+
/// |101XXXXX|  data  |
/// +--------+========+
TEST(MsgpackTest, fixstr)
{
    auto str="abc";

    // packing
	mpack::msgpack::vector_packer p;
    p << str;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(1+3, buffer.size());
	ASSERT_TRUE(mpack::msgpack::partial_bit_equal<mpack::msgpack::fixstr>(buffer[0]));

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::string out;
    u >> out;

    EXPECT_EQ(out, str);
}

/// str 8 stores a byte array whose length is upto (2^8)-1 bytes:
/// +--------+--------+========+
/// |  0xd9  |YYYYYYYY|  data  |
/// +--------+--------+========+
TEST(MsgpackTest, str8)
{
    auto str=
        "0123456789"
        "0123456789"
        "0123456789"
        "01"
        ;

    // packing
	mpack::msgpack::vector_packer p;
    p << str;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(2+32, buffer.size());
    EXPECT_EQ(0xd9, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::string out;
    u >> out;

    EXPECT_EQ(out, str);
}

/// str 16 stores a byte array whose length is upto (2^16)-1 bytes:
/// +--------+--------+--------+========+
/// |  0xda  |ZZZZZZZZ|ZZZZZZZZ|  data  |
/// +--------+--------+--------+========+
TEST(MsgpackTest, str16)
{
    auto str=
        "0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
        "0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
        "0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
        "0123456789" "0123456789" "0123456789" "0123456789" "0123456789" "0123456789"
        "0123456789" "012345"
        ;

    // packing
	mpack::msgpack::vector_packer p;
    p << str;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(3+256, buffer.size());
    EXPECT_EQ(0xda, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::string out;
    u >> out;

    EXPECT_EQ(out, str);
}

/// str 32 stores a byte array whose length is upto (2^32)-1 bytes:
/// +--------+--------+--------+--------+--------+========+
/// |  0xdb  |AAAAAAAA|AAAAAAAA|AAAAAAAA|AAAAAAAA|  data  |
/// +--------+--------+--------+--------+--------+========+
TEST(MsgpackTest, str32)
{
    std::string str;
    for(int i=0; i<(0xFFFF+1); ++i)
    {
        str.push_back('0'+(i%10));
    }

    // packing
	mpack::msgpack::vector_packer p;
    p << str;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(5+0xFFFF+1, buffer.size());
    EXPECT_EQ(0xdb, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::string out;
    u >> out;

    EXPECT_EQ(out, str);
}

/// bin 8 stores a byte array whose length is upto (2^8)-1 bytes:
/// +--------+--------+========+
/// |  0xc4  |XXXXXXXX|  data  |
/// +--------+--------+========+
TEST(MsgpackTest, bin8)
{
    std::vector<unsigned char> buf;
    buf.push_back(0);

    // packing
	mpack::msgpack::vector_packer p;
    p << buf;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(2+1, buffer.size());
    EXPECT_EQ(0xc4, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::vector<unsigned char> out;
    u >> out;

    EXPECT_EQ(buf, out);
}

/// bin 16 stores a byte array whose length is upto (2^16)-1 bytes:
/// +--------+--------+--------+========+
/// |  0xc5  |YYYYYYYY|YYYYYYYY|  data  |
/// +--------+--------+--------+========+
TEST(MsgpackTest, bin16)
{
    std::vector<unsigned char> buf;
    for(int i=0; i<0xFF+1; ++i){
        buf.push_back(i % 0xFF);
    }

    // packing
	mpack::msgpack::vector_packer p;
    p << buf;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(3+0xFF+1, buffer.size());
    EXPECT_EQ(0xc5, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::vector<unsigned char> out;
    u >> out;

    EXPECT_EQ(buf, out);
}

/// bin 32 stores a byte array whose length is upto (2^32)-1 bytes:
/// +--------+--------+--------+--------+--------+========+
/// |  0xc6  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|  data  |
/// +--------+--------+--------+--------+--------+========+
TEST(MsgpackTest, bin32)
{
    std::vector<unsigned char> buf;
    for(int i=0; i<0xFFFF+1; ++i){
        buf.push_back(i % 0xFF);
    }

    // packing
	mpack::msgpack::vector_packer p;
    p << buf;
    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    ASSERT_EQ(5+0xFFFF+1, buffer.size());
    EXPECT_EQ(0xc6, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    std::vector<unsigned char> out;
    u >> out;

    EXPECT_EQ(buf, out);
}

/// fixarray stores an array whose length is upto 15 elements:
/// +--------+~~~~~~~~~~~~~~~~~+
/// |1001XXXX|    N objects    |
/// +--------+~~~~~~~~~~~~~~~~~+
TEST(MsgpackTest, fixarray)
{
    // packing
	mpack::msgpack::vector_packer p;
	p << mpack::msgpack::array_context(3)
        << 1 << "str" << true
		;

    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
	ASSERT_TRUE(mpack::msgpack::partial_bit_equal<mpack::msgpack::fixarray>(buffer[0]));

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    EXPECT_TRUE(u.is_array());

	auto c = mpack::msgpack::array_context();
    int n;
    std::string str;
    bool b;
    u >> c >> n >> str >> b;

    EXPECT_EQ(3, c.size);
    EXPECT_EQ(1, n);
    EXPECT_EQ("str", str);
    EXPECT_EQ(true, b);
}

/// array 16 stores an array whose length is upto (2^16)-1 elements:
/// +--------+--------+--------+~~~~~~~~~~~~~~~~~+
/// |  0xdc  |YYYYYYYY|YYYYYYYY|    N objects    |
/// +--------+--------+--------+~~~~~~~~~~~~~~~~~+
TEST(MsgpackTest, array16)
{
    // packing
	mpack::msgpack::vector_packer p;
	p << mpack::msgpack::array_context(16)
        << 1 << "str1" << true << 1.5f
        << 2 << "str2" << false << 1.6f
        << 3 << "str3" << true << 1.7
        << 4 << "str4" << false << 1.8
		;

    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    EXPECT_EQ(0xdc, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    EXPECT_TRUE(u.is_array());

    // array
	auto c = mpack::msgpack::array_context();
    u >> c ;
    EXPECT_EQ(16, c.size);

    {
        int n;
        std::string str;
        bool b;
        double f;
        u >> n >> str >> b >> f
            ;
        EXPECT_EQ(1, n);
        EXPECT_EQ("str1", str);
        EXPECT_EQ(true, b);
        EXPECT_EQ(1.5f, f);
    }
}

/// array 32 stores an array whose length is upto (2^32)-1 elements:
/// +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
/// |  0xdd  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|    N objects    |
/// +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
TEST(MsgpackTest, array32)
{
    // packing
	mpack::msgpack::vector_packer p;
	auto pc = mpack::msgpack::array_context(0xFFFF + 1);
    p << pc;
    for(int i=0; i<pc.size; ++i){
        p << i;
    }

    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    EXPECT_EQ(0xdd, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    EXPECT_TRUE(u.is_array());

    // array
	auto uc = mpack::msgpack::array_context();
    u >> uc ;
    EXPECT_EQ(pc.size, uc.size);

    for(int i=0; i<uc.size; ++i){
        int n;
        u >> n;
        EXPECT_EQ(i, n);
    }
}

/// fixmap stores a map whose length is upto 15 elements
/// +--------+~~~~~~~~~~~~~~~~~+
/// |1000XXXX|   N*2 objects   |
/// +--------+~~~~~~~~~~~~~~~~~+
TEST(MsgpackTest, fixmap)
{
    // packing
	mpack::msgpack::vector_packer p;
	p << mpack::msgpack::map_context(3)
        << "key1" << 0
        << "key2" << 1
        << "key3" << 2
		;

    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
	ASSERT_TRUE(mpack::msgpack::partial_bit_equal<mpack::msgpack::fixmap>(buffer[0]));

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    EXPECT_TRUE(u.is_map());

	auto c = mpack::msgpack::map_context();
    u >> c;
    EXPECT_EQ(3, c.size);

    for(int i=0; i<c.size; ++i){
        std::string key;
        int value;
        u >> key >> value;
        std::stringstream ss;
        ss << "key" << (i+1);
        EXPECT_EQ(ss.str(), key);
        EXPECT_EQ(i, value);
    }
}

/// map 16 stores a map whose length is upto (2^16)-1 elements
/// +--------+--------+--------+~~~~~~~~~~~~~~~~~+
/// |  0xde  |YYYYYYYY|YYYYYYYY|   N*2 objects   |
/// +--------+--------+--------+~~~~~~~~~~~~~~~~~+
TEST(MsgpackTest, map16)
{
    // packing
	mpack::msgpack::vector_packer p;
	p << mpack::msgpack::map_context(17)
        << "key1" << 0 << "key2" << 1 << "key3" << 2 << "key4" << 3
        << "key5" << 4 << "key6" << 5 << "key7" << 6 << "key8" << 7
        << "key9" << 8 << "key10" << 9 << "key11" << 10 << "key12" << 11
        << "key13" << 12 << "key14" << 13 << "key15" << 14 << "key16" << 15
        << "key17" << 16
		;

    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    EXPECT_EQ(0xde, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    EXPECT_TRUE(u.is_map());

    // map
	auto c = mpack::msgpack::map_context();
    u >> c ;
    EXPECT_EQ(17, c.size);

    for(int i=0; i<c.size; ++i){
        std::string key;
        int value;
        u >> key >> value;
        std::stringstream ss;
        ss << "key" << (i+1);
        EXPECT_EQ(ss.str(), key);
        EXPECT_EQ(i, value);
    }
}

/// map 32 stores a map whose length is upto (2^32)-1 elements
/// +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
/// |  0xdf  |ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|ZZZZZZZZ|   N*2 objects   |
/// +--------+--------+--------+--------+--------+~~~~~~~~~~~~~~~~~+
TEST(MsgpackTest, map32)
{
    // packing
	mpack::msgpack::vector_packer p;
	auto pc = mpack::msgpack::map_context(0xFFFF + 1);
    p << pc; 
    for(int i=0; i<pc.size; ++i){
        std::stringstream ss;
        ss << "key" << (i+1);
        p << ss.str() << i;
    }

    auto &buffer=p.packed_buffer;
    ASSERT_FALSE(buffer.empty());

    // check
    EXPECT_EQ(0xdf, buffer[0]);

    // unpack
	auto u = mpack::msgpack::memory_unpacker(&buffer[0], buffer.size());
    EXPECT_TRUE(u.is_map());

    // map
	auto uc = mpack::msgpack::map_context();
    u >> uc ;
    EXPECT_EQ(pc.size, uc.size);

    for(int i=0; i<uc.size; ++i){
        std::string key;
        int value;
        u >> key >> value;
        std::stringstream ss;
        ss << "key" << (i+1);
        EXPECT_EQ(ss.str(), key);
        EXPECT_EQ(i, value);
    }
}

