file(REMOVE_RECURSE
  "librelu6_fwd_bf16.a"
  "librelu6_fwd_bf16.pdb"
  "relu6_fwd_bf16_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/relu6_fwd_bf16.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
