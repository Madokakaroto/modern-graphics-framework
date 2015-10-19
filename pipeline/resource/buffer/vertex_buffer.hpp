#pragma once

#include "buffer.hpp"
#include "layout\input_layout.hpp"

namespace leaves { namespace pipeline
{
	template <>
	struct buffer_traits<vertex_buffer>
	{
		static constexpr object_type type() noexcept
		{
			return object_type::vertex_buffer;
		}
	};

	class vertex_buffer : public buffer<vertex_buffer>
	{
	public:
		using base_type = buffer<vertex_buffer>;

		template <typename T>
		class iterator
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
			iterator(byte* data, byte* end, size_t offset, std::ptrdiff_t stride)
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

			iterator& operator++() // preincrement
			{
				data_ = throw_if_range_error(offset_);
				return *this;
			}

			iterator operator++(int) // postincrement
			{
				iterator tmp = *this;
				++*this;
				return tmp;
			}

			iterator& operator--() // predecrement
			{
				data_ = throw_if_range_error(-stride_);
				return *this;
			}

			iterator operator--(int) // postdecrement
			{
				iterator tmp = *this;
				--*this;
				return tmp;
			}

			iterator& operator += (int count)
			{
				data_ = throw_if_range_error(count * stride_);
				return *this;
			}

			iterator operator+(int count)
			{
				iterator tmp = *this;
				return tmp += count;
			}

			iterator& operator -= (int count)
			{
				return *this += (-count);
			}

			iterator operator-(int count)
			{
				iterator tmp = *this;
				return tmp -= count;
			}

			bool operator== (iterator const& rhs) const noexcept
			{
				// not safe but effective
				return data_ == rhs.data_;
			}

			bool operator!= (iterator const& rhs) const noexcept
			{
				return !(*this == rhs);
			}

			bool operator< (iterator const& rhs) const noexcept
			{
				return data_ < rhs.data_;
			}

		private:
			void throw_if_access_violate() const
			{
				if (nullptr == data_ || data_ >= end_ || data_ < begin_)
					throw std::exception{"access violation"};
			}

			byte* throw_if_range_error(std::ptrdiff_t diff) const
			{
				auto tmp = data_ + diff;
				if (nullptr == data_ || tmp > end_ || tmp < begin_)
					throw std::range_error{"range error"};
				return tmp;
			}

		private:
			byte*			data_;
			byte*			begin_;
			byte*			end_;
			difference_type	stride_;
		};

	public:
		// constructor
		vertex_buffer(string&& name, input_layout&& layout, size_t count)
			: base_type(std::move(name), layout.size(), count, device_access::none, device_access::read)
			, layout_(std::move(layout))
		{
		}

		// attribute access
		input_layout const& layout() const noexcept
		{
			return layout_;
		}

		// attribute modify
		void resize(input_layout&& meta_data, size_t count)
		{
			auto elem_size = meta_data.size();
			base_type::resize(elem_size, count);
			layout_ = std::move(meta_data);
		}

		// content access
		template <typename T>
		auto begin(data_semantic semantic)
		{
			auto& attr = layout_.find(semantic);

			if (sizeof(T) != size_of(attr.format))
				throw std::exception{};

			using iterator_type = iterator<T>;
			return iterator_type{ data(), data() + size(), attr.offset, static_cast<std::ptrdiff_t>(elem_size()) };
		}

		template <typename T>
		auto end(data_semantic semantic)
		{
			auto& attr = layout_.find(semantic);

			if (sizeof(T) != size_of(attr.format))
				throw std::exception{};

			using iterator_type = iterator<T>;
			auto ptr = data() + size();
			return iterator_type{ ptr, ptr, attr.offset, static_cast<std::ptrdiff_t>(elem_size()) };
		}

		template <typename T>
		auto begin()
		{
			if(sizeof(T) != elem_size())
				throw std::exception{};

			using iterator_type = iterator<P>;
			return iterator_type{ data(), data() + size(), 0, sizeof(T) };
		}

		template <typename T>
		auto end()
		{
			if (sizeof(T) != elem_size())
				throw std::exception{};

			using iterator_type = iterator<T>;
			auto ptr = data() + size();
			return iterator_type{ ptr, ptr, 0, sizeof(T) };
		}

	private:
		input_layout	layout_;
	};
} }