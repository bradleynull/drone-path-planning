#include <iostream>
#include <algorithm>

#include "path_planner.h"

using namespace path_planning;

PathPlanner::PathPlanner(const ElevationMap& emap) : emap_(emap) {
}

PathPlanner::~PathPlanner() {
}

bool PathPlanner::PlanPath(std::vector<int>* elevation_profile,
                           std::vector<int>* agl_elevation_profile,
                           const int& agl,
                           std::vector<std::pair<int, int>>* path) {
  // First generate the base path and elevation
  if(!GenerateBasePath(elevation_profile, path)) {
    std::cerr << "PathPlanner::PlanPath: Unable to generate a base path from "
                 "start to finish!"
              << std::endl;
    return false;
  }

  // Set the begining and end to the same as the next/previous since they are
  // marked with negative numbers
  (*elevation_profile)[0] = (*elevation_profile)[1];
  (*elevation_profile)[elevation_profile->size() - 1] =
      (*elevation_profile)[elevation_profile->size() - 2];

  // Filter the elevation profile based on the agl
  *agl_elevation_profile = *elevation_profile;
  for(auto& el : *agl_elevation_profile) {
    el += agl;
  }

  return true;
}

bool PathPlanner::GenerateBasePath(std::vector<int>* elevation_profile,
                                   std::vector<std::pair<int, int>>* path) {
  // Check that we have exactly one start and one end position
  auto start_pos = emap_.GetLocations(kStartPos);
  if(start_pos.size() != 1) {
    std::cerr << "PathPlanner::PlanPath: ERROR! Map has no start position "
              << "or multiple start positions. No path will be planned!"
              << std::endl;
    return false;
  }
  auto end_pos = emap_.GetLocations(kEndPos);
  if(end_pos.size() != 1) {
    std::cerr << "PathPlanner::PlanPath: ERROR! Map has no end position "
              << "or multiple end positions. No path will be planned!"
              << std::endl;
    return false;
  }
  // Define the line between the start and the beginning
  // TODO Replace this with an actual path planning algorithm to minimize 
  //  elevation change cost?
  auto current_pos = start_pos[0];
  if(path) path->emplace_back(current_pos);
  elevation_profile->emplace_back(emap_(current_pos.first, current_pos.second));
  while(current_pos != end_pos[0]) {
    int row_diff = end_pos[0].first - current_pos.first;
    int col_diff = end_pos[0].second - current_pos.second;

    // Give row movement priority over column by checking the magnitude of 
    // the differences
    if(row_diff > 0 && abs(row_diff) >= abs(col_diff)) {
      current_pos.first += 1;
    } else if (row_diff < 0 && abs(row_diff) >= abs(col_diff)) {
      current_pos.first -= 1;
    } else if(col_diff > 0) {
      current_pos.second += 1;
    } else if (col_diff < 0) {
      current_pos.second -= 1;
    }
    if (path) path->emplace_back(current_pos);
    elevation_profile->emplace_back(
      emap_(current_pos.first, current_pos.second));
  }
  return true;
}

std::vector<int> PathPlanner::MedianFilter(
    const std::vector<int>& elevation_profile, const int& filter_width) {
  std::vector<int> filtered_data;
  // Just a slow median filter because we sort sooo much
  for (int i = 0; i < elevation_profile.size(); i++) {
    // Don't try to filter the first values
    if(i < filter_width) {
      filtered_data.emplace_back(elevation_profile[i]);
      continue;
    }

    std::vector<int> filter_vals(elevation_profile.begin() + i - filter_width,
                                 elevation_profile.begin() + i);
    std::sort(filter_vals.begin(), filter_vals.end());
    // Take the average if there's an even number
    if (filter_width % 2 == 0) {
      filtered_data.emplace_back(
          (filter_vals[filter_width / 2 - 1] + filter_vals[filter_width / 2]) /
          2);
    } else {
      filtered_data.emplace_back(filter_vals[std::ceil(filter_width / 2)]);
    }
  }
  return filtered_data;
}

std::vector<int> PathPlanner::LowpassFilter(
    const std::vector<int>& elevation_profile, const double& alpha) {
  if(alpha < 0.0 || alpha > 1.0) {
    std::cerr << "PathPlanner::LowpassFilter: Alpha must be between 0 and 1!"
              << std::endl;
    return elevation_profile;
  }
  std::vector<int> filtered_data{elevation_profile[1]};
  for (int i = 1; i < elevation_profile.size(); i++) {
    filtered_data.emplace_back(int(alpha * elevation_profile[i] +
                                   (1.0 - alpha) * filtered_data.back()));
  }

  return filtered_data;
}