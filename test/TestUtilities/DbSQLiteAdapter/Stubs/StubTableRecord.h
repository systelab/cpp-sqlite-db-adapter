#ifndef _DBADAPTERTESTUTILITIES_STUBTABLERECORD_DMC_0410161816_H
#define _DBADAPTERTESTUTILITIES_STUBTABLERECORD_DMC_0410161816_H

#include "TestUtilities/DbAdapter/Mocks/MockTableRecord.h"
#include "TestUtilities/DbAdapter/Stubs/StubFieldValue.h"

#include <memory>
#include <vector>


namespace systelab { namespace test_utility {

	class StubField;

	class StubTableRecord : public MockTableRecord
	{
	public:
		StubTableRecord(const ITableRecord&);
		StubTableRecord(const StubTableRecord&);
		StubTableRecord(std::vector< std::unique_ptr<StubFieldValue> >&);
		virtual ~StubTableRecord();

		db::ITable& getTable() const;
		unsigned int getFieldValuesCount() const;
		StubFieldValue& getFieldValue(unsigned int index) const;
		db::IFieldValue& getFieldValue(const std::string& fieldName) const;

		bool hasFieldValue(const std::string& fieldName) const;

		std::vector<db::IFieldValue*> getValuesList() const;

		StubTableRecord& operator= (const StubTableRecord& other);

	private:
		std::vector< std::unique_ptr<StubFieldValue> > m_fieldValues;
	};

}}

#endif //_DBADAPTERTESTUTILITIES_STUBTABLERECORD_DMC_0410161816_H