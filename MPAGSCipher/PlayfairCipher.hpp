#ifndef MPAGSCIPHER_FAIRPLAYCIPHER_HPP
#define MPAGSCIPHER_FAIRPLAYCIPHER_HPP

// Standard library includes
#include <string>
#include <vector>
#include <utility>
#include <map>

// Our project headers
#include "CipherMode.hpp"

/**
 * \file PlayfairCipher.hpp
 * \brief Contains the declaration of the PlayfairCipher class
 */

/**
 * \class PlayfairCipher
 * \brief Encrypt or decrypt text using the Playfair cipher with the given key
 */
class PlayfairCipher {
  public:
    /**
     * Create a new CaesarCipher, converting the given string into the key
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    explicit PlayfairCipher( const std::string& key );


    /**
     * Set they key for future applyCipher calls
     *
     * \param key the string to convert into the key to be used in the cipher
     */
    void setKey(const std::string& key);

    /**
     * Apply the cipher to the provided text
     *
     * \param inputText the text to encrypt or decrypt
     * \param cipherMode whether to encrypt or decrypt the input text
     * \return the result of applying the cipher to the input text
     */
    std::string applyCipher( const std::string& inputText, const CipherMode cipherMode ) const;

  private:
    /// The cipher key
    std::string key_;
    /// The letter to coordinate map
    std::map<char, std::pair<int, int>> letter_map_;
    /// The coordinate to letter map
    std::map<std::pair<int, int>, char> coord_map_;
};

#endif
