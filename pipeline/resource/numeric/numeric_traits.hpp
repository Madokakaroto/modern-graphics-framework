#pragma once

namespace leaves { namespace pipeline
{
	template <typename T>
	struct numeric_traits;

	template <>
	struct numeric_traits<float>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float_;
		}

		static constexpr size_t reg() noexcept
		{
			return 1;
		}

		static constexpr size_t size() noexcept
		{
			return 4;
		}
	};

	template <>
	struct numeric_traits<float2>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float2;
		}

		static constexpr size_t reg() noexcept
		{
			return 2;
		}

		static constexpr size_t size() noexcept
		{
			return 8;
		}
	};

	template <>
	struct numeric_traits<float3>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float3;
		}

		static constexpr size_t reg() noexcept
		{
			return 3;
		}

		static constexpr size_t size() noexcept
		{
			return 12;
		}
	};

	template <>
	struct numeric_traits<float4>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float4;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 16;
		}
	};

	template <>
	struct numeric_traits<float2x2>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float2x2;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 32;
		}
	};

	template <>
	struct numeric_traits<float2x3>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float2x3;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 32;
		}
	};

	template <>
	struct numeric_traits<float2x4>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float2x4;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 32;
		}
	};

	template <>
	struct numeric_traits<float3x2>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float3x2;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 48;
		}
	};

	template <>
	struct numeric_traits<float3x3>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float3x3;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 48;
		}
	};

	template <>
	struct numeric_traits<float3x4>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float3x4;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 48;
		}
	};

	template <>
	struct numeric_traits<float4x2>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float4x2;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 64;
		}
	};

	template <>
	struct numeric_traits<float4x3>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float4x3;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 64;
		}
	};

	template <>
	struct numeric_traits<float4x4>
	{
		static constexpr data_format format() noexcept
		{
			return data_format::float4x4;
		}

		static constexpr size_t reg() noexcept
		{
			return 4;
		}

		static constexpr size_t size() noexcept
		{
			return 64;
		}
	};
} }