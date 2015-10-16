#pragma once

#include "buffer.hpp"

namespace leaves { namespace pipeline
{
	// input layout
	class input_layout
	{
	public:
		struct attribute
		{
			data_format		format;
			data_semantic	semantic;
			size_t			offset;
		};

		static constexpr size_t attribute_count = 16;
		using attribute_container = std::vector<attribute>;

	public:

		input_layout()
		{
			attributes_.reserve(attribute_count);
		}

		void add(data_format format, data_semantic semantic) noexcept
		{
			size_t offset = 0;
			if (!attributes_.empty())
			{
				auto const& back = attributes_.back();
				offset = back.offset + detail::size_of(back.format);
			}
			attributes_.push_back({ format, semantic, offset });
		}

		decltype(auto) get(size_t index) const
		{
			return attributes_[index];
		}

		decltype(auto) find(data_semantic semantic) const
		{
			return *std::find_if(attributes_.begin(), attributes_.end(), 
				[semantic](attribute const& attr)
			{
				return semantic == attr.semantic;
			});
		}

		size_t size() const noexcept
		{
			return std::accumulate(attributes_.begin(), attributes_.end(), size_t{0}, [](size_t sum, attribute const& attr)
			{
				return sum + detail::size_of(attr.format);
			});
		}

	private:
		attribute_container	attributes_;
	};

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

		template <typename T>
		auto ptr_as()
		{
			using type = std::add_pointer_t<T>;
			return const_cast<type>(const_cast<vertex_buffer const*>(this)->ptr_as<T>());
		}

		template <typename T>
		auto ptr_as() const
		{
			static_assert(!std::is_reference<T>::value, "Can`t be cast to a reference type");

			using under = std::remove_cv_t<std::remove_pointer<T>>;
			using type = std::add_pointer_t<std::add_const_t<T>>;

			if (sizeof(T) != elem_size())
				throw std::exception{};
			
			return reinterpret_cast<type>(data());
		}

	private:
		input_layout	layout_;
	};
} }