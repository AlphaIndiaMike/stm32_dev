set(cmsis_component_f4xx_source                      
    Source/Templates/gcc/startup_stm32f411xe.s
    #Source/Templates/system_stm32f4xx.c
)

set_source_files_properties(
  Source/Templates/gcc/startup_stm32f411xe.s
  PROPERTIES LANGUAGE ASM
)

set_source_files_properties( ${cmsis_component_f4xx_source}
    COMPILE_FLAGS "${DEFAULT_COMPILE_OPTIONS_M4}")

add_library(cmsis_f4xx_component OBJECT ${cmsis_component_f4xx_source})

target_include_directories( cmsis_f4xx_component PRIVATE
    ${cmsis_directory_f4xx}/../arm/Core/Include
    Include
)