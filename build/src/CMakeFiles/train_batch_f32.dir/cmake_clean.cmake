file(REMOVE_RECURSE
  "libtrain_batch_f32.a"
  "libtrain_batch_f32.pdb"
  "train_batch_f32_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/train_batch_f32.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
