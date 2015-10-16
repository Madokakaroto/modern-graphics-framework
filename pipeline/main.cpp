#include <iostream>
#include <pipeline\resource\buffer\vertex_buffer.hpp>
#include <pipeline\resource\buffer\index_buffer.hpp>

#include <fstream>

struct begin_with
{
	float a[9];
};

int main(void)
{
	using vertex_buffer = leaves::pipeline::vertex_buffer;
	using index_buffer = leaves::pipeline::index_buffer;
	using input_layout = leaves::pipeline::input_layout;
	using data_format = leaves::pipeline::data_format;
	using data_semantic = leaves::pipeline::data_semantic;
	using device_access = leaves::pipeline::device_access;
	using primitive_type = leaves::pipeline::primitive_type;

	using float4 = leaves::float4;

	input_layout layout;
	layout.add(data_format::float4, data_semantic::position);
	layout.add(data_format::float2, data_semantic::texcoord0);
	layout.add(data_format::float3, data_semantic::color0);

	vertex_buffer vb{ L"my vertex buffer", std::move(layout), 4 };

	index_buffer ib{ L"my index buffer", primitive_type::triangle_list_adj, data_format::uint, 16 };

	auto ptr_ib = ib.ptr_as<leaves::pipeline::uint32_t const>();

	// copy data
	auto data = vb.data();

	auto ptr_vb = vb.ptr_as<begin_with>();
}