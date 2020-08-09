#include "elevation_map.h"
#include "path_planner.h"

#include <iostream>

namespace pp = path_planning;

bool plan_path() {
  pp::ElevationMap emap;
  if (!emap.ReadMap("example_data/small_map.txt")) {
    std::cerr << "plan_path:: Error reading map." << std::endl;
    return false;
  }

  pp::PathPlanner planner(emap);

  std::vector<int> el_profile;
  std::vector<std::pair<int, int>> path;
  if(!planner.PlanPath(&el_profile, &path)) {
      return false;
  }

  // Expected path for the small map
  std::vector<int> expected_el_profile = {-1, 121, 122, -2};
  std::vector<std::pair<int, int>> expected_path = {
    std::make_pair(0, 1),
    std::make_pair(1, 1),
    std::make_pair(2, 1),
    std::make_pair(2, 2)
  };

  if(expected_el_profile != el_profile) {
    std::cout << "Elevation profiles did not match!" << std::endl;
    return false;
  }

  if(expected_path != path) {
    std::cout << "Paths did not match!" << std::endl;
    return false;
  }

  return true;
}

int main(int argc, char** argv) {
  if (!plan_path()) {
    return -1;
  }
  std::cout << "All path planning tests passed!" << std::endl;
  return 0;
}