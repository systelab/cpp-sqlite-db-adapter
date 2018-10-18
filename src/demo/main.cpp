#include "BinaryValue.h"
#include <iostream>

int main(int argc, char **argv) {
  systelab::db::sqlite::BinaryValue binary_value(1);

  std::cout << "Hello World! Just created an unary BinaryValue" << std::endl;
  std::cout << "\t->" << argv[0] << "<-" << std::endl;
  return 0;
}