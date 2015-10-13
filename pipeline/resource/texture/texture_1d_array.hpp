#pragma once

#include "texture.hpp"

namespace leaves { namespace pipeline
{
	template <>
	struct texture_traits<texture_1d_array> : texture_traits_base
	{
		static constexpr bool is_texture_1d() noexcept
		{
			return true;
		}

		static constexpr object_type type() noexcept
		{
			return object_type::texture_1d;
		}

		static constexpr bool is_texture_array() noexcept
		{
			return true;
		}

		// meta structure
		struct meta
		{
			pixel_format	format;
			uint16_t		width;
			uint16_t		array_size;
			bool			has_mipmap;

			texture_meta to_tex_meta() const noexcept
			{
				return{ format, width, uint16_t{1}, uint16_t{1}, array_size, has_mipmap };
			}
		};
	};

	class texture_1d_array : public texture<texture_1d_array>
	{
	public:
		// base class type
		using base_type = texture<texture_1d_array>;
		using meta = texture_traits<texture_1d_array>::meta;
	public:
		// constructor
		texture_1d_array(string&& name, meta const& meta_data_spec)
			: base_type(std::move(name), meta_data_spec.to_tex_meta(), device_access::none, device_access::read)
		{
		}
	};
} }