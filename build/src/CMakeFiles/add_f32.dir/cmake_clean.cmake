file(REMOVE_RECURSE
  "add_f32_x86.o"
  "libadd_f32.a"
  "libadd_f32.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/add_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
