file(REMOVE_RECURSE
  "add_f32_gaudi2_x86.o"
  "libadd_f32_gaudi2.a"
  "libadd_f32_gaudi2.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/add_f32_gaudi2.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
