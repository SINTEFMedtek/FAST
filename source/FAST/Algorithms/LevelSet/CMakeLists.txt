fast_add_sources(
        LevelSetSegmentation.cpp
        LevelSetSegmentation.hpp
)
if(FAST_MODULE_Visualization)
fast_add_test_sources(
        LevelSetTests.cpp
)
endif()
fast_add_process_object(LevelSetSegmentation LevelSetSegmentation.hpp)