#pragma once

#include "buffer.hpp"

namespace leaves { namespace pipeline 
{
	template <>
	struct buffer_traits<index_buffer>
	{
		static constexpr object_type type() noexcept
		{
			return object_type::index_buffer;
		}
	};

	class index_buffer : public buffer<index_buffer>
	{
	public:
		using base_type = buffer<index_buffer>;
		
		template <typename T>
		using iterator = stream_buffer_iterator<T>;

	public:
		// construct
		index_buffer(string&& name, primitive_type primitive, data_format format, size_t primitive_count)
			: base_type(std::move(name), 
				detail::size_of(format), 
				detail::count_of(primitive, primitive_count),
				device_access::none, device_access::read)
			, primitive_(primitive)
			, format_(format)
			, primitive_count_(primitive_count)
		{
			throw_if_invalid_format(format);
			allocate();
		}

		// attribute access
		primitive_type primitive() const noexcept
		{
			return primitive_;
		}

		data_format format() const noexcept
		{
			return format_;
		}

		size_t primitive_count() const noexcept
		{
			return primitive_count_;
		}

		// attribute modify
		void resize(primitive_type primitive, data_format format, size_t primitive_count)
		{
			throw_if_invalid_format(format);
			base_type::resize(detail::size_of(format), detail::count_of(primitive, primitive_count));

			primitive_ = primitive;
			format_ = format;
			primitive_count_ = primitive_count;
		}

		void resize(primitive_type primitive)
		{
			resize(primitive, format_, primitive_count_);
		}

		void resize(data_format format)
		{
			resize(primitive_, format, primitive_count_);
		}

		void resize(size_t primitive_count)
		{
			resize(primitive_, format_, primitive_count);
		}

		template <typename T>
		auto begin()
		{
			static_assert(std::is_integral<T>::value, "Can only be integral type!");

			if (std::is_signed<T>::value != detail::is_signed(format_) &&
				sizeof(T) != detail::size_of(format_))
				throw std::exception{};

			return reinterpret_cast<T*>(data());
		}

		template <typename T>
		auto end()
		{
			static_assert(std::is_integral<T>::value, "Can only be integral type!");

			if (std::is_signed<T>::value != detail::is_signed(format_) &&
				sizeof(T) != detail::size_of(format_))
				throw std::exception{};

			return reinterpret_cast<T*>(data() + size());
		}

	private:
		static void throw_if_invalid_format(data_format format)
		{
			if (format != data_format::int_ && format != data_format::uint)
				throw std::exception();
		}

	private:
		primitive_type		primitive_;
		data_format			format_;
		size_t				primitive_count_;
	};
} }