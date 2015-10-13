#pragma once

namespace leaves
{
	template <typename T, T Left, T Right>
	struct larger_than
	{
		static constexpr bool value = Left > Right;
	};
}