file(REMOVE_RECURSE
  "kl_div_fwd_f32_x86.o"
  "libkl_div_fwd_f32.a"
  "libkl_div_fwd_f32.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/kl_div_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
