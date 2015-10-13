#pragma once

#include <string>
#include <memory>
#include "forward.hpp"

namespace leaves { namespace pipeline 
{
	enum class object_type
	{
		texture_1d,
		texture_2d,
		texture_3d,
		texture_cube,
		texture_1d_array,
		texture_2d_array,
		texture_cube_array,
		texture_rt,
		texture_ds,

		vertex_buffer,
		index_buffer,
		constant_buffer,
		structured_buffer,
		raw_buffer,
		indirect_argument_buffer,

		vertex_shader,
		sampler,
	};

	class object : public std::enable_shared_from_this<object>
	{
	public:
		static constexpr uint32_t invalid_handle() noexcept
		{
			return -1;
		}

	public:
		object() = delete;
		virtual ~object() = default;
		object(object const&) = delete;
		object(object&&) = default;
		object& operator= (object const&) = delete;
		object& operator= (object&&) = default;

	protected:
		object(object_type type, string&& name) noexcept
			: type_(type)
			, id_(invalid_handle())
			, name_(std::move(name))
		{

		}

	public:
		object_type type() const noexcept
		{
			return type_;
		}

		string& name() noexcept
		{
			return name_;
		}

		string const& name() const noexcept
		{
			return name_;
		}

		uint32_t& id() noexcept
		{
			return id_;
		}

		uint32_t id() const noexcept
		{
			return id_;
		}

	private:
		object_type		type_;
		uint32_t		id_;
		string			name_;
	};
} }