file(REMOVE_RECURSE
  "libsoftmax_non_fcd_bf16_gaudi2.a"
  "libsoftmax_non_fcd_bf16_gaudi2.pdb"
  "softmax_non_fcd_bf16_gaudi2_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/softmax_non_fcd_bf16_gaudi2.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
