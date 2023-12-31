cmake_minimum_required(VERSION 3.26)
project(Udacity C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED true)

file (GLOB_RECURSE SourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.c" )
message(STATUS ${SourceFiles})

file (GLOB_RECURSE HeaderFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" )
message(STATUS ${HeaderFiles})


# Full disclosure: I originally preferred the globbing approach for its simplicity, but over the years I have come to recognise that explicitly listing the files is less error-prone for large, multi-developer projects.

# Original answer:

# The advantages to globbing are:

# It's easy to add new files as they are only listed in one place: on disk. Not globbing creates duplication.

# Your CMakeLists.txt file will be shorter. This is a big plus if you have lots of files. Not globbing causes you to lose the CMake logic amongst huge lists of files.

# The advantages of using hardcoded file lists are:

# CMake will track the dependencies of a new file on disk correctly - if we use glob then files not globbed first time round when you ran CMake will not get picked up

# You ensure that only files you want are added. Globbing may pick up stray files that you do not want.

# In order to work around the first issue, you can simply "touch" the CMakeLists.txt that does the glob, either by using the touch command or by writing the file with no changes. This will force CMake to re-run and pick up the new file.

# To fix the second problem you can organize your code carefully into directories, which is what you probably do anyway. In the worst case, you can use the list(REMOVE_ITEM) command to clean up the globbed list of files:

# file(GLOB to_remove file_to_remove.cpp)
# list(REMOVE_ITEM list ${to_remove})
# The only real situation where this can bite you is if you are using something like git-bisect to try older versions of your code in the same build directory. In that case, you may have to clean and compile more than necessary to ensure you get the right files in the list. This is such a corner case, and one where you already are on your toes, that it isn't really an issue.
set(to_be_removed "Build/CMakeFiles/3.26.3/CompilerIdC/CMakeCCompilerId.c")
list(REMOVE_ITEM SourceFiles ${to_be_removed})

add_executable(${PROJECT_NAME} ${SourceFiles})

include_directories(CARD/ TERMINAL/ SERVER/)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/)