file(REMOVE_RECURSE
  "../../qml/dontSlouch/Constants.qml"
  "../../qml/dontSlouch/DirectoryFontLoader.qml"
  "../../qml/dontSlouch/EventListModel.qml"
  "../../qml/dontSlouch/EventListSimulator.qml"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/dontSlouch_tooling.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
