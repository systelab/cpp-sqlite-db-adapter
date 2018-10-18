#ifndef _DBADAPTERINTERFACE_IRECORDSET_JOSEP_VILA_1712151008_H
#define _DBADAPTERINTERFACE_IRECORDSET_JOSEP_VILA_1712151008_H

#include "IField.h"
#include "IRecord.h"

#include <memory>

namespace systelab { namespace db {

	class IRecordSet
	{
	public:
		virtual ~IRecordSet() {}

		virtual unsigned int getFieldsCount() const = 0;
		virtual const IField& getField(unsigned int index) const = 0;
		virtual const IField& getField(const std::string& fieldName) const = 0;

		virtual unsigned int getRecordsCount() const = 0;

		virtual const IRecord& getCurrentRecord() const = 0;
		virtual std::unique_ptr<IRecord> copyCurrentRecord() const = 0;
		virtual bool isCurrentRecordValid() const = 0;
		virtual void nextRecord() = 0;
	};

}}

#endif //_DBADAPTERINTERFACE_IRECORDSET_JOSEP_VILA_1712151008_H