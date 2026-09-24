file(REMOVE_RECURSE
  "libsearchsorted_fwd_f32.a"
  "libsearchsorted_fwd_f32.pdb"
  "searchsorted_fwd_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/searchsorted_fwd_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
