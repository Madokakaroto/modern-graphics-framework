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
			auto offset = attributes_.empty() ? 0 : attributes_.back().offset + size_of(format);
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
				return sum + size_of(attr.format);
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
			static_assert(std::is_pod<T>::value, "");
		public:
			iterator(byte* data, byte* end, size_t offset)
				: data_(data + offset)
				, end_(end + offset)
				, offset_(offset)
			{

			}
			
			T& operator*()
			{
				throw_if_overflow()
				return *reinterpret_cast<T*>(data_);
			}

			iterator& operator++() // preincrement
			{
				throw_if_overflow();
				data_ += offset_;
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
				throw_if_overflow();

				data_ -= offset_;
				return *this;
			}

			iterator operator--(int) // predecrement
			{
				iterator tmp = *this;
				--*this;
				return tmp;
			}

		private:

			void throw_if_overflow() const
			{
				if (nullptr == data_ && data_ >= end_)
					throw std::overflow_error{};
			}

		private:
			byte*	data_;
			byte*   end_;
			size_t	offset_;
		};

	public:
		// constructor
		vertex_buffer(string&& name, input_layout&& layout, size_t count)
			: base_type(std::move(name), layout.size(), count, device_access::none, device_access::read)
			, layout_(std::move(layout))
		{
		}

		void resize(input_layout&& meta_data, size_t count)
		{
			auto elem_size = meta_data.size();
			base_type::resize(elem_size, count);
			layout_ = std::move(meta_data);
		}

		template <typename T>
		auto begin(data_semantic semantic)
		{
			decltype(auto) attr = layout_.find(semantic);

			if (sizeof(T) != size_of(attr.format))
				throw std::exception{};

			using iterator_type = iterator<T>;
			return iterator_type{ data(), data() + size(), attr.offset };
		}

		template <typename T>
		auto end(data_semantic semantic)
		{
			decltype(auto) attr = layout_.find(semantic);

			if (sizeof(T) != size_of(attr.format))
				throw std::exception{};

			using iterator_type = iterator<T>;
			auto ptr = data() + size();
			return iterator_type{ ptr, ptr, attr.offset };
		}

	private:
		input_layout	layout_;
	};
} }