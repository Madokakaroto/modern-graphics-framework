#pragma once

#include <cstdint>
#include "..\object.hpp"
#include "..\math.hpp"

namespace leaves { namespace pipeline 
{
	class sampler : public object
	{
	public:
		enum class filter_mode : std::uint8_t
		{
			min_p_mag_p_mip_p,
			min_p_mag_p_mip_l,
			min_p_mag_l_mip_l,
			min_p_mag_l_mip_p,
			min_l_mag_p_mip_p,
			min_l_mag_p_mip_l,
			min_l_mag_l_mip_p,
			min_l_mag_l_mip_l,
			anisotropic,
		};

		enum class filter_ext : std::uint8_t
		{
			none,
			comparison,
			minimum,
			maximum,
		};

		enum class address_mode : std::uint8_t
		{
			wrap,
			mirror,
			clamp,
			border,
			mirror_once,
		};

		enum class cmp_func : std::uint8_t
		{
			never,
			less,
			equal,
			less_equal,
			grater,
			not_equal,
			greater_equal,
			always,
		};

	public:
		explicit sampler(string&& name)
			: object(object_type::sampler, std::move(name))
			, filter(filter_mode::min_p_mag_p_mip_p)
			, address_u(address_mode::wrap)
			, address_v(address_mode::wrap)
			, address_w(address_mode::wrap)
			, mip_level_bias(0.0f)
			, min_mip_level(0.0f)
			, max_mip_level(0.0f)
			, border_color(0.0f, 0.0f, 0.0f, 0.0f)
			, comparison(cmp_func::never)
			, filter_extend(filter_ext::none)
		{

		}

	public:
		filter_mode		filter;
		address_mode	address_u;
		address_mode	address_v;
		address_mode	address_w;
		float			mip_level_bias;
		float			min_mip_level;
		float			max_mip_level;
		float4			border_color;
		cmp_func		comparison;
		filter_ext		filter_extend;
	};
} }