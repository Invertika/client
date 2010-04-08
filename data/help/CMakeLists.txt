SET(FILES
    about.txt
    commands.txt
    header.txt
    index.txt
    skills.txt
    support.txt
    team.txt
	windows.txt
    )

INSTALL(FILES ${FILES} DESTINATION ${DATA_DIR}/help)
