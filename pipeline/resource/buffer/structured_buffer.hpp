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
		structured_buffer(
			string&& name,
			structured_layout&& layout, 
			uint16_t count
			) noexcept
			: base_type(std::move(name), 0, count, device_access::none, device_access::read_write)
			, layout_(std::move(layout))
		{
		}

		void resize(structured_layout&& layout, uint16_t count)
		{
			auto elem_size = layout.size();
			base_type::resize(elem_size, count);
			layout_ = std::move(layout);
		}



	private:
		structured_layout layout_;
	};
} }