#pragma once

#include "buffer.hpp"

namespace leaves { namespace pipeline 
{
	template <>
	struct buffer_traits<index_buffer>
	{
		static constexpr object_type type() noexcept
		{
			return object_type::index_buffer;
		}
	};

	class index_buffer : public buffer<index_buffer>
	{

	};
} }