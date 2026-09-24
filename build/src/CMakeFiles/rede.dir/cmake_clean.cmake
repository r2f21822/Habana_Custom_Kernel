file(REMOVE_RECURSE
  "librede.a"
  "librede.pdb"
  "rede_x86.o"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/rede.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
