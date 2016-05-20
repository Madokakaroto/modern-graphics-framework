#pragma once

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
		void bind_numeric(numeric_layout& layout, offset_register& helper)
		{
			using traits_t = array_traits<T>;
			using traits_type = typename traits_t::numeric_traits_t;

			uint16_t reg = detail::reg_size(traits_type::format(), traits_t::count);
	
			// if the rest register count in the four component vector is not enough
			if (reg + helper.reg > 4 && 0 != helper.reg)
			{
				// begin with a new four component vector
				helper.offset = detail::align<16>(helper.offset);
				helper.reg = 0;
			}
	
			// calculate size
			auto size = detail::size_of(traits_type::format(), traits_t::count);
	
			// add to container
			layout.add_sub(traits_type::format(), traits_t::count, size, helper.offset);
	
			// update intermediate variables
			helper.offset += size;
			helper.reg += reg & 0x03;
		}
	
		template <typename T, size_t ... Is>
		void init_variable_layout_from_tuple(numeric_layout& layout, large_class_wrapper<T> tuple, std::index_sequence<Is...> seq)
		{
			offset_register helper = { 0, 0 };
			using swallow_t = bool[];
	
			swallow_t s = { (bind_numeric<type_at<T, Is>>(layout, helper), true)... };
		}
	
		template <typename T>
		void init_variable_layout_from_tuple(numeric_layout& layout, large_class_wrapper<T> tuple)
		{
			init_variable_layout_from_tuple(layout, tuple, std::make_index_sequence<sequence_size<T>::value>{});
		}
	}

	// numeric layout
	class numeric_layout
	{
		using sub_var_container = std::vector<numeric_layout>;
		static constexpr size_t sub_count = 8;

	public:
		// construct
		numeric_layout(data_format format, uint16_t count, uint16_t size, uint16_t offset)
			: format_(format)
			, count_(count)
			, size_(size)
			, offset_(offset)
		{
			sub_variables_.reserve(sub_count);
		}

		template <typename T>
		numeric_layout(large_class_wrapper<T> t)
			: numeric_layout(
				data_format::structured, 1, 
				structure_size<T>::value, 0)
		{
			detail::init_variable_layout_from_tuple(*this, t);
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
			//assert(data_format::structured != format_);
			assert(offset + size <= size_);

			sub_variables_.emplace_back(format, count, size, offset);
		}

		numeric_layout& operator[] (size_t index)
		{
			return sub_variables_[index];
		}

		numeric_layout const& operator[] (size_t index) const
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
	class numeric_variable
	{
	public:
		numeric_variable(numeric_layout const& layout, byte* ptr) noexcept
			: layout_(layout)
			, ptr_(ptr)
		{
		}

		numeric_variable(numeric_variable const&) = default;
		~numeric_variable() = default;

		template <typename T>
		numeric_variable& operator= (T const& t)
		{
			return *this;
		}

		numeric_variable operator[] (size_t index) const
		{
			return numeric_variable{ layout_[index], ptr_ };
		}

	private:
		numeric_layout const&	layout_;
		byte*					ptr_;
	};
} }