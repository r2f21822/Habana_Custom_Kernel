file(REMOVE_RECURSE
  "cast_f32_to_bf16_x86.o"
  "libcast_f32_to_bf16.a"
  "libcast_f32_to_bf16.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/cast_f32_to_bf16.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
