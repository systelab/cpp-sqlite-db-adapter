#ifndef _DBADAPTERTESTUTILITIES_MOCKTABLERECORDSET_QV_0403160901_H
#define _DBADAPTERTESTUTILITIES_MOCKTABLERECORDSET_QV_0403160901_H

#include "DbAdapterInterface/ITableRecordSet.h"

#include "MockTableRecord.h"

#include <gmock/gmock.h>

namespace systelab { namespace test_utility {

	class MockTableRecordSet : public db::ITableRecordSet
	{
	public:
		MockTableRecordSet()
			:m_index(0)
		{
			setDefaultExpectations();
		}

		MockTableRecordSet( std::vector< std::unique_ptr<MockTableRecord> >& records)
			:m_index(0)
		{
			unsigned int nRecords = records.size();
			for (unsigned int i = 0; i < nRecords; i++)
			{
				m_mockRecords.push_back( std::move(records[i]) );
			}
			setDefaultExpectations();
		}

		void setDefaultExpectations()
		{
			ON_CALL(*this, getCurrentRecord()).WillByDefault(Invoke(this, &MockTableRecordSet::getCurrentRecord_Impl));
			ON_CALL(*this, isCurrentRecordValid()).WillByDefault(Invoke(this, &MockTableRecordSet::isCurrentRecordValid_Impl));
			ON_CALL(*this, getRecordsCount()).WillByDefault(Invoke(this, &MockTableRecordSet::getRecordsCount_Impl));
			ON_CALL(*this, nextRecord()).WillByDefault(Invoke(this, &MockTableRecordSet::nextRecord_Impl));
		}

		MOCK_CONST_METHOD0(getTable, db::ITable&());

		MOCK_CONST_METHOD0(getFieldsCount, unsigned int());
		MOCK_CONST_METHOD1(getField, const db::IField&(unsigned int));
		MOCK_CONST_METHOD1(getField, const db::IField&(const std::string&));

		MOCK_CONST_METHOD0(getRecordsCount, unsigned int());
		unsigned int getRecordsCount_Impl() const
		{
			return m_mockRecords.size();
		}

		MOCK_CONST_METHOD0(getCurrentRecord, const db::ITableRecord&());
		const db::ITableRecord& getCurrentRecord_Impl() const
		{
			return *(m_mockRecords[m_index]);
		}

		MOCK_CONST_METHOD0(copyCurrentRecordProxy, db::ITableRecord*());
		std::unique_ptr<db::ITableRecord> copyCurrentRecord() const
		{
			return std::unique_ptr<db::ITableRecord>(copyCurrentRecordProxy());
		}

		MOCK_CONST_METHOD0(isCurrentRecordValid, bool());
		bool isCurrentRecordValid_Impl() const
		{
			return m_index < m_mockRecords.size();
		}

		MOCK_METHOD0(nextRecord, void());
		void nextRecord_Impl()
		{
			m_index++;
		}

		std::unique_ptr<MockTableRecordSet> cloneMock()
		{

			std::vector< std::unique_ptr<MockTableRecord> > m_clonedMockRecords;
			unsigned int nRecords = m_mockRecords.size();
			for (unsigned int i = 0; i < nRecords; i++)
			{
				m_clonedMockRecords.push_back( m_mockRecords[i]->cloneMock() );
			}

			std::unique_ptr<MockTableRecordSet> returnValue(new MockTableRecordSet(m_clonedMockRecords));
			returnValue->setDefaultExpectations();
			return returnValue;
		}

	private:
		std::vector< std::unique_ptr<MockTableRecord> > m_mockRecords;
		unsigned int m_index;
	};
}}

#endif //_DBADAPTERTESTUTILITIES_MOCKTABLERECORDSET_QV_0403160901_H