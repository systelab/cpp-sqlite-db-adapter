// TODO: reference additional headers your program requires here

#include <stdio.h>
//#include <tchar.h>
#include <memory>
#include <string>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[]) {
  //    ::testing::GTEST_FLAG(output) = "xml:./report.xml";
  ::testing::InitGoogleTest(&argc, argv);

  int res = RUN_ALL_TESTS();

  return res;
}
