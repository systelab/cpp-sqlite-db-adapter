#pragma once

#include <DbAdapterInterface/Types.h>

namespace systelab::db::sqlite::date_time
{
	bool isDateTimeNull(const DateTimeType&);
	std::string toISOString(const DateTimeType&);
	DateTimeType toDateTime(const std::string& isoString);
}