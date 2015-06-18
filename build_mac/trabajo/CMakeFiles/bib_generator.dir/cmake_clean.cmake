file(REMOVE_RECURSE
  "bib_generator.pdb"
  "bib_generator"
)

# Per-language clean rules from dependency scanning.
foreach(lang)
  include(CMakeFiles/bib_generator.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
