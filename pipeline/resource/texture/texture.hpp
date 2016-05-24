#pragma once

#include "..\resource.hpp"
#include "detail\texture_subresource.hpp"

namespace leaves { namespace pipeline 
{
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

	struct texture_meta
	{
		pixel_format			format;
		uint16_t				width;
		uint16_t				height;
		uint16_t				depth;
		uint16_t				array_size;
		bool					has_mipmap;
	};

	template <typename Impl>
	class texture : public resource
	{
	public:
		using traits_type = texture_traits<Impl>;
		using subresource_container = std::vector<subresource>;
		using meta_data = typename traits_type::meta;

	protected:

		/*
		 * constructor
		 */
		texture(
			string name,						// resource name
			texture_meta const& meta_data,		// meta data
			device_access cpu_access,			// host access
			device_access gpu_access)			// device access
			: resource(traits_type::type(), std::move(name), 0, cpu_access, gpu_access)
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
			texture_subresource ctr
			{ 
				meta_data.format, 
				meta_data.width, meta_data.height, meta_data.depth, 
				meta_data.array_size, 
				meta_data.has_mipmap 
			};
			subresources_ = std::move(ctr.move_subres());
			replace(std::move(ctr.move_data()));
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
		auto begin() noexcept
		{
			return subresources_.begin();
		}

		/*
		 * iterate through all subresources
		 */
		auto end() noexcept
		{
			return subresources_.end();
		}

		/*
		 * reset this texture resource
		 */
		void reset(meta_data const& meta_data_spec)
		{
			auto meta_data = meta_data_spec.to_tex_meta();

			// if meta equals, no construct needed
			if (::memcmp(&meta_data, &meta_data_, sizeof(texture_meta)) == 0)
				return;

			construct(meta_data);		// basic guarantee
			meta_data_ = meta_data;		// no exception(copy assign is with noexcept signature)
		}

	protected:
		texture_meta			meta_data_;
		subresource_container	subresources_;
	};
} }