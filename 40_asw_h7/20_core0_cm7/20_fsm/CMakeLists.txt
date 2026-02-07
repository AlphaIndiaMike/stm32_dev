#[[-----------------------------------
Module: 20_fsm
-------------------------------------]]

set(component_name 20_fsm)
set(component_src ${component_name}_source)

message("Compiling ${component_name}")

#[[-----------------------------------
Build setup
-------------------------------------]]

set(${component_src}
    # Here we place the source files: e.g.: Src/main.c
)

set_source_files_properties(${${component_src}}
    COMPILE_FLAGS "${SYSTEM_COMPILE_OPTIONS}")

add_library(${component_name} OBJECT ${${component_src}})

target_compile_definitions(${component_name} PRIVATE
    ${TARGET_COMPILE_DEFS}
)

target_include_directories(${component_name} PRIVATE
    # Here we place the includes for component e.g.: Inc
)

#[[-----------------------------------
END
-------------------------------------]]