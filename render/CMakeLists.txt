project(skyscraper)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -ggdb -g3 -pg -O0")

add_executable(skyscraper main.cpp tgaimage.cpp model.cpp)

install(TARGETS skyscraper RUNTIME DESTINATION bin)
