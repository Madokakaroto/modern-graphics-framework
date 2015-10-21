#pragma once

namespace leaves { namespace pipeline 
{
	template <typename T>
	class stream_buffer_iterator
	{
	public:
		// adapt to stl
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using reference = value_type&;
		using const_reference = value_type const&;
		using pointer = T*;
		using const_pointer = T const*;
		using difference_type = std::ptrdiff_t;

	public:
		stream_buffer_iterator(byte* data, byte* end, size_t offset, std::ptrdiff_t stride)
			: data_(data + offset)
			, begin_(data_)
			, end_(end + offset)
			, stride_(offset)
		{

		}

		T& operator*()
		{
			throw_if_access_violate();
			return *reinterpret_cast<T*>(data_);
		}

		stream_buffer_iterator& operator++() // preincrement
		{
			data_ = throw_if_range_error(stride_);
			return *this;
		}

		stream_buffer_iterator operator++(int) // postincrement
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}

		stream_buffer_iterator& operator--() // predecrement
		{
			data_ = throw_if_range_error(-stride_);
			return *this;
		}

		stream_buffer_iterator operator--(int) // postdecrement
		{
			iterator tmp = *this;
			--*this;
			return tmp;
		}

		stream_buffer_iterator& operator += (int count)
		{
			data_ = throw_if_range_error(count * stride_);
			return *this;
		}

		stream_buffer_iterator operator+(int count)
		{
			iterator tmp = *this;
			return tmp += count;
		}

		stream_buffer_iterator& operator -= (int count)
		{
			return *this += (-count);
		}

		stream_buffer_iterator operator-(int count)
		{
			iterator tmp = *this;
			return tmp -= count;
		}

		bool operator== (stream_buffer_iterator const& rhs) const noexcept
		{
			// not safe but effective
			return data_ == rhs.data_;
		}

		bool operator!= (stream_buffer_iterator const& rhs) const noexcept
		{
			return !(*this == rhs);
		}

		bool operator< (stream_buffer_iterator const& rhs) const noexcept
		{
			return data_ < rhs.data_;
		}

	private:
		void throw_if_access_violate() const
		{
			if (nullptr == data_ || data_ >= end_ || data_ < begin_)
				throw std::exception{ "access violation" };
		}

		byte* throw_if_range_error(std::ptrdiff_t diff) const
		{
			auto tmp = data_ + diff;
			if (nullptr == data_ || tmp > end_ || tmp < begin_)
				throw std::range_error{ "range error" };
			return tmp;
		}

	private:
		byte*			data_;
		byte*			begin_;
		byte*			end_;
		difference_type	stride_;
	};
} }