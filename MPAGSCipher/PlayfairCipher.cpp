
// Standard library includes
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <cassert>

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
  key_.append("abcdefghijklmnopqrstuvwxyz");

  // Make sure the key is upper case
  std::transform(key_.begin(), key_.end(), key_.begin(), toupper);

  // Remove non-alpha characters
  auto iter = std::remove_if(key_.begin(), key_.end(), [] (char ch) { return ! std::isalpha(ch); });
  key_.erase(iter, key_.end());

  // Change J -> I
  std::transform(key_.begin(), key_.end(), key_.begin(), [] (char ch) { return (ch == 'J' ? 'I' : ch); });

  // Remove duplicated letters
  std::string seen_chars {""};
  auto remove_seen = [&seen_chars] (char ch) {
    if (seen_chars.find(ch) == std::string::npos) {
      // no ch in seen_chars
      seen_chars += ch;
      return false;
    } else {
      // ch already in seen chars
      return true;
    }
  };

  iter = std::remove_if(key_.begin(), key_.end(), remove_seen);
  key_.erase(iter, key_.end());

  // Should have a 5x5 grids worth of letters
  assert(key_.size() == 25);

  // Store the coords of each letter
  std::map<char, std::pair<int, int>> letters_coords;
  std::map<std::pair<int, int>, char> coords_letters;
  for ( int i=0; i<5; i++) {
    for ( int j=0; j<5; j++) {
      const std::pair<int, int> this_coord {i ,j};
      const char this_letter { key_[i*5+j] };
      letters_coords[this_letter] = this_coord;
      coords_letters[this_coord] = this_letter;
    }
  }

  // Store the playfair cipher key map
  letter_map_ = letters_coords;
  coord_map_ == coords_letters;

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
