file(REMOVE_RECURSE
  "librelu_bwd_f32_gaudi2.a"
  "librelu_bwd_f32_gaudi2.pdb"
  "relu_bwd_f32_gaudi2_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/relu_bwd_f32_gaudi2.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
