#pragma once
#include <vector>
#include "..\object.hpp"
#include "..\mpl.hpp"
#include "..\math.hpp"

namespace leaves { namespace pipeline 
{
	enum class pixel_format
	{
		unknown,
		rgba_32,
		dc1,
	};

	enum class device_access
	{
		none,
		read,
		write,
		read_write,
	};

	template <typename Impl>
	class resource : public object
	{
	protected:
		resource(object_type type, string&& name, size_t size, device_access cpu_access, device_access gpu_access) noexcept
			: object(type, std::move(name))
			, size_(size)
			, data_(size, 0)
			, cpu_access_(cpu_access)
			, gpu_access_(gpu_access)
		{
		}

	public:
		byte const* data() const noexcept
		{
			return data_.data();
		}

		byte size() const noexcept
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

	protected:
		size_t						size_;				// 32bit integral for x86 & 64bit integral for x64
		std::vector<byte>			data_;				
		device_access				cpu_access_;
		device_access				gpu_access_;
	};
} }