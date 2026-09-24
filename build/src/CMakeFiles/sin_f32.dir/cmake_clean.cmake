file(REMOVE_RECURSE
  "libsin_f32.a"
  "libsin_f32.pdb"
  "sin_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/sin_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
