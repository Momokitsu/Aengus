file(GLOB_RECURSE AENGUS_SOURCES CONFIGURE_DEPENDS *.cpp *.cc *.c)
file(GLOB_RECURSE AENGUS_HEADERS CONFIGURE_DEPENDS *.h *.hpp)

add_library(aengus STATIC ${AENGUS_SOURCES} ${AENGUS_HEADERS})


# 头文件包含路径
target_include_directories(aengus
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(aengus PUBLIC aengus_options) # 继承所有编译/链接选项

target_compile_options(aengus PRIVATE
    # 主库可能需要更严格的警告
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Werror          # 将警告视为错误（主库严格要求）
    >
)

# 安装配置
install(TARGETS aengus
    EXPORT aengusTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES ${AENGUS_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDER}/aengus
)