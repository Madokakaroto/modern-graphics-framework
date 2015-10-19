#include <iostream>
#include <pipeline\resource\buffer.hpp>

#include <fstream>

using float4 = leaves::float4;
using float3 = leaves::float3;
using float2 = leaves::float2;
using float4x4 = leaves::float4x4;
using uint32_t = leaves::uint32_t;

struct begin_with
{
	float3 position;
	float3 normal;
	float2 tex_coord;
	float4 color;
};

struct cbuffer_host
{
	using tuple_type = std::tuple<float3, float2, float2>;

	float3 dir;
	float2 coord1;
	float2 coord2;
};

struct cbuffer_device
{
	float4 dir;
	float2 coord1;
	float2 coord2;
};

void test_vertex_buffer()
{
	using vertex_buffer = leaves::pipeline::vertex_buffer;
	using input_layout = leaves::pipeline::input_layout;
	using data_format = leaves::pipeline::data_format;
	using data_semantic = leaves::pipeline::data_semantic;

	input_layout layout;
	layout.add(data_format::float3, data_semantic::position);
	layout.add(data_format::float3, data_semantic::normal);
	layout.add(data_format::float2, data_semantic::texcoord0);
	layout.add(data_format::float4, data_semantic::color0);

	vertex_buffer vb{ L"my vertex buffer", std::move(layout), 4 };

	auto ptr = vb.ptr_as<begin_with>();
}

void test_index_buffer()
{
	using index_buffer = leaves::pipeline::index_buffer;
	using data_format = leaves::pipeline::data_format;
	using primitive_type = leaves::pipeline::primitive_type;

	index_buffer ib{ L"my index buffer", primitive_type::triangle_list_adj, data_format::uint, 16 };

	auto ptr = ib.ptr_as<uint32_t>();
}

void test_constant_buffer()
{
	using constant_buffer = leaves::pipeline::constant_buffer;
	using structured_layout = leaves::pipeline::structured_layout;
	using data_format = leaves::pipeline::data_format;
	using leaves::pipeline::wrap_large_class;

	structured_layout layout = wrap_large_class<cbuffer_host>();

	constant_buffer cbuffer{ L"my constant buffer", std::move(layout) };

	cbuffer[0].interpret_as<float3>() = { 1.0f, 1.0f, 1.0f };
	cbuffer[1].interpret_as<float2>() = { 2.0f, 2.0f };
	cbuffer[2].interpret_as<float2>() = { 3.0f, 3.0f };

	auto ptr = cbuffer.ptr_as<cbuffer_device>();
}

int main(void)
{
	test_vertex_buffer();
	test_index_buffer();
	test_constant_buffer();

	return 0;
}