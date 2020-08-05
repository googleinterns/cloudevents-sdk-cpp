#include "base64.h"
#include <iostream>

#include <gtest/gtest.h>

namespace base64 {


// Test all possibilites of fill bytes (none, one =, two ==)
// References calculated with: https://www.base64encode.org/


TEST(Encode, Empty) {
   absl::StatusOr<std::string> encoded = base64_encode(""); 

   ASSERT_TRUE(encoded.ok());
   ASSERT_EQ((*encoded), "");
}

TEST(Encode, t0) {
   std::string rest0_original = "abc";
   std::string rest0_reference = "YWJj";

   absl::StatusOr<std::string> rest0_encoded = base64_encode(rest0_original);

   ASSERT_TRUE(rest0_encoded.ok());
   ASSERT_EQ((*rest0_encoded), rest0_reference);
}

TEST(Encode, t1) {
   std::string rest1_original = "abcd";
   std::string rest1_reference = "YWJjZA==";

   absl::StatusOr<std::string> rest1_encoded = base64_encode(rest1_original);

   ASSERT_TRUE(rest1_encoded.ok());
   ASSERT_EQ((*rest1_encoded), rest1_reference);
}

TEST(Encode, t2) {
   std::string rest2_original = "abcde";
   std::string rest2_reference = "YWJjZGU=";

   absl::StatusOr<std::string> rest2_encoded = base64_encode(rest2_original);

   ASSERT_TRUE(rest2_encoded.ok());
   ASSERT_EQ((*rest2_encoded), rest2_reference);
}

TEST(Decode, Empty) {
   absl::StatusOr<std::string> decoded = base64_decode(""); 

   ASSERT_TRUE(decoded.ok());
   ASSERT_EQ((*decoded), "");
}

TEST(Decode, t0) {
   std::string rest0_original = "abc";
   std::string rest0_reference = "YWJj";

   absl::StatusOr<std::string> rest0_decoded = base64_decode(rest0_reference);

   ASSERT_TRUE(rest0_decoded.ok());
   ASSERT_EQ((*rest0_decoded), rest0_original);
}

TEST(Decode, t1) {
   std::string rest1_original = "abcd";
   std::string rest1_reference = "YWJjZA==";

   absl::StatusOr<std::string> rest1_decoded = base64_decode(rest1_reference);

   ASSERT_TRUE(rest1_decoded.ok());
   ASSERT_EQ((*rest1_decoded), rest1_original);
}

TEST(Decode, t2) {
   std::string rest2_original = "abcde";
   std::string rest2_reference = "YWJjZGU=";

   absl::StatusOr<std::string> rest2_decoded = base64_decode(rest2_reference);

   ASSERT_TRUE(rest2_decoded.ok());
   ASSERT_EQ((*rest2_decoded), rest2_original);
}


} // base64
