#pragma once

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

		decltype(auto) operator[] (size_t index)
		{
			return sub_variables_[index];
		}

		decltype(auto) operator[] (size_t index) const
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
} }