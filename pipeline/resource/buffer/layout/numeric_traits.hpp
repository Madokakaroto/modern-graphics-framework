#pragma once

namespace leaves { namespace pipeline 
{
	namespace detail
	{
		template <typename T>
		struct numeric_traits_base;

		template <>
		struct numeric_traits_base<float>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float_;
			}
		};

		template <>
		struct numeric_traits_base<float2>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float2;
			}
		};

		template <>
		struct numeric_traits_base<float3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float3;
			}
		};

		template <>
		struct numeric_traits_base<float4>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4;
			}
		};

		template <>
		struct numeric_traits_base<float3x3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float3x3;
			}
		};

		template <>
		struct numeric_traits_base<float4x3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4x3;
			}
		};

		template <>
		struct numeric_traits_base<float4x4>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4x4;
			}
		};
	}

	template <typename T>
	struct numeric_traits : detail::numeric_traits_base<T>
	{
		static constexpr uint16 count()
		{
			return 1;
		}
	};

	template <typename T, size_t N>
	struct numeric_traits<T[N]> : detail::numeric_traits_base<T>
	{
		static constexpr uint16 count()
		{
			return N;
		}
	};
} }