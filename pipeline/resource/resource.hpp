#pragma once

#include "..\object.hpp"
#include "infrastucture.hpp"
#include "detail\numeric_size.hpp"

namespace leaves { namespace pipeline 
{
	template <typename Impl>
	class resource : public object
	{
	protected:
		resource(object_type type, string&& name, size_t size, device_access cpu_access, device_access gpu_access, bool alloc_mem = true) noexcept
			: object(type, std::move(name))
			, size_(size)
			, data_()
			, cpu_access_(cpu_access)
			, gpu_access_(gpu_access)
		{
			if (alloc_mem)
				data_.resize(size, 0);
		}

	public:
		byte const* data() const noexcept
		{
			return data_.data();
		}

		byte* data() noexcept
		{
			return data_.data();
		}

		size_t size() const noexcept
		{
			return size_;
		}

		device_access cpu_access() const noexcept
		{
			return cpu_access_;
		}

		device_access& cpu_access() noexcept
		{
			return cpu_access_;
		}

		device_access gpu_access() const noexcept
		{
			return gpu_access_;
		}

		device_access& gpu_access() noexcept
		{
			return gpu_access_;
		}

	protected:

		void resize(size_t size)
		{
			data_.resize(size);
			size_ = size;
		}

		void replace(std::vector<byte>&& other)
		{
			data_ = std::move(other);
			size_ = data_.size();
		}

	protected:
		size_t						size_;				// 32bit integral for x86 & 64bit integral for x64
		std::vector<byte>			data_;				
		device_access				cpu_access_;
		device_access				gpu_access_;
	};
} }