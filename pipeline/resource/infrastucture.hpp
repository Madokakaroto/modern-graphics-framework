#pragma once

namespace leaves { namespace pipeline
{
	enum class pixel_format
	{
		unknown,
		rgba_32,
		bc1,
	};

	enum class data_format
	{
		float_,
		float2,
		float3,
		float4,
		float2x2,
		float2x3,
		float2x4,
		float3x2,
		float3x3,
		float3x4,
		float4x2,
		float4x3,
		float4x4,
		int_,
		int2,
		int3,
		int4,
		uint,
		uint2,
		uint3,
		uint4,
		structured,
	};

	enum class device_access
	{
		none,
		read,
		write,
		read_write,
	};

	enum class data_semantic
	{
		position,
		normal,
		binormal,
		tangent,
		texcoord0,
		texcoord1,
		texcoord2,
		texcoord3,
		texcoord4,
		texcoord5,
		texcoord6,
		texcoord7,
		color0,
		color1,
	};

	enum class primitive_type
	{
		point_list,
		line_list,
		line_strip,
		triangle_list,
		triangle_strip,
		line_list_adj,
		line_strip_adj,
		triangle_list_adj,
		triangle_strip_adj,
		control_point_1,
		control_point_2,
		control_point_3,
		control_point_4,
		control_point_5,
		control_point_6,
		control_point_7,
		control_point_8,
		control_point_9,
		control_point_10,
		control_point_11,
		control_point_12,
		control_point_13,
		control_point_14,
		control_point_15,
		control_point_16,
		control_point_17,
		control_point_18,
		control_point_19,
		control_point_20,
		control_point_21,
		control_point_22,
		control_point_23,
		control_point_24,
		control_point_25,
		control_point_26,
		control_point_27,
		control_point_28,
		control_point_29,
		control_point_30,
		control_point_31,
		control_point_32,
	};
} }