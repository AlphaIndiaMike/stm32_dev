set (DEFAULT_COMPILE_OPTIONS_M7 ${DEFAULT_COMPILE_OPTIONS_M7}
        "-mcpu=cortex-m7 \\"
        "-mthumb \\"
        "-mfpu=fpv4-sp-d16 \\"
        "-mfloat-abi=hard \\"
        "-fdata-sections \\"
        "-ffunction-sections \\"
        "-Wdouble-promotion \\"
        "-Wall \\"
        "-Ofast \\"
        ""
    )
string (REPLACE ";" " " DEFAULT_COMPILE_OPTIONS_M7 ${DEFAULT_COMPILE_OPTIONS_M7})

set (DBG_COMPILE_OPTIONS_M7 ${DBG_COMPILE_OPTIONS_M7}
        "-mcpu=cortex-m7 \\"
        "-mthumb \\"
        "-mfpu=fpv4-sp-d16 \\"
        "-mfloat-abi=hard \\"
        "-fdata-sections \\"
        "-ffunction-sections \\"
        "-Wdouble-promotion \\"
        "-Wall \\"
        "-Og \\"
        ""
    )
string (REPLACE ";" " " DBG_COMPILE_OPTIONS_M7 ${DBG_COMPILE_OPTIONS_M7})

set (DEFAULT_COMPILE_OPTIONS_M4 ${DEFAULT_COMPILE_OPTIONS_M4}
        "-mcpu=cortex-m4 \\"
        "-mthumb \\"
        "-mfpu=fpv4-sp-d16 \\"
        "-mfloat-abi=hard \\"
        "-fdata-sections \\"
        "-ffunction-sections \\"
        "-Wdouble-promotion \\"
        "-Wall \\"
        "-Ofast \\"
        ""
    )
string (REPLACE ";" " " DEFAULT_COMPILE_OPTIONS_M4 ${DEFAULT_COMPILE_OPTIONS_M4})

set (DBG_COMPILE_OPTIONS_M4 ${DBG_COMPILE_OPTIONS_M4}
        "-mcpu=cortex-m4 \\"
        "-mthumb \\"
        "-mfpu=fpv4-sp-d16 \\"
        "-mfloat-abi=hard \\"
        "-fdata-sections \\"
        "-ffunction-sections \\"
        "-Wdouble-promotion \\"
        "-Wall \\"
        "-Og \\"
        ""
    )
string (REPLACE ";" " " DBG_COMPILE_OPTIONS_M4 ${DBG_COMPILE_OPTIONS_M4})