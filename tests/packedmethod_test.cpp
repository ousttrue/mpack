#include <mpack.h>
#include <gtest/gtest.h>


static int zero(void)
{
    return 0;
}

static int one(int a)
{
    return ++a;
}

static int bin(int a, int b)
{
    return a+b;
}


TEST(PackedMethod, fp_0to1) 
{
    // wrap static function pointer
    auto method=mpack::msgpack::rpc::packmethod(zero);

    // call method(packing result to result_packer)
    auto result_packer=mpack::msgpack::create_vector_packer();
    method(result_packer, 
            mpack::msgpack::create_unpacker_from_packer(
                mpack::msgpack::create_vector_packer() 
                << mpack::msgpack::array(0) 
                ));

    // unpack result
	auto u = mpack::msgpack::create_unpacker_from_packer(result_packer);
    int n;
    u >> n;
    EXPECT_EQ(0, n);
}

TEST(PackedMethod, fp_1to1) 
{
    // wrap static function pointer
    auto method=mpack::msgpack::rpc::packmethod(one);

    // call method(packing result to result_packer)
    auto result_packer=mpack::msgpack::create_vector_packer();
    method(result_packer, 
            mpack::msgpack::create_unpacker_from_packer(
                mpack::msgpack::create_vector_packer() 
                << mpack::msgpack::array(1) 
                << 1
                ));

    // unpack result
	auto u = mpack::msgpack::create_unpacker_from_packer(result_packer);
    int n;
    u >> n;
    EXPECT_EQ(2, n);
}

TEST(PackedMethod, fp_2to1) 
{
    // wrap static function pointer
    auto method=mpack::msgpack::rpc::packmethod(bin);

    // call method(packing result to result_packer)
    auto result_packer=mpack::msgpack::create_vector_packer();
    method(result_packer, 
            mpack::msgpack::create_unpacker_from_packer(
                mpack::msgpack::create_vector_packer() 
                << mpack::msgpack::array(2) 
                << 1 << 2
                ));

    // unpack result
	auto u = mpack::msgpack::create_unpacker_from_packer(result_packer);
    int n;
    u >> n;
    EXPECT_EQ(3, n);
}

TEST(PackedMethod, lambda_2to1) 
{
    auto add_lambda=[](int a, int b){ return a+b; };
    auto method=mpack::msgpack::rpc::packmethod(add_lambda);

    // call method(packing result to result_packer)
    auto result_packer=mpack::msgpack::create_vector_packer();
    method(result_packer, 
            mpack::msgpack::create_unpacker_from_packer(
                mpack::msgpack::create_vector_packer() 
                << mpack::msgpack::array(2) 
                << 1 << 2
                ));

    // unpack result
	auto u = mpack::msgpack::create_unpacker_from_packer(result_packer);
    int n;
    u >> n;
    EXPECT_EQ(3, n);
}

