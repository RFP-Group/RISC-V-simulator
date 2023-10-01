find_package(Ruby REQUIRED)

function(gen_file)
    set(singlevalues GENERATOR TEMPLATE OUTPUT)
	set(multiplevalues REQUIES)
    cmake_parse_arguments(
        ARG
        ""
        "${singlevalues}"
		"${multiplevalues}"
		${ARGN}
    )
    set(DEPENDS_LIST ${ARG_GENERATOR} ${ARG_TEMPLATE})
	add_custom_command(OUTPUT ${ARG_OUTPUT}
        COMMENT "Generate file for ${ARG_TEMPLATE}"
        COMMAND ${Ruby_EXECUTABLE} ${ARG_GENERATOR} --root ${PROJECT_ROOT} --template ${ARG_TEMPLATE} --output ${ARG_OUTPUT}
        DEPENDS ${DEPENDS_LIST}
    )
endfunction()