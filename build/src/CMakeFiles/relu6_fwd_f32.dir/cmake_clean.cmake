file(REMOVE_RECURSE
  "librelu6_fwd_f32.a"
  "librelu6_fwd_f32.pdb"
  "relu6_fwd_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/relu6_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
