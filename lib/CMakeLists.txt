
add_library(WARFinder SHARED WARFinder.cpp)

target_link_libraries(
    WARFinder
    "$<$<PLATFORM_ID:Darwin>:-undefined dynamic_lookup>"
)
