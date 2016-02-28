#pragma once
#include "texture.hpp"

namespace leaves { namespace pipeline 
{
	template <>
	struct texture_traits<texture_rt> : texture_traits_base
	{
		static constexpr bool is_texture_2d() noexcept
		{
			return true;
		}

		static constexpr bool is_texture_rt() noexcept
		{
			return true;
		}

		static constexpr object_type type() noexcept
		{
			return object_type::texture_rt;
		}

		struct meta
		{
			pixel_format	format;
			uint16_t		width;
			uint16_t		height;

			texture_meta to_tex_meta() const
			{
				// to do ... throw if pixel fomat is not a render target format
				// ......

				return{ format, width, height, uint16_t{1}, uint16_t{1}, false };
			}
		};
	};

	class texture_rt : public texture<texture_rt>
	{
	public:
		using base_type = texture<texture_rt>;
		using meta = texture_traits<texture_rt>::meta;
	public:
		texture_rt(string&& name, meta const& meta_data_spec)
			: base_type(std::move(name), meta_data_spec.to_tex_meta(), device_access::none, device_access::read_write)
		{

		}
	};
} }