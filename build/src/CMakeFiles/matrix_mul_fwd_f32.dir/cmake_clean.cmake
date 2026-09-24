file(REMOVE_RECURSE
  "libmatrix_mul_fwd_f32.a"
  "libmatrix_mul_fwd_f32.pdb"
  "matrix_mul_fwd_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/matrix_mul_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
