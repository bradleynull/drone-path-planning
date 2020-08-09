#include <map>
#include <string>
#include <vector>

namespace path_planning {
/// A map of special character in the map to indicate their semantic meaning
/// and assign an integer value to them.
static const std::map<char, int> kSpecialLocations{
    {'A', -1},  // Start location
    {'B', -2}   // End location
};

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
 private:
  /// The map data structure for querying
  std::vector<std::vector<int>> map_;
};
}