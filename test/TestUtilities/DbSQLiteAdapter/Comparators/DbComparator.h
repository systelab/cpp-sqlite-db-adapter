#ifndef _DBADAPTERTESTUTILITIES_DBCOMPARATOR_DMC_0410161948_H
#define _DBADAPTERTESTUTILITIES_DBCOMPARATOR_DMC_0410161948_H

#include "TestUtilities/DbAdapter/Stubs/StubField.h"
#include "TestUtilities/DbAdapter/Stubs/StubFieldValue.h"
#include "TestUtilities/DbAdapter/Stubs/StubTableRecord.h"


using namespace testing;

namespace systelab { namespace test_utility {

	inline testing::AssertionResult DbComparator(const db::IField& expected, const db::IField& actual)
	{
		if (typeid(expected) != typeid(actual))
		{
			return AssertionFailure() << "Different class type: "
										<< "expected=" << typeid(expected).name()
										<< ", actual=" << typeid(actual).name();
		}

		if (expected.getName() != actual.getName())
		{
			return AssertionFailure() << "Different Name value: "
										<< "expected=" << expected.getName()
										<< ", actual=" << actual.getName();
		}

		if (expected.getType() != actual.getType())
		{
			return AssertionFailure() << "Different Type value: "
										<< "expected=" << expected.getType()
										<< ", actual=" << actual.getType();
		}

		return AssertionSuccess();
	}

	inline testing::AssertionResult DbComparator(const db::IFieldValue& expected, const db::IFieldValue& actual)
	{
		if (typeid(expected) != typeid(actual))
		{
			return AssertionFailure() << "Different class type: "
										<< "expected=" << typeid(expected).name()
										<< ", actual=" << typeid(actual).name();
		}

		if (expected.isDefault() != actual.isDefault())
		{
			return AssertionFailure() << "Different Is Default value: "
										<< "expected=" << expected.isDefault()
										<< ", actual=" << actual.isDefault();
		}

		if (expected.isNull() != actual.isNull())
		{
			return AssertionFailure() << "Different Is Null value: "
										<< "expected=" << expected.isNull()
										<< ", actual=" << actual.isNull();
		}			
			
		if (!expected.isNull())
		{
			systelab::db::FieldTypes fieldType = expected.getField().getType();

			if (fieldType == systelab::db::BOOLEAN && (expected.getBooleanValue() != actual.getBooleanValue()))
			{
				return AssertionFailure() << "Different Boolean value: "
										<< "expected=" << expected.getBooleanValue()
										<< ", actual=" << actual.getBooleanValue();
			}
				
			if (fieldType == systelab::db::INT && (expected.getIntValue() != actual.getIntValue()))
			{
				return AssertionFailure() << "Different Integer value: "
										<< "expected=" << expected.getIntValue()
										<< ", actual=" << actual.getIntValue();
			}

			if (fieldType == systelab::db::DOUBLE && (expected.getDoubleValue() != actual.getDoubleValue()))
			{
				return AssertionFailure() << "Different Double value: "
										<< "expected=" << expected.getDoubleValue()
										<< ", actual=" << actual.getDoubleValue();
			}

			if (fieldType == systelab::db::STRING && (expected.getStringValue() != actual.getStringValue()))
			{
				return AssertionFailure() << "Different String value: "
										<< "expected=" << expected.getStringValue()
										<< ", actual=" << actual.getStringValue();
			}

			if (fieldType == systelab::db::DATETIME && (expected.getDateTimeValue() != actual.getDateTimeValue()))
			{
				return AssertionFailure() << "Different DateTime value: "
										<< "expected=" << expected.getDateTimeValue()
										<< ", actual=" << actual.getDateTimeValue();
			}
		}

		const db::IField& expectedStepField = expected.getField();
		const db::IField& actualStepField = actual.getField();
		
		AssertionResult stubFieldsComparisonResult = DbComparator(expectedStepField, actualStepField);
		if (!stubFieldsComparisonResult)
		{
			return AssertionFailure() << stubFieldsComparisonResult.message();
		}

		return AssertionSuccess();
	}

	inline testing::AssertionResult DbComparator(const db::ITableRecord& expected, const db::ITableRecord& actual)
	{
		if (typeid(expected) != typeid(actual))
		{
			return AssertionFailure() << "Different class type: "
									  << "expected=" << typeid(expected).name()
									  << ", actual=" << typeid(actual).name();
		}

		unsigned int nExpectedFieldValues = expected.getFieldValuesCount();
		unsigned int nActualFieldValues = actual.getFieldValuesCount();
		if (nExpectedFieldValues != nActualFieldValues)
		{
			return AssertionFailure() << "Different Number of field values: "
									  << "expected=" << nExpectedFieldValues
									  << ", actual=" << nActualFieldValues;
		}

		for (unsigned int i = 0; i < nExpectedFieldValues; i++)
		{
			const db::IFieldValue& fieldValue1 = expected.getFieldValue(i);
			const db::IFieldValue& fieldValue2 = actual.getFieldValue(i);
		
			AssertionResult stubFieldsComparisonResult = DbComparator(fieldValue1, fieldValue2);
			if (!stubFieldsComparisonResult)
			{
				return AssertionFailure() << "Different data for field " << i << " "
										  << "(name='" + fieldValue1.getField().getName() + "'): "
										  << stubFieldsComparisonResult.message();
			}
		}

		return AssertionSuccess();
	}



}};

#endif //_DBADAPTERTESTUTILITIES_DBCOMPARATOR_DMC_0410161948_H
