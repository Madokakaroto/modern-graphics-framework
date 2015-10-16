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
		using traits_type = buffer_traits<index_buffer>;

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

		// content access
		template <typename T>
		auto ptr_as()
		{
			using type = std::add_pointer_t<T>;
			return const_cast<type>(const_cast<index_buffer const*>(this)->ptr_as<T>());
		}

		template <typename T, typename = std::enable_if_t<std::is_integral<T>::value> >
		auto ptr_as() const
		{
			using under = std::remove_cv_t<std::remove_pointer<T>>;
			using type = std::add_pointer_t<std::add_const_t<T>>;

			if (sizeof(T) != elem_size())
				throw std::exception{};

			return reinterpret_cast<type>(data());
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