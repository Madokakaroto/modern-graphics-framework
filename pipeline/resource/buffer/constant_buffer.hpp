#pragma once

#include "buffer.hpp"

namespace leaves{ namespace pipeline 
{
	template<>
	struct buffer_traits<constant_buffer>
	{
		static constexpr object_type type() noexcept
		{
			return object_type::constant_buffer;
		}
	};
	
	class constant_buffer : public buffer<constant_buffer>
	{
		using base_type = buffer<constant_buffer>;
	
	public:
		constant_buffer(string&& name, numeric_layout&& layout)
			: base_type(std::move(name), layout.size(), 1, device_access::write, device_access::read)
			, layout_(std::move(layout))
		{
			allocate();
		}

		constant_buffer(string&& name, numeric_layout const& layout)
			: base_type(std::move(name), layout.size(), 1, device_access::write, device_access::read)
			, layout_(layout)
		{
			allocate();
		}
	
		void resize(numeric_layout&& layout)
		{
			auto elem_size = layout.size();
			base_type::resize(elem_size, 1);
			layout_ = std::move(layout);
		}
	
		numeric_variable operator[](size_t index)
		{
			return{ layout_[index], data() };
		}
	
	private:
		numeric_layout		layout_;
	};
} }