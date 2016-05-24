#pragma once
#include "texture.hpp"

namespace leaves { namespace pipeline
{
	template <>
	struct texture_traits<texture_2d> : texture_traits_base
	{
		static constexpr object_type type() noexcept
		{
			return object_type::texture_2d;
		}

		static constexpr bool is_texture_2d() noexcept
		{
			return true;
		}

		// meta structure
		struct meta
		{
			pixel_format	format;
			uint16_t		width;
			uint16_t		height;
			bool			has_mipmap;

			texture_meta to_tex_meta() const noexcept
			{
				return{ format, width, height, uint16_t{1}, uint16_t{1}, has_mipmap };
			}
		};
	};


	class texture_2d : public texture<texture_2d>
	{
	public:
		using base_type = texture<texture_2d>;
		using meta = texture_traits<texture_2d>::meta;
	public:
		texture_2d(string name, meta const& meta_data_spec)
			: base_type(std::move(name), meta_data_spec.to_tex_meta(), device_access::none, device_access::read)
		{
		}
	};
} }