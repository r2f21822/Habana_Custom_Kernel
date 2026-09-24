file(REMOVE_RECURSE
  "filter_fwd_2d_bf16_x86.o"
  "libfilter_fwd_2d_bf16.a"
  "libfilter_fwd_2d_bf16.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/filter_fwd_2d_bf16.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
