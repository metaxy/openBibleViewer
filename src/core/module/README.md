# How to add a new Module

  + Create .cpp and .h file
  + add to src/core/CMakeLists.txt
  + create new moduletype in src/core/moduletools.h
  + add filematcher in src/core/moduletools.cpp in recognizeModuleType()
  + add translation to moduleTypeNames() and moduleTypeName()
  + add class in typeToClass()
  + add category in getCategory()
  + add creator in newCommentaryModule() in src/core/module.cpp or similiar
  + add header file in  src/core/module.cpp
  + add simple creator in newSimpleModule() in  src/core/module.cpp