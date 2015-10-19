#pragma once

namespace leaves { namespace pipeline
{
	namespace detail
	{
		uint16_t size_of(pixel_format format) noexcept
		{
			return 0;
		}

		uint16_t size_of(data_format format) noexcept
		{
			switch (format)
			{
			case data_format::float_:
			case data_format::int_:
			case data_format::uint:
				return 4;
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
			case data_format::float2x2:
			case data_format::float3x2:
			case data_format::float4x2:
				return 32;
			case data_format::float2x3:
			case data_format::float3x3:
			case data_format::float4x3:
				return 48;
			case data_format::float2x4:
			case data_format::float3x4:
			case data_format::float4x4:
				return 64;
			default:
				return 0;
			}
		}

		static size_t count_of(primitive_type primitive, size_t primitive_count) noexcept
		{
			switch (primitive)
			{
			case primitive_type::line_strip:
				return primitive_count;
			case primitive_type::line_list:
				return primitive_count * 2;
			case primitive_type::triangle_list:
				return primitive_count * 3;
			case primitive_type::triangle_strip:
			case primitive_type::line_strip_adj:
				return primitive_count + 2;
			case primitive_type::line_list_adj:
				return primitive_count * 4;
			case primitive_type::triangle_list_adj:
				return primitive_count * 6;
			case primitive_type::triangle_strip_adj:
				return primitive_count * 2 + 4;
			default: //primitive_type::point_list:
				return primitive_count;
			}
		}

		static uint16_t aligned_size(data_format format, uint16_t count, uint16_t size = 0) noexcept
		{
			return 0;
		}
	}
} }