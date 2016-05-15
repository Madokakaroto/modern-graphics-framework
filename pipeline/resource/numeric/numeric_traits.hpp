#pragma once

namespace leaves { namespace pipeline
{
	namespace detail
	{
		constexpr uint16_t ceil_to_mutiply_of_16(uint16_t const input) noexcept
		{
			return ((input + 15) / 16) * 16;
		}

		template <typename T, bool IsStruct>
		struct numeric_traits;

		template <typename T>
		struct numeric_traits_base;

		template <typename T>
		struct numeric_traits<T, false> : numeric_traits_base<T>
		{

		};

		template <typename T>
		struct numeric_traits<T, true>
		{

		};

		template <>
		struct numeric_traits_base<float>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float_;
			}

			static constexpr uint16_t size() noexcept
			{
				return 4;
			}
		};

		template <>
		struct numeric_traits_base<float2>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float2;
			}

			static constexpr uint16_t size() noexcept
			{
				return 8;
			}
		};

		template <>
		struct numeric_traits_base<float3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float3;
			}

			static constexpr uint16_t size() noexcept
			{
				return 12;
			}
		};

		template <>
		struct numeric_traits_base<float4>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4;
			}

			static constexpr uint16_t size() noexcept
			{
				return 16;
			}
		};

		template <>
		struct numeric_traits_base<float2x2>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float2x2;
			}

			static constexpr uint16_t size() noexcept
			{
				return 32;
			}
		};

		template <>
		struct numeric_traits_base<float2x3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float2x3;
			}

			static constexpr uint16_t size() noexcept
			{
				return 48;
			}
		};

		template <>
		struct numeric_traits_base<float2x4>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float2x4;
			}

			static constexpr uint16_t size() noexcept
			{
				return 64;
			}
		};

		template <>
		struct numeric_traits_base<float3x2>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float3x2;
			}

			static constexpr uint16_t size() noexcept
			{
				return 32;
			}
		};

		template <>
		struct numeric_traits_base<float3x3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float3x3;
			}

			static constexpr uint16_t size() noexcept
			{
				return 48;
			}
		};

		template <>
		struct numeric_traits_base<float3x4>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float3x4;
			}

			static constexpr uint16_t size() noexcept
			{
				return 64;
			}
		};

		template <>
		struct numeric_traits_base<float4x2>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4x2;
			}

			static constexpr uint16_t size() noexcept
			{
				return 16;
			}
		};

		template <>
		struct numeric_traits_base<float4x3>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4x3;
			}

			static constexpr uint16_t size() noexcept
			{
				return 48;
			}
		};

		template <>
		struct numeric_traits_base<float4x4>
		{
			static constexpr data_format format() noexcept
			{
				return data_format::float4x4;
			}

			static constexpr uint16_t size() noexcept
			{
				return 64;
			}
		};
	}

	template <typename T>
	struct numeric_traits : detail::numeric_traits<T, boost::fusion::traits::is_sequence<T>::value>
	{
		static constexpr uint16_t count() noexcept
		{
			return 1;
		}

		static constexpr uint16_t size() noexcept
		{

		}
	};

	template <typename T, size_t N>
	struct numeric_traits<T[N]>
	{
		static_assert(N != 1, "...");
		using element_numeric_tratis = detail::numeric_traits_base<T>;

		static constexpr uint16_t count() noexcept
		{
			return N;
		}

		static constexpr uint16_t size() noexcept
		{
			return detail::ceil_to_mutiply_of_16(element_numeric_tratis::size()) * count();
		}
	};

	template <typename T, size_t N>
	struct numeric_traits<std::array<T, N>> : numeric_traits<T[N]>
	{
	};
} }