SET(target_CPP
${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
)
SET(target_INCLUDE
#${CMAKE_CURRENT_SOURCE_DIR}/include/Avatar.h
)
SET(target_INCLUDE_DIR
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

FIND_PACKAGE(MsgPack REQUIRED)
ADD_DEFINITIONS(
  -D_CRT_SECURE_NO_WARNINGS
  -D_LIB
)

#SEGS_ADD_COMPONENT(Auth SOURCE)

INCLUDE_DIRECTORIES(
        ${MsgPack_INCLUDE_DIR}
        ${target_INCLUDE_DIR}
        ${ACE_path}
)

SET (target_SOURCES
${target_CPP}
${target_INCLUDE}
)

IF(WIN32)
        ADD_EXECUTABLE( segs_client_example_01_cli ${target_SOURCES} )
        SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:console")
        TARGET_LINK_LIBRARIES(segs_client_example_01_cli ws2_32)
ELSE(WIN32)
        ADD_EXECUTABLE( segs_client_example_01_cli ${target_SOURCES} )
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(segs_client_example_01_cli segs_common
                                                ${ZLIB_LIBRARIES}
                                                ${CMAKE_THREAD_LIBS_INIT}
                                                ${MsgPack_LIBRARY}
                                                ${ACE_LIBRARIES}
                                                )

ADD_DEPENDENCIES(segs_client_example_01_cli authserver_lib segs_common gameserver_lib)
