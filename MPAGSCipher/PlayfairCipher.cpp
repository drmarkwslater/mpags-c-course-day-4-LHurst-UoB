
// Standard library includes
#include <iostream>
#include <string>
#include <vector>

// Out project headers
#include "PlayfairCipher.hpp"

PlayfairCipher::PlayfairCipher( const std::string& key )
{
  // Set the key to the one passed to the constructor
  setKey(key);
}

void PlayfairCipher::setKey( const std::string& key )
{
  // store the original key
  key_ = key;

  // Append the alphabet

  // Make sure the key is upper case

  // Remove non-alpha characters

  // Change J -> I

  // Remove duplicated letters

  // Store the coords of each letter

  // Store the playfair cipher key map
}


std::string PlayfairCipher::applyCipher( const std::string& inputText, const CipherMode cipherMode ) const
{
  // Change J -> I
  std::cout << (cipherMode == cipherMode) << std::endl;
  // If repeated chars in a digraph add and X or Q if XX

  // if the size of input is odd, add a trailing Z

  // Loop over theinput in Digraphs

  // - Find the coords in the grid for each digraph

  // - Apply the rules to these coords to get 'new' coords

  // - Find teh letter associated with the new coords

  // return the text
  return inputText;
}
