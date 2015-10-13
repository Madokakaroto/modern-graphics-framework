#pragma once

#include "..\object.hpp"

namespace leaves { namespace pipeline 
{
	class shader : public object
	{
	protected:
		shader(object_type type, string&& name)
			: object(type, std::move(name))
		{

		}

	private:

	};
} } 