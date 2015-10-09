project(Lua)

message(STATUS "Lua Project")
set(SRCS
	./lapi.c
	./lapi.h
	./lauxlib.c
	./lauxlib.h
	./lbaselib.c
	./lbitlib.c
	./lcode.c
	./lcode.h
	./lcorolib.c
	./lctype.c
	./lctype.h
	./ldblib.c
	./ldebug.c
	./ldebug.h
	./ldo.c
	./ldo.h
	./ldump.c
	./lfunc.c
	./lfunc.h
	./lgc.c
	./lgc.h
	./linit.c
	./liolib.c
	./llex.c
	./llex.h
	./llimits.h
	./lmathlib.c
	./lmem.c
	./lmem.h
	./loadlib.c
	./lobject.c
	./lobject.h
	./lopcodes.c
	./lopcodes.h
	./loslib.c
	./lparser.c
	./lparser.h
	./lprefix.h
	./lstate.c
	./lstate.h
	./lstring.c
	./lstring.h
	./lstrlib.c
	./ltable.c
	./ltable.h
	./ltablib.c
	./ltm.c
	./ltm.h
	./lua.c
	./lua.h
	./lua.hpp
	#./luac.c
	./luaconf.h
	./lualib.h
	./lundump.c
	./lundump.h
	./lutf8lib.c
	./lvm.c
	./lvm.h
	./lzio.c
	./lzio.h
)

source_group("Src" FILES ${SRCS})
include_directories(.)

# lua ���ʺ�ʹ�� VS ���� DLL ����Ϊ VS ���Ҫ���� Lua DLL ����Ҫһ�� def �ļ����У�������б��뾯�档VS ������������� LUA_LIB ���� LUA_CORE ���ͻ��о��棬��������壬#define LUA_API __declspec(dllimport) ����˶�������
# Lua ����� Dll�����忴 luaconf.h�� LUA_CORE �Ѿ���  Lua Դ�������ж����ˣ����ʹ�� LUA_LIB 
#ADD_DEFINITIONS(-DLUA_BUILD_AS_DLL -DLUA_CORE)
#ADD_DEFINITIONS(-DLUA_BUILD_AS_DLL -DLUA_LIB)
#ADD_LIBRARY(Lua SHARED ${SRCS})
ADD_LIBRARY(Lua STATIC ${SRCS})
set_target_properties(Lua PROPERTIES DEBUG_POSTFIX _d)


# �����ļ�
file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua.hpp DESTINATION "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lua.h DESTINATION "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lualib.h DESTINATION "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lauxlib.h DESTINATION "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/luaconf.h DESTINATION "${CMAKE_SOURCE_DIR}/../Inc/${PROJECT_NAME}")

file(REMOVE_RECURSE "${CMAKE_SOURCE_DIR}/../Lib/${PROJECT_NAME}")
file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/../Lib/${PROJECT_NAME}")

install(FILES 
	${CMAKE_CURRENT_BINARY_DIR}/Debug/Lua_d.lib
	DESTINATION "${CMAKE_SOURCE_DIR}/../Lib/${PROJECT_NAME}" CONFIGURATIONS Debug
)
install(FILES 
	${CMAKE_CURRENT_BINARY_DIR}/Release/Lua.lib
	DESTINATION "${CMAKE_SOURCE_DIR}/../Lib/${PROJECT_NAME}" CONFIGURATIONS Release None ""
)
install(FILES 
	${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo/Lua.lib
	DESTINATION "${CMAKE_SOURCE_DIR}/../Lib/${PROJECT_NAME}" CONFIGURATIONS RelWithDebInfo
)
install(FILES 
	${CMAKE_CURRENT_BINARY_DIR}/MinSizeRel/Lua.lib
	DESTINATION "${CMAKE_SOURCE_DIR}/../Lib/${PROJECT_NAME}" CONFIGURATIONS MinSizeRel
)