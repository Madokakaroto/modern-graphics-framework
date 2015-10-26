#pragma once

#include "buffer.hpp"
#include "layout\structured_layout.hpp"

namespace leaves { namespace pipeline 
{
	template <>
	struct buffer_traits<structured_buffer>
	{
		static constexpr object_type type() noexcept
		{
			return object_type::structured_buffer;
		}
	};

	class structured_buffer : public buffer<structured_buffer>
	{
		using base_type = buffer<structured_buffer>;
	public:
		structured_buffer(string&& name, structured_layout&& layout, uint16_t count,
			device_access cpu_access = device_access::none, device_access gpu_access = device_access::read_write)
			: base_type(std::move(name), 0, count, cpu_access, gpu_access)
			, layout_(std::move(layout))
		{
			if (device_access::none != cpu_access)
			{
				resize(layout_.size(), count);
			}
		}

	private:
		structured_layout layout_;
	};
} }