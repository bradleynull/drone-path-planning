#include <iostream> 
#include <fstream>
#include <sstream>

#include "elevation_map.h"

using namespace path_planning;

ElevationMap::ElevationMap() {
}

ElevationMap::~ElevationMap() {
}

bool ElevationMap::ReadMap(const std::string& map_filename) {
  Clear();
  std::ifstream in_file(map_filename);
  // Try to open the file or fail out
  if (!in_file.is_open()) {
    std::cerr << "ElevationMap::ReadMap(): ERROR! Unable to read map file: "
              << map_filename << std::endl;
    return false;
  }

  std::string row;
  std::string token;
  // For each row in the map bookended by []
  while (std::getline(in_file, row, ']')) {
    // Check for an empty row at the end of the file, or just a comma
    if(row.size() <= 1) {
      continue;
    }
    map_.emplace_back(std::vector<int>());
    // Go through the row and tokenize elements by commas
    std::istringstream rowss(row);
    while (std::getline(rowss, token, ',')) {
      // Strip off the opening brackets if there are any
      token.erase(std::remove(token.begin(), token.end(), ' '), token.end());
      token.erase(std::remove(token.begin(), token.end(), ','), token.end());
      token.erase(std::remove(token.begin(), token.end(), '\r'), token.end());
      token.erase(std::remove(token.begin(), token.end(), '\n'), token.end());
      token.erase(std::remove(token.begin(), token.end(), '['), token.end());
      if (token.size() == 0) {
        continue;
      }
      try {
        // If we can parse it as an int, then put it in the map, otherwise its a 
        // special location or a bad character
        map_.back().emplace_back(std::stoi(token));
      } catch(const std::invalid_argument& ia) {
        // A () marks a special location in the map, first make sure its big 
        // enough to hold a location without segfaulting
        if(token.size() >= 3 && token[0] == '(') {
          auto key_loc = kSpecialLocations.find(token[1]);
          // Make sure we know about this location
          if(key_loc != kSpecialLocations.end()) {
            map_.back().emplace_back(key_loc->second);
            // Store the special locations for convenience
            special_locations_[key_loc->first].emplace_back(std::make_pair(
                int(map_.size()) - 1, int(map_.back().size()) - 1));
          }  else {
            std::cerr << "ElevationMap::ReadMap: Unable to parse location "
                      << token << std::endl;
            Clear();
            return false;
          }
        } else {
            std::cerr << "ElevationMap::ReadMap: Unable to parse token "
                      << token << std::endl;
            Clear();
            return false;
        }
      }
    }
  }

  // Validate that all rows are the same size
  size_t row_size = map_.begin()->size();
  for(const auto& row : map_) {
    if(row.size() != row_size) {
      Clear();
      std::cerr << "ElevationMap::ReadMap: Inconsistent row size "
                << row_size << " vs. " << row.size() << std::endl;
      return false;
    }
  }

  return true;
}

std::vector<std::pair<int, int>> ElevationMap::GetLocations(char c) {
  auto it = special_locations_.find(c);
  if(it != special_locations_.end()) {
    return it->second;
  }
  return std::vector<std::pair<int, int>>();
}

int& ElevationMap::operator()(int row, int col) {
  // This function is supposed to throw, but just let the natural vector 
  // throw handle it
  return map_.at(row).at(col);
}

int ElevationMap::operator()(int row, int col) const {
  // This function is supposed to throw, but just let the natural vector 
  // throw handle it
  return map_.at(row).at(col);
}