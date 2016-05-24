#pragma once
#include "texture.hpp"

namespace leaves { namespace pipeline
{
	template <>
	struct texture_traits<texture_ds> : texture_traits_base
	{
		static constexpr bool is_texture_2d() noexcept
		{
			return true;
		}

		static constexpr bool is_texture_ds() noexcept
		{
			return true;
		}

		static constexpr object_type type() noexcept
		{
			return object_type::texture_ds;
		}

		struct meta
		{
			pixel_format	format;
			uint16_t		width;
			uint16_t		height;

			texture_meta to_tex_meta() const
			{
				// to do ... throw if pixel fomat is not a depth stencil format
				// ......

				return{ format, width, height, uint16_t{1}, uint16_t{1}, false };
			}
		};
	};

	class texture_ds : public texture<texture_ds>
	{
	public:
		using base_type = texture<texture_ds>;
		using meta = texture_traits<texture_ds>::meta;
	public:
		texture_ds(string name, meta const& meta_data_spec)
			: base_type(std::move(name), meta_data_spec.to_tex_meta(), device_access::none, device_access::read_write)
		{

		}
	};
} }