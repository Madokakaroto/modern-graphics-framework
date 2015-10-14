#pragma once

#include "..\resource.hpp"

namespace leaves { namespace pipeline 
{
	template <typename Impl>
	class buffer : public resource<buffer<Impl>>
	{
		using base_type = resource<buffer<Impl>>;
		using traits_type = buffer_traits<Impl>;

	public:
		buffer(
			string&& name,				// resource name 
			size_t elem_size,			// element size 
			size_t elem_count,			// element count
			device_access cpu_access,	// host access
			device_access gpu_access	// device access
			)
			: base_type(traits_type::type(), std::move(name), elem_size * elem_count, cpu_access, gpu_access)
			, element_size_(elem_size)
			, element_count_(elem_count)
		{
		}

		size_t elem_size() const
		{
			return element_size_;
		}

		size_t elem_count() const
		{
			return element_count_;
		}

	protected:

		void resize(size_t elem_size, size_t elem_count)
		{
			if (elem_size == element_size_ && elem_count == element_count_)
				return;

			if(elem_size * elem_count != size())
				base_type::resize(elem_size * elem_count);

			element_size_ = elem_size;
			element_count_ = elem_count;
		}

	private:
		size_t		element_size_;
		size_t		element_count_;
	};
} }