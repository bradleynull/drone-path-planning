add_library(drone_path_planning STATIC
    elevation_map.h
    elevation_map.cc
    path_planner.h
    path_planner.cc
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_include_directories(drone_path_planning PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)