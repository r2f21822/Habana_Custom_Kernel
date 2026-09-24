file(REMOVE_RECURSE
  "batch_norm_fwd_f32_x86.o"
  "libbatch_norm_fwd_f32.a"
  "libbatch_norm_fwd_f32.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/batch_norm_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
