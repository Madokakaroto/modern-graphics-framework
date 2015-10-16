#pragma once

namespace leaves { namespace pipeline 
{
	namespace detail
	{

		// [type] [name][count];
		class variable
		{
			using sub_vars = std::list<variable>;
		public:
			variable(data_format format, uint16_t count, uint16_t size, uint16_t offset)
				: format_(format)
				, count_(count)
				, size_(size)
				, offset_(offset)
				, sub_vars_()
			{

			}



		private:
			data_format		format_;
			uint16_t		count_;
			uint16_t		size_;
			uint16_t		offset_;
			sub_vars		sub_vars_;
		};
	}
} }