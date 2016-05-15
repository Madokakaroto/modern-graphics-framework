#include <iostream>
#include <pipeline\resource\texture.hpp>

#include <boost\fusion\adapted.hpp>

using float4 = leaves::float4;
using float3 = leaves::float3;
using float2 = leaves::float2;
using float4x4 = leaves::float4x4;
using uint32_t = leaves::uint32_t;

void test_texture_1d()
{
	using leaves::pipeline::pixel_format;
	using leaves::pipeline::texture_1d;
	using meta_type = texture_1d::meta;

	meta_type meta_data;
	meta_data.format = pixel_format::rgba_32;
	meta_data.has_mipmap = true;
	meta_data.width = 256;
	texture_1d texture{ "texture 1d", meta_data };
}

void test_texture_2d()
{
	using leaves::pipeline::pixel_format;
	using leaves::pipeline::texture_2d;
	using meta_type = texture_2d::meta;

	// create a texture
	meta_type meta_data;
	meta_data.format = pixel_format::rgba_32;
	meta_data.has_mipmap = true;
	meta_data.height = 128;
	meta_data.width = 128;
	texture_2d texture{ "texture 2d", meta_data };
}

void test_texture_3d()
{
	using leaves::pipeline::pixel_format;
	using leaves::pipeline::texture_3d;
	using meta_type = texture_3d::meta;

	// create a texture
	meta_type meta_data;
	meta_data.format = pixel_format::rgba_32;
	meta_data.has_mipmap = true;
	meta_data.height = 128;
	meta_data.width = 128;
	meta_data.depth = 128;
	texture_3d texture{ "texture 3d", meta_data };
}

void test_texture_1d_array()
{
	using leaves::pipeline::pixel_format;
	using leaves::pipeline::texture_1d_array;
	using meta_type = texture_1d_array::meta;

	meta_type meta_data;
	meta_data.format = pixel_format::rgba_32;
	meta_data.array_size = 4;
	meta_data.width = 128;
	meta_data.has_mipmap = true;
	texture_1d_array texture_array{ "texture array 1d",  meta_data };
}

void test_texture_2d_array()
{
	using leaves::pipeline::texture_2d_array;
	using leaves::pipeline::pixel_format;
	using meta_type = texture_2d_array::meta;

	meta_type meta_data;
	meta_data.format = pixel_format::rgba_32;
	meta_data.width = 128;
	meta_data.height = 128;
	meta_data.array_size = 4;
	meta_data.has_mipmap = true;
	texture_2d_array texture_array{ "texture array 2d", meta_data };
}

void test_texture_cube()
{
	using leaves::pipeline::texture_cube;
	using leaves::pipeline::pixel_format;
	using meta_type = texture_cube::meta;
	
	meta_type meta_data;
	meta_data.format = pixel_format::rgba_32;
	meta_data.has_mipmap = true;
	meta_data.height = 128;
	meta_data.width = 128;
	texture_cube cube_texture{ "texture cube", meta_data };
}

void test_texture_cube_array()
{
	using leaves::pipeline::texture_cube_array;
	using leaves::pipeline::pixel_format;
	using meta_type = texture_cube_array::meta;
	
	meta_type meta_data;
	meta_data.width = 128;
	meta_data.height = 128;
	meta_data.array_size = 6;
	meta_data.format = pixel_format::rgba_32;
	meta_data.has_mipmap = true;
	texture_cube_array cube_array{ "texture cube array", meta_data };
	
	cube_array.deallocate();
}

void test_texture_ds()
{
	using leaves::pipeline::texture_ds;
	using leaves::pipeline::pixel_format;
	using meta_type = texture_ds::meta;

	meta_type meta_data;
	meta_data.width = 128;
	meta_data.height = 128;
	meta_data.format = pixel_format::rgba_32;
	texture_ds texture{ "texture depth stencil",  meta_data };
}

void test_texture_rt()
{
	using leaves::pipeline::texture_rt;
	using leaves::pipeline::pixel_format;
	using meta_type = texture_rt::meta;

	meta_type meta_data;
	meta_data.width = 128;
	meta_data.height = 128;
	meta_data.format = pixel_format::rgba_32;
	texture_rt texture{ "texture render target",  meta_data };
}

namespace leaves
{
	struct foo
	{
		int a;
		bool b;
	};

	struct fee
	{
		int a;
		bool b;
	};
}

BOOST_FUSION_ADAPT_STRUCT(
	leaves::foo,
	(int , a)
	(bool, b))

int main(void)
{
	// test texture
	test_texture_1d();
	test_texture_2d();
	test_texture_3d();
	test_texture_1d_array();
	test_texture_2d_array();
	test_texture_cube();
	test_texture_cube_array();
	test_texture_ds();
	test_texture_rt();

	auto r = boost::fusion::traits::is_sequence<leaves::fee>::value;
	r = boost::fusion::traits::is_sequence<leaves::foo>::value;

	using type = std::array<leaves::float2x4, 4>;

	auto result = leaves::pipeline::numeric_traits<type>::size();

	return 0;
}