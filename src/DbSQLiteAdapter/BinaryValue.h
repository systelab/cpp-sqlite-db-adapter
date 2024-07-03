#pragma once

#include "DbAdapterInterface/IBinaryValue.h"

namespace systelab { namespace db { namespace sqlite {

	class BinaryValue : public IBinaryValue
	{
	public:
		explicit BinaryValue(int size);
		explicit BinaryValue(std::istream&);
		~BinaryValue();

		std::ostream getOutputStream() const override;
		std::istream getInputStream() const override;

	private:
		int m_size;
		char* m_buffer;
	};

}}}
