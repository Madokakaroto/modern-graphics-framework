#pragma once

namespace leaves
{
	template <typename T>
	struct large_class_wrapper
	{
	};

	template <typename T>
	auto wrap_large_class()
	{
		return detail::large_class_wrapper<typename T::tuple_type>{};
	}
}