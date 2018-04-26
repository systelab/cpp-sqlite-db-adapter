#pragma once

#include <ostream>
#include <istream>

namespace systelab { namespace db {
	class IBinaryValue
	{
	public:
		virtual ~IBinaryValue(){};

		virtual std::ostream getOutputStream() const = 0;
		virtual std::istream getInputStream() const = 0;
	};
}}
