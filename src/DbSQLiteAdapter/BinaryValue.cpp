#include "BinaryValue.h"

#include <streambuf>


namespace systelab { namespace db { namespace sqlite {

	struct istreambuf : std::streambuf
	{
		istreambuf(char* begin, char* end)
		{
			this->setg(begin, begin, end);
		}
	};

	struct ostreambuf : public std::basic_streambuf<char, std::char_traits<char> >
	{
		ostreambuf(char* buffer, std::streamsize bufferLength)
		{
			setp(buffer, buffer + bufferLength);
		}
	};

	BinaryValue::BinaryValue(int size)
	{
		m_size = size;
		m_buffer = new char[m_size];
	}

	BinaryValue::BinaryValue(std::istream& inputStream)
	{
		m_size = (int) inputStream.width();
		m_buffer = new char[m_size];
		inputStream.read(m_buffer, m_size);
	}

	BinaryValue::~BinaryValue()
	{
		delete m_buffer;
	}

	std::ostream BinaryValue::getOutputStream() const
	{
		ostreambuf obuf(m_buffer, m_size);
		throw "Not implemented yet!";
		//return std::ostream(&obuf);
	}

	std::istream BinaryValue::getInputStream() const
	{
		istreambuf sbuf(m_buffer, m_buffer + sizeof(m_buffer));
		throw "Not implemented yet!";
		//return std::istream(&sbuf);
	}

}}}