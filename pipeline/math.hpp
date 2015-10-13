#pragma once

#include <cml/cml.h>

namespace leaves
{
	using float2 = cml::vector<float, cml::fixed<2>>;
	using float3 = cml::vector<float, cml::fixed<3>>;
	using float4 = cml::vector<float, cml::fixed<4>>;

	using float3x3 = cml::matrix<float, cml::fixed<3, 3>, cml::row_basis, cml::col_major>;
	using float4x3 = cml::matrix<float, cml::fixed<4, 3>, cml::row_basis, cml::col_major>;
	using float4x4 = cml::matrix<float, cml::fixed<4, 4>, cml::row_basis, cml::col_major>;

	template <typename T>
	auto log2_integral(T t)
		-> std::enable_if_t<std::is_integral<T>::value, T>
	{
		T log2 = (t & 0xAAAAAAAAu) != 0;
		log2 |= ((t & 0xFFFF0000u) != 0) << 4;
		log2 |= ((t & 0xFF00FF00u) != 0) << 3;
		log2 |= ((t & 0xF0F0F0F0u) != 0) << 2;
		log2 |= ((t & 0xCCCCCCCCu) != 0) << 1;
		return log2;
	}
}