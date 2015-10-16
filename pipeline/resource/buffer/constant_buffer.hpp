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

	// recursive member
	struct complexed_layout
	{

	};

	class constant_buffer : public buffer<constant_buffer>
	{
		using base_type = buffer<constant_buffer>;
	public:


	};
} }