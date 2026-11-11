# MaxsuCombatEscape.dll (Better Combat Escape - NG, fork) on alandtse's CommonLibSSE-NG (Skyrim SE and AE incl. 1.7.x, no VR);
# included by E:/WorkSpace/ng-build/CMakeLists.txt (see ng_plugin there). That build has ENABLE_SKYRIM_VR OFF, so the virtual calls on
# RE::Actor (IsInCombat, StopCombat) compile to the SE/AE vtable slots 0xE3/0xE5. The repository's own CMake (cmake/XSEPlugin.cmake)
# builds the library with VR ON; see include/PCH.h for the upstream bug that this fork fixes.
set(_mce_ver "1.0.5")
set(_mce_saved_project_name "${PROJECT_NAME}")
set(PROJECT_NAME "MaxsuCombatEscape")
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 5)
set(_mce_gen "${CMAKE_BINARY_DIR}/maxsucombatescape-gen")
configure_file("${CMAKE_CURRENT_LIST_DIR}/cmake/Plugin.h.in" "${_mce_gen}/Plugin.h" @ONLY)
set(PROJECT_NAME "${_mce_saved_project_name}")

ng_plugin(TARGET MaxsuCombatEscape NAME MaxsuCombatEscape VERSION ${_mce_ver} AUTHOR "doodlum (fork CageTV)" ROOT "${CMAKE_CURRENT_LIST_DIR}"
    SOURCES src/CombatEsc_DataHandler.cpp src/CombatEsc_Events.cpp src/CombatEsc_Functinos.cpp src/CombatEsc_LoadGameFunc.cpp src/XSEPlugin.cpp
    INCLUDES "${CMAKE_CURRENT_LIST_DIR}/include" "${_mce_gen}"
    PCH "${CMAKE_CURRENT_LIST_DIR}/include/PCH.h"
    DEFINES MCE_NG)
