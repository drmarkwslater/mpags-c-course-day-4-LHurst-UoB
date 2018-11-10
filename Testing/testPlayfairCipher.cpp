//! Unit Tests for MPAGSCipher PlayfairCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.hpp"

TEST_CASE("Playfair Cipher encryption", "[playfair]") {
  PlayfairCipher pc{"Cipher Key"};
  REQUIRE( pc.applyCipher("HELLOWORLD", CipherMode::Encrypt) == "ECSHSFQVDAUM");
}

TEST_CASE("Playfair Cipher decryption", "[playfair]") {
  PlayfairCipher pc{"Cipher Key"};
  // N.B. Due to the sanitisation of the input text, the best we can get out
  //   is with the double letters seperated and 'Z' padded to even length.
  REQUIRE( pc.applyCipher("ECSHSFQVDAUM", CipherMode::Decrypt) == "HELXLOWORLDZ");
}
