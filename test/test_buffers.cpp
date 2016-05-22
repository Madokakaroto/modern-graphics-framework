#include <pipeline/resource/buffer.hpp>

static constexpr size_t triangle_geom = 3;
static constexpr size_t quadrangle_geom = 4;
static constexpr size_t tetrahedron_geom = 12;
static constexpr size_t cubic_geom = 24;
static constexpr size_t cubic_vertices = 36;
static constexpr size_t octahedron = 24;

// numeric types
using float2 = leaves::float2;
using float3 = leaves::float3;
using float4 = leaves::float4;
using float4x4 = leaves::float4x4;

// graphics type
using vertex_buffer_t = leaves::pipeline::vertex_buffer;
using index_buffer_t = leaves::pipeline::index_buffer;
using constant_buffer_t = leaves::pipeline::constant_buffer;
using input_layout_t = leaves::pipeline::input_layout;
using structure_layout_t = leaves::pipeline::numeric_layout;
using format_t = leaves::pipeline::data_format;
using semantic_t = leaves::pipeline::data_semantic;
using primative_t = leaves::pipeline::primitive_type;

using leaves::wrap_large_class;

void test_vertex_buffer_and_index_buffer()
{
	struct vertex_data
	{
		float3	position;
		float3	normal;
	};

	vertex_data vertices[] = 
	{
		// left four points
		{ { -1.0f, -1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, { -1.0f,  0.0f,  0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, { -1.0f,  0.0f,  0.0f } },

		// right four points
		{ {  1.0f, -1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f } },
		{ {  1.0f, -1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f } },
		{ {  1.0f,  1.0f,  1.0f }, {  1.0f,  0.0f,  0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, {  1.0f,  0.0f,  0.0f } },

		// up four points
		{ {  1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f } },
		{ { -1.0f,  1.0f,  1.0f }, {  0.0f,  1.0f,  0.0f } },
		{ { -1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f } },
		{ {  1.0f,  1.0f, -1.0f }, {  0.0f,  1.0f,  0.0f } },

		// down four points
		{ {  1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f } },
		{ { -1.0f, -1.0f,  1.0f }, {  0.0f, -1.0f,  0.0f } },
		{ { -1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f } },
		{ {  1.0f, -1.0f, -1.0f }, {  0.0f, -1.0f,  0.0f } },

		// front four points
		{ {  1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },
		{ { -1.0f,  1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },
		{ { -1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },
		{ {  1.0f, -1.0f,  1.0f }, {  0.0f,  0.0f,  1.0f } },

		// back four points
		{ {  1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
		{ { -1.0f,  1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
		{ { -1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
		{ {  1.0f, -1.0f, -1.0f }, {  0.0f,  0.0f, -1.0f } },
	};  

	// input layout object
	input_layout_t input_layout;
	input_layout.add(format_t::float3, semantic_t::position);
	input_layout.add(format_t::float3, semantic_t::normal);

	// create vertex buffer
	vertex_buffer_t vertex_buffer{ "vertex buffer", std::move(input_layout), cubic_geom };
	std::copy(vertices, vertices + cubic_geom, vertex_buffer.begin<vertex_data>());

	// index buffer data
	int indicis[] =
	{
		0, 1, 2,
		2, 3, 0,
		4, 7, 6,
		6, 5, 4,
		11, 10, 9,
		9, 8, 11,
		15, 12, 13,
		13, 14, 15,
		16, 17, 18,
		18, 19, 16,
		21, 20, 23,
		23, 22, 21,
	};

	// create index buffer
	index_buffer_t index_buffer{ "index buffer", primative_t::triangle_list, format_t::int_, cubic_vertices };
	std::copy(indicis, indicis + cubic_vertices, index_buffer.ptr_as<int32_t>());
}

// test constant buffer
struct global_cbuffer
{
	float3		light_position;
	float4		ambient_color;
	float4		diffuse_color;
	float4x4	model_view;
	float3		light_direction;
};

BOOST_FUSION_ADAPT_STRUCT(
	global_cbuffer,
	(float3,	light_position)
	(float4,	ambient_color)
	(float4,	diffuse_color)
	(float4x4,	model_view)
	(float3,	light_direction)
	);

void test_constant_buffer()
{
	structure_layout_t layout = wrap_large_class<global_cbuffer>();
	constant_buffer_t cbuffer{ "global cbuffer", std::move(layout) };
}

int main(void)
{
	test_vertex_buffer_and_index_buffer();
	test_constant_buffer();

	return 0;
}