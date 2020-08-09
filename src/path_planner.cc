#include <iostream>

#include "path_planner.h"

using namespace path_planning;

PathPlanner::PathPlanner(const ElevationMap& emap) : emap_(emap) {
}

PathPlanner::~PathPlanner() {
}

bool PathPlanner::PlanPath(std::vector<int>* elevation_profile,
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
