#ifndef _DBSQLITEADAPTER_BINARYVALUE_QUIM_VILA_2112151746_H
#define _DBSQLITEADAPTER_BINARYVALUE_QUIM_VILA_2112151746_H

#include "DbAdapterInterface/IBinaryValue.h"

namespace systelab { namespace db { namespace sqlite {

	class BinaryValue : public IBinaryValue
	{
	public:
		BinaryValue(int size);
		BinaryValue(std::istream& inputStream);
		virtual ~BinaryValue();

		std::ostream getOutputStream() const;
		std::istream getInputStream() const;

	private:
		int m_size;
		char* m_buffer;

	};

}}}

#endif //_DBSQLITEADAPTER_BINARYVALUE_QUIM_VILA_2112151746_H