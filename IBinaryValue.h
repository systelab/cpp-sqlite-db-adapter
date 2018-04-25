#ifndef _DBADAPTERINTERFACE_IBINARYVALUE_JOSEP_VILA_1712150930_H
#define _DBADAPTERINTERFACE_IBINARYVALUE_JOSEP_VILA_1712150930_H

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

#endif //_DBADAPTERINTERFACE_IBINARYVALUE_JOSEP_VILA_1712150930_H