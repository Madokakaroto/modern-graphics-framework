#include <iostream>
#include <pipeline\resource\texture\texture_2d.hpp>

int main(void)
{
	using texture_2d = leaves::pipeline::texture_2d;
	using pixel_format = leaves::pipeline::pixel_format;
	using string = leaves::pipeline::string;

	auto meta_data_spec = texture_2d::meta{ pixel_format::rgba_32, 256, 256, true };

	texture_2d texture{ L"my texture", meta_data_spec };

	texture.reset(meta_data_spec);

	return 0;
}