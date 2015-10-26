#pragma once

#include "numeric_traits.hpp"

namespace leaves { namespace pipeline 
{
	class variable_layout
	{
		using sub_var_container = std::vector<variable_layout>;

	public:
		// construct
		variable_layout(data_format format, uint16_t count, uint16_t size, uint16_t offset)
			: format_(format)
			, count_(count)
			, size_(size)
			, offset_(offset)
		{

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