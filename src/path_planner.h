#pragma once

#include <vector>
#include <utility>

#include "elevation_map.h"

namespace path_planning {

class PathPlanner {
 public:
  /// @brief Constructor
  PathPlanner();
  /// @brief Constructor
  /// @param An instantiated elevation map 
  PathPlanner(const ElevationMap& emap);
  /// @brief Destructor
  ~PathPlanner();
  /// @brief Set the current map to use for path planning
  /// @param emap - The Elevation map to use for path planning
  void SetMap(const ElevationMap& emap) { emap_ = emap; };
  /// @brief Plan a path from the beginning to the end locations on the current 
  /// elevation map
  /// @param elevation_profile - The output elevation profile from the 
  /// planned path.
  /// @param path - Optional. The full path that was taken in row and 
  /// column coordinates
  /// @return true if a path was successfully planned
  bool PlanPath(std::vector<int>* elevation_profile, 
    std::vector<std::pair<int, int>>* path=nullptr);
 private:
  /// The current elevation map to plan a path for
  ElevationMap emap_;
};
}