#include <iostream>
#include <pipeline\resource\buffer\vertex_buffer.hpp>

struct begin_with
{
	float a[9];
};

int main(void)
{
	using vertex_buffer = leaves::pipeline::vertex_buffer;
	using input_layout = leaves::pipeline::input_layout;
	using data_format = leaves::pipeline::data_format;
	using data_semantic = leaves::pipeline::data_semantic;
	using device_access = leaves::pipeline::device_access;

	using float4 = leaves::float4;

	input_layout layout;
	layout.add(data_format::float4, data_semantic::position);
	layout.add(data_format::float2, data_semantic::texcoord0);
	layout.add(data_format::float3, data_semantic::color0);

	vertex_buffer vb{ L"my vertex buffer", std::move(layout), 4 };

	auto data = vb.data();
	auto begin = vb.begin<float4>(data_semantic::position);
	auto end = vb.end<float4>(data_semantic::position);

	auto whole_begin = vb.begin<float4>();

	auto ptr = vb.get<const begin_with>();
}