#include <iostream>

#include "elevation_map.h"

bool read_map() {
  path_planning::ElevationMap emap;
  if(!emap.ReadMap("example_data/small_map.txt")) {
    return false;
  }

  std::vector<std::vector<int>> small_map = {
      {123, path_planning::kSpecialLocations.at('A'), 121, 120},
      {122, 121, 120, 121},
      {122, 122, path_planning::kSpecialLocations.at('B'), 120},
      {124, 123, 122, 121}};
    
  for(int row = 0; row < small_map.size(); row++) {
    for(int col = 0; col < small_map[row].size(); col++) {
      if(emap(row, col) != small_map[row][col]) {
        std::cout << "Parsed map does not match test map at " << row << ", "
                  << col << std::endl;
        return false;
      }
    }
  }

  if(*emap.GetLocations('A').begin() != std::make_pair(0, 1)) {
    std::cout << "Unable to find location for A in map" << std::endl;
    return false;
  }

  if(*emap.GetLocations('B').begin() != std::make_pair(2, 2)) {
    std::cout << "Unable to find location for A in map" << std::endl;
    return false;
  }

  return true;
}

int main(int argc, char** argv) {
  if(!read_map()) {
    return -1;
  }
  std::cout << "All map read tests passed!" << std::endl;
  return 0;
}