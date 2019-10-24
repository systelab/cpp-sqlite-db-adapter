#pragma once

#define _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING 1

// STL
#include <stdio.h>
#include <memory>
#include <string>
using namespace std::string_literals;

// GTEST
#include <gtest/gtest.h>
#include <gmock/gmock.h>

// BOOST
#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#include <boost/optional.hpp>