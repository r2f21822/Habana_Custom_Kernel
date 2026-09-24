file(REMOVE_RECURSE
  "libprintf_test.a"
  "libprintf_test.pdb"
  "printf_test_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/printf_test.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
