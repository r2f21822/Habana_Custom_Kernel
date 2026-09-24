file(REMOVE_RECURSE
  "avg_pool_2d_fwd_f32_x86.o"
  "libavg_pool_2d_fwd_f32.a"
  "libavg_pool_2d_fwd_f32.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/avg_pool_2d_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
