file(REMOVE_RECURSE
  "librelu_bwd_f32.a"
  "librelu_bwd_f32.pdb"
  "relu_bwd_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/relu_bwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
