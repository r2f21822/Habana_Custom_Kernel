file(REMOVE_RECURSE
  "leakyrelu_f32_gaudi_x86.o"
  "libleakyrelu_f32_gaudi.a"
  "libleakyrelu_f32_gaudi.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/leakyrelu_f32_gaudi.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
