#pragma once

#include <vector>
#include <utility>

#include "elevation_map.h"

namespace path_planning {

/// @class Path planner for generating elevation and paths for a given map
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
  /// elevation map.
  /// @brief
  /// @param elevation_profile - The output elevation profile from the
  /// planned path.
  /// @param agl_elevation_profile - The elevation profile with the agl applied
  /// to it.
  /// @param agl - The minimum altitude to maintain over the terrain when
  /// planning the path
  /// @param path - Optional. The full path that was taken in row and
  /// column coordinates
  /// @return true if a path was successfully planned
  bool PlanPath(std::vector<int>* elevation_profile,
                std::vector<int>* agl_elevation_profile,
                const int& agl = 0,
                std::vector<std::pair<int, int>>* path = nullptr);
  /// @brief Apply a median filter to the input data
  /// @param elevation_profile The data to filter
  /// @param filter_width The width of the data to consider for the median
  /// @return The filtered signal
  std::vector<int> MedianFilter(const std::vector<int>& elevation_profile,
                                const int& filter_width);
  /// @brief Apply a simple lowpass filter to the input data
  /// @param elevation_profile The data to filter
  /// @param alpha The coefficient to use in the filter when combining values.
  /// Must be on [0, 1].
  /// @return The filtered signal
  std::vector<int> LowpassFilter(const std::vector<int>& elevation_profile,
                                 const double& alpha);
  /// @brief Apply a mean filter to the input data
  /// @param elevation_profile The data to filter
  /// @param filter_width The width of the data to consider for the mean
  /// @return The filtered signal
  std::vector<int> MeanFilter(const std::vector<int>& elevation_profile,
                              const int& filter_width);
  /// @brief Correct the path by making sure it does not collide with the ground
  /// based on some given threshold
  /// @param elevation_profile The original elevation profile of the map
  /// @param filtered_profile The output of applying a filter to the profile
  /// @param min_alt The minimum value the path must be from the
  /// `elevation_profile`
  /// @return The clipped path that will not collied with the ground
  std::vector<int> CorrectPath(const std::vector<int>& elevation_profile,
                               const std::vector<int>& filtered_profile,
                               const int& min_alt);

 private:
  /// @brief Generate the base elevation profile and path prior to filtering
  /// @param elevation_profile - The output elevation profile from the 
  /// planned path.
  /// @param path - Optional. The full path that was taken in row and 
  /// column coordinates
  /// @return true if a path was successfully planned
  bool GenerateBasePath(std::vector<int>* elevation_profile,
                        std::vector<std::pair<int, int>>* path = nullptr);

  /// The current elevation map to plan a path for
  ElevationMap emap_;
};
}