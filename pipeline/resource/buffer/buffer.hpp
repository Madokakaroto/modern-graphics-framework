#pragma once

#include "..\resource.hpp"

namespace leaves { namespace pipeline 
{
	template <typename Impl>
	class buffer : public resource<buffer<Impl>>
	{
		typedef resource<buffer<Impl>> base_type;
	public:
		buffer(
			string&& name,				// resource name 
			size_t elem_size,			// element size 
			size_t elem_count,			// element count
			device_access cpu_access,	// host access
			device_access gpu_access	// device access
			)
			: base_type(std::move(name), elem_size * elem_count, cpu_access, gpu_access)
			, element_size_(elem_size)
			, element_count_(elem_count)

		{
		}

	protected:
		void resize(size_t elem_size, size_t elem_count)
		{
			base_type::resize(elem_size * elem_count);
			element_size_ = elem_size;
			element_count_ = elem_count;
		}

	private:
		size_t		element_size_;
		size_t		element_count_;
	};
} }