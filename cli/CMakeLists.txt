add_executable(alpx
    main.cpp
)

target_link_libraries(alpx
    PRIVATE
        alps
)

target_compile_features(alpx PRIVATE
    cxx_std_20
)

target_compile_options(alpx PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -O3
)
