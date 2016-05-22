#pragma once

#include "..\object.hpp"
#include "numeric.hpp"

namespace leaves { namespace pipeline 
{
	class resource : public object
	{
	protected:
		resource(object_type type, string&& name, size_t size, 
			device_access cpu_access, device_access gpu_access) noexcept
			: object(type, std::move(name))
			, size_(size)
			, cpu_access_(cpu_access)
			, gpu_access_(gpu_access)
		{
		}

	public:
		byte const* data() const noexcept
		{
			assert(allocated());
			return data_.data();
		}

		byte* data() noexcept
		{
			assert(allocated());
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

		void allocate()
		{
			data_.resize(size_, 0);
		}

		void deallocate() noexcept
		{
			data_.clear();
		}

		bool allocated() const noexcept
		{
			return !data_.empty();
		}

	protected:
		void resize(size_t size)
		{
			if (allocated())
				data_.resize(size, 0);
			size_ = size;
		}

		void replace(std::vector<byte>&& other)
		{
			data_ = std::move(other);
			size_ = data_.size();
		}

	private:
		size_t						size_;				// 32bit integral for x86 & 64bit integral for x64
		std::vector<byte>			data_;				
		device_access				cpu_access_;
		device_access				gpu_access_;
	};
} }