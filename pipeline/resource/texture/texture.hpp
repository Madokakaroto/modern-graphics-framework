#pragma once

#include "..\resource.hpp"
#include "detail\texture_subresource.hpp"

namespace leaves { namespace pipeline 
{
	struct subresource
	{
		uint16_t	mip_level;
		uint16_t	array_index;
		uint16_t	width;
		uint16_t	height;
		uint16_t	depth;
		size_t		row_pitch;
		size_t		slice_pitch;
		byte*		pointer;
	};

	struct texture_meta
	{
		pixel_format			format;
		uint16_t				width;
		uint16_t				height;
		uint16_t				depth;
		uint16_t				array_size;
		bool					has_mipmap;
	};

	struct texture_traits_base
	{
		static constexpr bool is_texture_1d() noexcept
		{
			return false;
		}

		static constexpr bool is_texture_2d() noexcept
		{
			return true;
		}

		static constexpr bool is_texture_3d() noexcept
		{
			return false;
		}

		static constexpr bool is_texture_cube() noexcept
		{
			return false;
		}

		static constexpr bool is_texture_array() noexcept
		{
			return false;
		}

		static constexpr bool is_texture_rt() noexcept
		{
			return false;
		}

		static constexpr bool is_texture_ds() noexcept
		{
			return false;
		}
	};

	template <typename Impl>
	class texture : public resource<texture<Impl>>
	{
	public:
		using base_type = resource;
		using traits_type = texture_traits<Impl>;
		using subresource_container = std::vector<subresource>;
		using meta_data = typename traits_type::meta;

	protected:

		/*
		 * constructor
		 */
		texture(
			string&& name,						// resource name
			texture_meta const& meta_data,		// meta data
			device_access cpu_access,			// host access
			device_access gpu_access)			// device access
			: base_type(traits_type::type(), name, cpu_access, gpu_access)
			, meta_data_(meta_data)
			, subresources_()
		{
			construct(meta_data);
		}

		/* 
		 * construct function
		 */
		void construct(texture_meta const& meta_data)
		{
			texture_subresource ctr{ meta_data };
			subresources_ = std::move(ctr.move_subres());
			data_ = std::move(ctr.move_data());
		}

	public:

		/*
		 * get the texture meta data
		 */ 
		texture_meta const& get_meta() const noexcept
		{
			return meta_data_;
		}

		/*
		 * iterate through all subresources
		 */
		auto subres_begin() noexcept
		{
			return subresources_.begin();
		}

		/*
		 * iterate through all subresources
		 */
		auto subres_end() noexcept
		{
			return subresources_.end();
		}

		/*
		 * reset this texture resource
		 */
		void reset(meta_data const& meta_data_spec)
		{
			auto meta_data = meta_data_spec.to_tex_meta();
			construct(meta_data);		// basic guarantee
			meta_data_ = meta_data;		// no exception(copy assign is with noexcept signature)
		}

	protected:
		texture_meta			meta_data_;
		subresource_container	subresources_;
	};
} }