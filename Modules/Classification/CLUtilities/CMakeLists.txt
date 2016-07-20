MITK_CREATE_MODULE(
  DEPENDS MitkCore MitkCLCore
  PACKAGE_DEPENDS 
    PUBLIC Eigen
    PUBLIC  ITK|ITKImageLabel+ITKConnectedComponents+ITKImageCompare+ITKMathematicalMorphology+ITKBinaryMathematicalMorphology
  WARNINGS_AS_ERRORS
)

add_subdirectory(test)
