#pragma once

namespace leaves { namespace pipeline
{
	namespace detail
	{
		template <size_t N, typename T>
		constexpr static T align(T input) noexcept
		{
			//return (size & 0xFFF0) + (((size & 0x000F) != 0) << 4);
			return ((input + N - 1) / N) * N;
		}

		uint16_t size_of(pixel_format format) noexcept
		{
			switch (format)
			{
			case pixel_format::bc1:
				return 0;
			case pixel_format::rgba_32:
				return 4;
			default:
				assert(false);
				return 0;
			}
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
				assert(false);
				return 0;
			}
		}

		static size_t count_of(primitive_type primitive, size_t primitive_count) noexcept
		{
			switch (primitive)
			{
			case primitive_type::point_list:
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
			default: //control points n
				assert(false);
				return 0;
			}
		}

		static uint16_t size_of(data_format format, uint16_t count, uint16_t size = 0) noexcept
		{
			auto data_size = size_of(format);

			if (data_format::structured == format)
				data_size = size;
			
			if (count > 1)
				data_size = align<16>(size) * count;

			return data_size;
		}

		static uint16_t reg_size(data_format format) noexcept
		{
			switch (format)
			{
			case data_format::int_:
			case data_format::uint:
			case data_format::float_:
				return 1;
			case data_format::int2:
			case data_format::uint2:
			case data_format::float2:
				return 2;
			case data_format::int3:
			case data_format::uint3:
			case data_format::float3:
				return 3;
			default:
				return 4;
			}
		}

		static bool is_signed(data_format format) noexcept
		{
			switch (format)
			{
			case data_format::uint:
			case data_format::uint2:
			case data_format::uint3:
			case data_format::uint4:
				return false;
			default:
				return true;
			}
		}
	}

	namespace detail
	{
		template <size_t Reg, size_t Offset>
		struct structure_size_helper
		{
			static constexpr size_t reg = Reg;
			static constexpr size_t offset = Offset;
		};

		template <typename Helper, typename ... Args>
		struct structure_size_impl_expand_impl;

		template <typename Helper, typename T>
		struct export_calculate_result
		{
			using traits_t = array_traits<T>;
			using numeric_traits_t = typename traits_t::numeric_traits_t;
			using helper_t = Helper;

			static constexpr size_t count = traits_t::count;
			static constexpr size_t numeric_size = count > 1 ? 
				align<16>(numeric_traits_t::size()) * count : numeric_traits_t::size();
			static constexpr size_t numeric_reg = numeric_traits_t::reg();

			static constexpr bool new_four_component =
				(count > 1) || (helper_t::reg + numeric_reg >= 4);

			static constexpr size_t new_reg = new_four_component ?
				0 : (helper_t::reg + numeric_reg) % 4;
			static constexpr size_t new_size = new_four_component ?
				align<16>(helper_t::offset) + numeric_size : helper_t::offset + numeric_size;
		};

		template <typename Helper, typename First, typename ... Rests>
		struct structure_size_impl_expand_impl<Helper, First, Rests...> : export_calculate_result<Helper, First>
		{
			using next_helper = structure_size_helper<new_reg, new_size>;
			using next_type = structure_size_impl_expand_impl<next_helper, Rests...>;
			static constexpr size_t value = next_type::value;
		};

		template <typename Helper, typename Last>
		struct structure_size_impl_expand_impl<Helper, Last>
			: export_calculate_result<Helper, Last>
		{
			static constexpr size_t value = align<16>(new_size);
		};

		template <typename T, typename Indices>
		struct structure_size_impl;

		template <typename ... Args>
		struct structure_size_impl_expand
		{
			static constexpr size_t value = structure_size_impl_expand_impl
				<structure_size_helper<0,0>, Args...>::value;
		};

		template <typename T, size_t ... Is>
		struct structure_size_impl<T, std::index_sequence<Is...>>
		{
			static constexpr size_t value = structure_size_impl_expand<
				type_at<T, Is>...>::value;
		};
	}

	template <typename T>
	struct structure_size
	{
		static_assert(is_sequence<T>::value, "!!!");
		static constexpr size_t size = sequence_size<T>::value;

		static constexpr size_t value = detail::structure_size_impl<
			T, std::make_index_sequence<size>>::value;
	};
} }