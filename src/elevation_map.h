#pragma once

#include <map>
#include <string>
#include <vector>

namespace path_planning {

/// The default starting position for a map
static const char kStartPos = 'A';
/// The default end position for a map
static const char kEndPos = 'B';
/// A map of special character in the map to indicate their semantic meaning
/// and assign an integer value to them.
static const std::map<char, int> kSpecialLocations{
    {kStartPos, -1},  // Start location
    {kEndPos, -2}   // End location
};

/// @class Data structure for reading and querying map data
class ElevationMap {
 public:
  /// @brief Constructor
  ElevationMap();
  /// @brief Destructor
  ~ElevationMap();
  /// @brief Read in a map from the given file
  /// @param map_filename - The file to read the map from
  /// @return true if the map was successfully read
  bool ReadMap(const std::string& map_filename);
  /// @brief Get an element in the current map
  /// @param row - The row index into the map
  /// @param col - The column index into the map
  /// @return The value at row, col. Throws `out_of_range` if row, col does
  /// not exist
  int operator()(int row, int col) const;
  /// @brief Get an element in the current map for mutating
  /// @param row - The row index into the map
  /// @param col - The column index into the map
  /// @return The value at row, col. Throws `out_of_range` if row, col does
  /// not exist
  int& operator()(int row, int col);
  /// @brief Get the total number of rows in the map
  /// @return The total number of rows
  const int rows() const { return int(map_.size()); }
  /// @brief Get the total number of columns in the map
  /// @return The total number of columns
  const int cols() const { return rows() > 0 ? int(map_[0].size()) : 0;}
  /// @brief Clear all the map data and meta data
  void Clear() {map_.clear(); special_locations_.clear();}
  /// @brief Get the special locations in the map for the specified character.
  /// @param c - The character from kSpecialLocations that IDs the location
  /// @return A vector of all locations that correspond with c
  std::vector<std::pair<int, int>> GetLocations(char c);
 private:
  /// The map data structure for querying
  std::vector<std::vector<int>> map_;
  /// A map of all the special locations that were placed in the map
  std::map<char, std::vector<std::pair<int, int>>> special_locations_;
};
}