file(REMOVE_RECURSE
  "libspatial_conv_f32.a"
  "libspatial_conv_f32.pdb"
  "spatial_conv_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/spatial_conv_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
