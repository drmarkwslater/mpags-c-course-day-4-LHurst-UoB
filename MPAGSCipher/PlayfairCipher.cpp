
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

#define debug

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
  auto iter = std::remove_if(key_.begin(), key_.end(), [] (const char ch) { return ! std::isalpha(ch); });
  key_.erase(iter, key_.end());

  // Change J -> I
  std::transform(key_.begin(), key_.end(), key_.begin(), [] (const char ch) { return (ch == 'J' ? 'I' : ch); });

  // Remove duplicated letters
  std::string seen_chars {""};
  auto remove_seen = [&seen_chars] (const char ch) {
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
#ifdef debug
      std::cout << "Dumping grid as we build it:" << std::endl;
#endif
  for ( int i=0; i<5; i++) {
    for ( int j=0; j<5; j++) {
      const std::pair<int, int> this_coord {i ,j};
      const char this_letter { key_[i*5+j] };
      letters_coords[this_letter] = this_coord;
      coords_letters[this_coord] = this_letter;
#ifdef debug
      std::cout << this_letter;
#endif
    }
#ifdef debug
      std::cout << std::endl;
#endif
  }

  // Store the playfair cipher key map
  letter_map_ = letters_coords;
  coord_map_ = coords_letters;
#ifdef debug
  std::cout << "Dumping stored info for letters: " << std::endl;
  std::cout << "\tletter(coords): ";
  std::for_each(letter_map_.begin(), letter_map_.end(), [] (std::pair<char, std::pair<int, int>> pair) { std::cout << pair.first << '(' << pair.second.first << ',' << pair.second.second << ") "; });
  std::cout << std::endl;
  std::cout << "\t(coords)letter: ";
  std::for_each(coord_map_.begin(), coord_map_.end(), [] (std::pair<std::pair<int, int>, char> pair) { std::cout << '(' << pair.first.first << ',' << pair.first.second << ')' << pair.second << ' '; });
  std::cout << std::endl;
#endif
}


std::string PlayfairCipher::applyCipher( const std::string& inputText,
  const CipherMode cipherMode ) const
{

  std::string outputText {""};

  /*
   * We shouldn't need to apply the filter rules if the mode is decrypt,
   * however it will not do any harm (as it should already be well formed).
   * The slides read to me as though we should not reverse these on decrypt,
   * which seems reasonable as it is not possible to ensure we get the
   * original text back due to there being many edgecases, especially with
   * repeated letters and non-even length strings.  Not reversing these
   * ensures none of the original text can possibly be lost by removing
   * trailing Zs, Qs between Xs and Xs between double letters that may or may
   * not have been part of the original text.
   */

  // Use null in the last_char to indicate the start of a new digraph
  char last_char {'\0'};
  /*
   * Honestly, I think this would be better as a seperate function due to the
   * length but in the spirit of "using what we were taught on Friday" I've
   * kept it as a lambda function
   */
  auto sanitise_input = [&outputText, &last_char] (const char ch) {
    char this_ch {ch};
  // Change J -> I
    if ( this_ch == 'J' )
      this_ch = 'I';
  // If repeated chars in a digraph add and X or Q if XX
    if ( this_ch == last_char ) {
      if ( this_ch == 'X' ) {
        outputText += "Q";
      } else {
        outputText += 'X';
      }
      last_char = '\0'; // this_ch now forms the first char of the next digraph
    }

    // Null last_char indicates that this is the first character of a digraph
    if ( last_char == '\0' ) {
      last_char = this_ch;
    } else {
      // This was the 2nd char of a digraph, so reset last_char
      last_char = '\0';
    }

    outputText += this_ch; // Add this_ch
  };

  // Apply first 2 rules
  std::for_each(inputText.begin(), inputText.end(), sanitise_input);

  // if the size of input is odd, add a trailing Z
  /*
   * If lsb is '1' then is not divisible by 2 - bitwise & is more efficient
   * than '%'.
   * Could also have used (last_char != '\0') to find if we ended on a full
   * digraph or not.
   */
  if ( outputText.size() & 1 )
    outputText += 'Z';

  // Final sanity check before we try to assume the length is even
  assert((outputText.size() & 1) == 0);

#ifdef debug
  std::cout << "Output text: " << outputText << std::endl;
#endif
  // Loop over the input in Digraphs
  // I tried to use an iterator but filed miserably.  Far easier to address chars by index ;)
  for ( unsigned int i = 0; i < outputText.size(); i+=2) {

  // - Find the coords in the grid for each digraph
    auto coord1 = (*letter_map_.find(outputText[i])).second;
    auto coord2 = (*letter_map_.find(outputText[i+1])).second;

  // - Apply the rules to these coords to get 'new' coords
    // same row
    if ( coord1.first == coord2.first ) {
      // Use letter to the right
#ifdef debug
      std::cout << "Letters " << outputText[i] << " and " << outputText[i+1] << " are on same row" << std::endl;
#endif
      switch (cipherMode) {
        case CipherMode::Encrypt:
          coord1.second = (coord1.second + 1) % 5;
          coord2.second = (coord2.second + 1) % 5;
          break;
        case CipherMode::Decrypt:
          coord1.second = (coord1.second - 1); if (coord1.second < 0) coord1.second = 4;
          coord2.second = (coord2.second - 1); if (coord2.second < 0) coord2.second = 4;
          break;
      }
    // same column
    } else if ( coord1.second == coord2.second ) {
      // Use letter below
#ifdef debug
      std::cout << "Letters " << outputText[i] << " and " << outputText[i+1] << " are in same column" << std::endl;
#endif
      switch (cipherMode) {
        case CipherMode::Encrypt:
          coord1.first = (coord1.first + 1) % 5;
          coord2.first = (coord2.first + 1) % 5;
          break;
        case CipherMode::Decrypt:
          coord1.first = (coord1.first - 1); if (coord1.first < 0) coord1.first = 4;
          coord2.first = (coord2.first - 1); if (coord2.first < 0) coord2.first = 4;
          break;
      }
    // box
    } else {
#ifdef debug
      std::cout << "Letters " << outputText[i] << " and " << outputText[i+1] << " form a box" << std::endl;
#endif
      /*
       *        first --->
       * second +--------------+
       *    |   |1            2|
       *    |   |              |
       *    v   |3            4|
       *        +--------------+
       */
      // Swap with the other corner by picking the character on the same row in-line with the other character
      const int coord1_tmp_first = coord1.first;
      coord1.first = coord2.first;
      coord2.first = coord1_tmp_first;
    }
  // - Find the letter associated with the new coords
#ifdef debug
    std::cout << "Finding letters at coords (" << coord1.first << ',' << coord1.second << ") and  (" << coord2.first << ',' << coord2.second << ")" << std::endl;
    std::cout << "Letter 1: " << (*coord_map_.find(coord1)).second << std::endl;;
    std::cout << "Letter 2: " << (*coord_map_.find(coord2)).second << std::endl;;
#endif
    outputText[i] = (*coord_map_.find( coord1 )).second;
    outputText[i+1] = (*coord_map_.find( coord2 )).second;
  }

  std::cout << "Output text: " << outputText << std::endl;

  // return the text
  return outputText;
}
