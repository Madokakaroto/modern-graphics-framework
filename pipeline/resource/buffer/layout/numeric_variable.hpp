#pragma once

#include "numeric_traits.hpp"
#include <pipeline\large_class.hpp>

namespace leaves { namespace pipeline 
{
	namespace detail
	{
		struct offset_register
		{
			uint16_t offset;
			uint16_t reg;
		};

		template <typename T>
		void bind_numeric(variable_layout& layout, offset_register& helper)
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
			layout.add_sub(traits_type::format(), traits_type::count(), size, helper.offset);

			// update intermediate variables
			helper.offset += size;
			helper.reg += reg & 0x03;
		}

		template <typename T, size_t ... Is>
		void init_variable_layout_from_tuple(variable_layout& layout, detail::large_class_wrapper<T> tuple, std::index_sequence<Is...> seq)
		{
			offset_register helper = { 0, 0 };
			using swallow_t = bool[];

			swallow_t s = { (bind_numeric<std::tuple_element_t<Is, T>>(layout, helper), true)... };
		}

		template <typename T>
		void init_variable_layout_from_tuple(variable_layout& layout, detail::large_class_wrapper<T> tuple)
		{
			init_variable_layout_from_tuple(layout, tuple, std::make_index_sequence<std::tuple_size<T>::value>{});
		}
	}

	// numeric layout
	class variable_layout
	{
		using sub_var_container = std::vector<variable_layout>;

		static constexpr size_t sub_count = 8;

	public:

		// construct
		variable_layout(data_format format, uint16_t count, uint16_t size, uint16_t offset)
			: format_(format)
			, count_(count)
			, size_(size)
			, offset_(offset)
		{
			sub_variables_.reserve(sub_count);
		}

		template <typename T>
		variable_layout(detail::large_class_wrapper<T> tuple)
			: variable_layout(data_format::structured, 1, 0, 0)
		{
			detail::init_variable_layout_from_tuple(*this, tuple);
		}

		// attribute access
		data_format format() const noexcept
		{
			return format_;
		}

		uint16_t count() const noexcept
		{
			return count_;
		}

		uint16_t size() const noexcept
		{
			return size_;
		}

		uint16_t offset() const noexcept
		{
			return offset_;
		}

		// sub variables
		void add_sub(data_format format, uint16_t count, uint16_t size, uint16_t offset)
		{
			assert(data_format::structured != format_);
			assert(offset + size <= size_);

			sub_variables_.emplace_back(format, count, size, offset);
		}

		variable_layout& operator[] (size_t index)
		{
			return sub_variables_[index];
		}

		variable_layout const& operator[] (size_t index) const
		{
			return sub_variables_[index];
		}

	private:
		data_format			format_;
		uint16_t			count_;
		uint16_t			size_;
		uint16_t			offset_;
		sub_var_container	sub_variables_;
	};

	// numeric variable
	struct numeric_variable
	{
	public:
		numeric_variable(variable_layout const& layout, byte* ptr) noexcept
			: layout_(layout)
			, ptr_(ptr)
		{
		}

		template <typename T>
		numeric_variable& operator= (T const& t)
		{
			this->as<T>() = t;
			return *this;
		}

		template <typename T>
		decltype(auto) as()
		{
			using type = std::add_rvalue_reference_t<T>;
			return const_cast<type>(const_cast<numeric_variable const&>(*this).as<T>());
		}

		template <typename T>
		decltype(auto) as() const
		{
			static_assert(!std::is_pointer<T>::value, "Can`t a be pointer type.");
			static_assert(!std::is_reference<T>::value, "Can`t be a reference type.");

			if (sizeof(T) != layout_.size() || nullptr == ptr_)
				throw std::exception{};

			using type = std::add_pointer_t<std::add_const_t<T>>;
			return *reinterpret_cast<type>(ptr_ + layout_.offset());
		}

		numeric_variable operator[] (size_t index)
		{
			return{ layout_[index], ptr_ };
		}

	private:
		variable_layout const&	layout_;
		byte*					ptr_;
	};
} }