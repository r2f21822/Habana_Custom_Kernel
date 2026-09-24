file(REMOVE_RECURSE
  "libmamba_pscan_bf16_gaudi3.a"
  "libmamba_pscan_bf16_gaudi3.pdb"
  "mamba_pscan_bf16_gaudi3_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/mamba_pscan_bf16_gaudi3.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
