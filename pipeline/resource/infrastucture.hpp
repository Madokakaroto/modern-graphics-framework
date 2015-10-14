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
		int_,
		int2,
		int3,
		int4,
		uint,
		uint2,
		uint3,
		uint4
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

	size_t size_of(pixel_format format) noexcept
	{
		return 0;
	}

	size_t size_of(data_format format) noexcept
	{
		switch (format)
		{
		case data_format::float2:
		case data_format::int2:
		case data_format::uint2:
			return 8;
		case data_format::float3:
		case data_format::int3:
		case data_format::uint3:
			return 12;
		case data_format::float4:
		case data_format::int4:
		case data_format::uint4:
			return 16;
		default:
			return 4;
		}
	}
} }