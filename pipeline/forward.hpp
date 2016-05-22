#pragma once

#include <cstdint>
#include <cstddef>
#include <string>
#include <numeric>
#include <algorithm>
#include <tuple>
#include <array>
#include <stdexcept>
#include <vector>
#include <list>
#include <cassert>

namespace leaves
{
	using int8_t	= std::int8_t;
	using int16_t	= std::int16_t;
	using int32_t	= std::int32_t;
	using int64_t	= std::int64_t;
	using uint8_t	= std::uint8_t;
	using uint16_t	= std::uint16_t;
	using uint32_t	= std::uint32_t;
	using uint64_t	= std::uint64_t;
	using size_t	= std::size_t;
	using byte		= std::uint8_t;
	using string	= std::string;
	using char_t	= string::value_type;
}

namespace leaves { namespace pipeline
{
	enum class object_type;
	enum class pixel_format;
	enum class device_access;

	class object;

	// layout
	class input_layout;
	class numeric_layout;
	class numeric_variable;

	// resource
	class resource;
		
		// textures
		template <typename>
		class texture;
			class texture_1d;
			class texture_2d;
			class texture_3d;
			class texture_cube;
			class texture_1d_array;
			class texture_2d_array;
			class texture_cube_array;
			class texture_rt;			// render target texture
			class texture_ds;			// depth stencil texture

		// buffer
		template <typename>
		class buffer;
			class vertex_buffer;
			class index_buffer;
			class constant_buffer;
			class structured_buffer;
			class raw_buffer;
			class indirect_argument_buffer;

	// graphics states
	class sampler;

	// texture traits
	template <typename>
	struct texture_traits;

	// buffer traits
	template <typename>
	struct buffer_traits;
} }