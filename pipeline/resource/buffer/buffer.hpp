#pragma once

#include "..\resource.hpp"

namespace leaves { namespace pipeline 
{
	template <typename Impl>
	class buffer : public resource
	{
		using traits_type = buffer_traits<Impl>;

	protected:
		buffer(
			string&& name,				// resource name 
			size_t elem_size,			// element size 
			size_t elem_count,			// element count
			device_access cpu_access,	// host access
			device_access gpu_access	// device access
			)
			: resource(traits_type::type(), std::move(name), elem_size * elem_count, cpu_access, gpu_access)
			, element_size_(elem_size)
			, element_count_(elem_count)
		{
		}

		void resize(size_t elem_size, size_t elem_count)
		{
			if (elem_size == element_size_ && elem_count == element_count_)
				return;

			if (elem_size * elem_count != size())
				resource::resize(elem_size * elem_count);

			element_size_ = elem_size;
			element_count_ = elem_count;
		}

	public:
		size_t elem_size() const
		{
			return element_size_;
		}

		size_t elem_count() const
		{
			return element_count_;
		}

		template <typename T>
		auto ptr_as()
		{
			using type = std::add_pointer_t<T>;
			return const_cast<type>(const_cast<buffer const*>(this)->ptr_as<T>());
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
		size_t		element_size_;
		size_t		element_count_;
	};
} }