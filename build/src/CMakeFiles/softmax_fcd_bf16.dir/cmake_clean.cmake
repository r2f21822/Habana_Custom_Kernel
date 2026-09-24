file(REMOVE_RECURSE
  "libsoftmax_fcd_bf16.a"
  "libsoftmax_fcd_bf16.pdb"
  "softmax_fcd_bf16_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/softmax_fcd_bf16.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
