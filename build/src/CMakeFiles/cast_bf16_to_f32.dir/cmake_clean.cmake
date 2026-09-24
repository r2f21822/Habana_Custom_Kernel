file(REMOVE_RECURSE
  "cast_bf16_to_f32_x86.o"
  "libcast_bf16_to_f32.a"
  "libcast_bf16_to_f32.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/cast_bf16_to_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
