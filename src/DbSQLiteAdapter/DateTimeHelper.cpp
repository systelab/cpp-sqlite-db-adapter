#include "stdafx.h"
#include "DateTimeHelper.h"

#include <array>

namespace systelab::db::sqlite::date_time
{
	bool isDateTimeNull(const DateTimeType& dateTime)
	{
		return dateTime == DateTimeType();
	}

	std::string toISOString(const DateTimeType& dateTime)
	{
		return (!isDateTimeNull(dateTime)) ? std::format("{:%Y%m%dT%H%M%S}", dateTime) : "";
	}

	DateTimeType toDateTime(const std::string& isoString)
	{
		std::array<const char*, 2> supportedFormats =
		{
			"%Y%m%dT%H%M%S", // ISO string DateTime (no separators except T): 20220703T103045 or 20220703T103045.123 or 20220703T103045.123456
			"%Y%m%d" // ISO string Date (no separators): 20220703
		};
		
		DateTimeType timePointDateTime;
		std::istringstream is;
		auto formatIt = supportedFormats.begin();
		do
		{
			is = std::istringstream{ isoString };
			is >> std::chrono::parse(*formatIt, timePointDateTime);
			formatIt++;
		} while (is.fail() && formatIt != supportedFormats.cend());

		return timePointDateTime;
	}
}