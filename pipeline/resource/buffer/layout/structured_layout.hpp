#pragma once

#include "numeric_variable.hpp"

namespace leaves { namespace pipeline 
{
	namespace detail
	{
		template <typename T>
		struct large_class_wrapper
		{
		};
	}

	template <typename T>
	auto wrap_large_class()
	{
		return detail::large_class_wrapper<typename T::tuple_type>{};
	}

	class structured_layout
	{
		using var_container = std::vector<variable_layout>;
		static constexpr size_t attribute_count = 8;

		struct offset_reg
		{
			uint16_t offset;
			uint16_t reg;
		};

	public:
		structured_layout()
		{
			variables_.reserve(attribute_count);
		}

		// for simple use
		template <typename T>
		structured_layout(detail::large_class_wrapper<T> tuple)
			: structured_layout()
		{
			init_from_tuple(tuple, std::make_index_sequence<std::tuple_size<T>::value>{});
		}

		void add(data_format format, uint16_t count, uint16_t size, uint16_t offset)
		{
			variables_.emplace_back(format, count, size, offset);
		}

		uint16_t size() const noexcept
		{
			if (variables_.empty())
				return 0;

			auto& back = variables_.back();
			return detail::align(back.offset() + back.size());
		}

		variable_layout& operator[] (size_t index)
		{
			return variables_[index];
		}

		variable_layout const& operator[] (size_t index) const
		{
			return variables_[index];
		}

	private:

		template <typename T, size_t ... Is>
		void init_from_tuple(detail::large_class_wrapper<T> tuple, std::index_sequence<Is...> seq)
		{
			offset_reg helper = { 0, 0 };

			using swallow_t = bool[];
			swallow_t s = { (add_numeric<std::tuple_element_t<Is, T>>(helper), true)... };
		}

		template <typename T>
		void add_numeric(offset_reg& helper)
		{
			using traits_type = numeric_traits<T>;

			uint16_t reg = detail::reg_size(traits_type::format(), traits_type::count());
			// if the rest register count in the four component vector is not enough
			if (reg + helper.reg > 4 && 0 != helper.reg)
			{
				// begin with a new four component vector
				helper.offset = detail::align(helper.offset);
				helper.reg = 0;
			}

			// calculate size
			auto size = detail::size_of(traits_type::format(), traits_type::count());
			// add to container
			add(traits_type::format(), traits_type::count(), size, helper.offset);

			// update intermediate variables
			helper.offset += size;
			helper.reg += reg & 0x03;
		}

	private:
		var_container	variables_;
	};
} }