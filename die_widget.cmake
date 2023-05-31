include_directories(${CMAKE_CURRENT_LIST_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/../die_script/die_script.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogtextinfo.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/xdialogprocess.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../FormatDialogs/dialogfindtext.cmake)

set(DIE_WIDGET_SOURCES
    ${DIE_SCRIPT_SOURCES}
    ${DIALOGTEXTINFO_SOURCES}
    ${XDIALOGPROCESS_SOURCES}
    ${DIALOGFINDTEXT_SOURCES}
    ${CMAKE_CURRENT_LIST_DIR}/dialogdiescandirectory.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogdiescandirectory.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogdiescanprocess.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogdiescanprocess.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogsignatures.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogsignatures.ui
    ${CMAKE_CURRENT_LIST_DIR}/die_highlighter.cpp
    ${CMAKE_CURRENT_LIST_DIR}/die_signatureedit.cpp
    ${CMAKE_CURRENT_LIST_DIR}/die_widget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/die_widget.ui
    ${CMAKE_CURRENT_LIST_DIR}/dialogdiesignatureselapsed.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dialogdiesignatureselapsed.ui
    ${CMAKE_CURRENT_LIST_DIR}/dieoptionswidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/dieoptionswidget.ui
    ${CMAKE_CURRENT_LIST_DIR}/diewidgetadvanced.cpp
    ${CMAKE_CURRENT_LIST_DIR}/diewidgetadvanced.ui
)
