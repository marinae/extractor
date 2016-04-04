set(LLVM_LINK_COMPONENTS support)

# TODO: set variables

include_directories(
    include
    )

add_library(matcher STATIC
    src/Matcher.cpp
    )

add_library(typeConversion STATIC
    src/TypeConversion.cpp
    )

add_library(utils STATIC
    src/Utils.cpp
    )

add_clang_executable(extractor
    src/Matcher.cpp
    src/TypeConversion.cpp
    src/Utils.cpp
    src/main.cpp
    )

target_link_libraries(extractor
    clangTooling
    clangBasic
    clangASTMatchers
    matcher
    typeConversion
    utils
    )
