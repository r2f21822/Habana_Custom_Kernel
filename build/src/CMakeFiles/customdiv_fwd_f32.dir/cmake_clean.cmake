file(REMOVE_RECURSE
  "customdiv_fwd_f32_x86.o"
  "libcustomdiv_fwd_f32.a"
  "libcustomdiv_fwd_f32.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/customdiv_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
