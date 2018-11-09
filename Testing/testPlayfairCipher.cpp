//! Unit Tests for MPAGSCipher PlayfairCipher Class
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "PlayfairCipher.hpp"

TEST_CASE("Playfair Cipher encryption", "[playfair]") {
  PlayfairCipher pc{"Cipher Key"};
  REQUIRE( pc.applyCipher("HELLOWORLD", CipherMode::Encrypt) == "ROVVYGYBVN");
}

TEST_CASE("Playfair Cipher decryption", "[playfair]") {
  PlayfairCipher pc{"Cipher Key"};
  REQUIRE( pc.applyCipher("ROVVYGYBVN", CipherMode::Decrypt) == "HELLOWORLD");
}
