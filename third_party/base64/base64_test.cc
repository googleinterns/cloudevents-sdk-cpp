#include "base64.h"
#include <iostream>

#include <gtest/gtest.h>

namespace cloudevents_base64 {

// Test all possibilites of fill bytes (none, one =, two ==)
// References calculated with: https://www.base64encode.org/


TEST(Encode, Empty) {
   std::string encoded = base64_encode(""); 

   ASSERT_EQ(encoded, "");
}

TEST(Encode, FillZero) {
   std::string rest0_original = "abc";
   std::string rest0_reference = "YWJj";

   std::string rest0_encoded = base64_encode(rest0_original);

   ASSERT_EQ(rest0_encoded, rest0_reference);
}

TEST(Encode, FillOne) {
   std::string rest1_original = "abcd";
   std::string rest1_reference = "YWJjZA==";

   std::string rest1_encoded = base64_encode(rest1_original);

   ASSERT_EQ(rest1_encoded, rest1_reference);
}

TEST(Encode, FillTwo) {
   std::string rest2_original = "abcde";
   std::string rest2_reference = "YWJjZGU=";

   std::string rest2_encoded = base64_encode(rest2_original);

   ASSERT_EQ(rest2_encoded, rest2_reference);
}

TEST(Decode, Empty) {
   std::string decoded = base64_decode(""); 

   ASSERT_EQ(decoded, "");
}

TEST(Decode, FillZero) {
   std::string rest0_original = "abc";
   std::string rest0_reference = "YWJj";

   std::string rest0_decoded = base64_decode(rest0_reference);

   ASSERT_EQ(rest0_decoded, rest0_original);
}

TEST(Decode, FillOne) {
   std::string rest1_original = "abcd";
   std::string rest1_reference = "YWJjZA==";

   std::string rest1_decoded = base64_decode(rest1_reference);

   ASSERT_EQ(rest1_decoded, rest1_original);
}

TEST(Decode, FillTwo) {
   std::string rest2_original = "abcde";
   std::string rest2_reference = "YWJjZGU=";

   std::string rest2_decoded = base64_decode(rest2_reference);

   ASSERT_EQ(rest2_decoded, rest2_original);
}

}  // namespace cloudevents_base64
