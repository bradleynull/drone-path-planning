#include "elevation_map.h"
#include "path_planner.h"

#include <iostream>
#include <iomanip>
#include <fstream>

namespace pp = path_planning;

bool plan_path() {
  pp::ElevationMap emap;
  if (!emap.ReadMap("example_data/small_map.txt")) {
    std::cerr << "plan_path:: Error reading map." << std::endl;
    return false;
  }

  pp::PathPlanner planner(emap);

  std::vector<int> el_profile;
  std::vector<int> agl_el_profile;
  std::vector<std::pair<int, int>> path;
  if(!planner.PlanPath(&el_profile, &agl_el_profile, 0, &path)) {
      return false;
  }

  // Expected path for the small map
  std::vector<int> expected_el_profile = {121, 121, 122, 122};
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

bool plan_large_path() {
  pp::ElevationMap emap;
  if(!emap.ReadMap("example_data/test_map.txt")) {
    std::cerr << "plan_large_path: ERROR! Unable to parse map." << std::endl;
    return false;
  }

  pp::PathPlanner planner(emap);
  std::vector<int> el_profile;
  std::vector<int> agl_el_profile;
  std::vector<std::pair<int, int>> path;
  if(!planner.PlanPath(&el_profile, &agl_el_profile, 0, &path)) {
    return false;
  }

  for(auto pt : path) {
    emap(pt.first, pt.second) = 0;
  }
  emap(path[0].first, path[0].second) = -111;
  emap(path.back().first, path.back().second) = -222;

  for(int i = 0; i < emap.rows(); i++) {
    for(int j = 0; j < emap.cols(); j++) {
      std::cout << std::setw(6) << emap(i, j);
    }
    std::cout << std::endl;
  }

  return true;
}

bool filter_large_path() {
  pp::ElevationMap emap;
  if(!emap.ReadMap("example_data/test_map.txt")) {
    std::cerr << "plan_large_path: ERROR! Unable to parse map." << std::endl;
    return false;
  }

  pp::PathPlanner planner(emap);
  std::vector<int> el_profile;
  std::vector<int> agl_el_profile;
  std::vector<std::pair<int, int>> path;
  if(!planner.PlanPath(&el_profile, &agl_el_profile, 20, &path)) {
    return false;
  }
  std::vector<int> med3_filt_profile = planner.CorrectPath(
      el_profile, planner.MedianFilter(agl_el_profile, 3), 5);
  std::vector<int> med9_filt_profile = planner.CorrectPath(
      el_profile, planner.MedianFilter(agl_el_profile, 9), 5);
  std::vector<int> lp005_filt_profile = planner.CorrectPath(
      el_profile, planner.LowpassFilter(agl_el_profile, 0.05), 5);
  std::vector<int> lp02_filt_profile = planner.CorrectPath(
      el_profile, planner.LowpassFilter(agl_el_profile, 0.2), 5);
  std::vector<int> mean3_filt_profile =
      planner.CorrectPath(el_profile, planner.MeanFilter(agl_el_profile, 3), 5);
  std::vector<int> mean9_filt_profile =
      planner.CorrectPath(el_profile, planner.MeanFilter(agl_el_profile, 9), 5);

  // output as json for easy parsing
  std::ofstream out_file;
  std::string test_filename = "./filter_large_path.json";
  out_file.open(test_filename);
  auto write_path = [&out_file](const std::string& path_name, const std::vector<int>& path) {
    out_file << "{\"name\":\"" << path_name << "\",";
    out_file << "\"path\":[";
    for(size_t i = 0; i < path.size() - 1; i++) {
      out_file << path[i] << ",";
    }
    out_file << path.back() << "]}";
  };
  out_file << "[";
  write_path("original_path", el_profile);
  out_file << ",";
  write_path("agl_path", agl_el_profile);
  out_file << ",";
  write_path("median_filter_3", med3_filt_profile);
  out_file << ",";
  write_path("median_filter_9", med9_filt_profile);
  out_file << ",";
  write_path("mean_filter_3", mean3_filt_profile);
  out_file << ",";
  write_path("mean_filter_9", mean9_filt_profile);
  out_file << ",";
  write_path("lowpass_0.05", lp005_filt_profile);
  out_file << ",";
  write_path("lowpass_0.2", lp02_filt_profile);
  out_file << "]";
  out_file.close();
  std::cout << "Test files written to: " << test_filename << std::endl;

  return true;
}

int main(int argc, char** argv) {
  if (!plan_path()) {
    return -1;
  }
  if(!plan_large_path()) {
    return -1;
  }
  if(!filter_large_path()) {
    return -1;
  }

  std::cout << "All path planning tests passed!" << std::endl;
  return 0;
}