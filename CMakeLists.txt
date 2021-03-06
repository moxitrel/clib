cmake_minimum_required(VERSION 3.13)

project(Cogo)
add_library(cogo)
target_sources(cogo PRIVATE co_st.c)

if (PROJECT_NAME STREQUAL CMAKE_PROJECT_NAME)
    include(CTest)
    if (BUILD_TESTING)
        include(GoogleTest)
        find_package(GTest REQUIRED)

        add_compile_options(-Weverything
                -Wno-c99-extensions
                -Wno-unused-label
                -Wno-unreachable-code
                -Wno-gnu-label-as-value
                -Wno-gnu-zero-variadic-macro-arguments

                -Wno-old-style-cast
                -Wno-zero-as-null-pointer-constant
                -Wno-c++98-compat
                -Wno-c++98-compat-pedantic
                -Wno-padded
                -Wno-poison-system-directories
                -Wno-global-constructors        # google-test
                -Wno-missing-prototypes         # unit test
        )
        link_libraries(GTest::GTest GTest::Main)

        # yield_case
        add_executable(yield_case_test)
        target_sources(yield_case_test
                PRIVATE yield_test.cpp)
        target_compile_features(yield_case_test
                PRIVATE cxx_std_11)
        target_compile_definitions(yield_case_test
                PRIVATE COGO_CASE)
        gtest_discover_tests(yield_case_test)

        # yield_label_value
        add_executable(yield_label_value_test)
        target_sources(yield_label_value_test
                PRIVATE yield_test.cpp)
        target_compile_features(yield_label_value_test
                PRIVATE cxx_std_11)
        target_compile_definitions(yield_case_test
                PRIVATE COGO_LABEL_VALUE)
        gtest_discover_tests(yield_label_value_test)

        # co
        add_executable(co_test)
        target_sources(co_test
                PRIVATE co_test.cpp)
        target_compile_features(co_test
                PRIVATE cxx_std_14)
        gtest_discover_tests(co_test)

        # co_st
        add_executable(co_st_test)
        target_sources(co_st_test
                PRIVATE co_st_test.cpp)
        target_compile_features(co_st_test
                PRIVATE cxx_std_11)
        gtest_discover_tests(co_st_test)

    endif ()
endif ()
