add_library(ui STATIC
    dialog/nodesummary/nodesummary.cpp
    dialog/nodesummary/nodesummary.ui
    dialog/iosummary/iosummary.cpp
    dialog/iosummary/iosummary.ui
    sidebar/findsidebar/findsidebar.cpp
    sidebar/findsidebar/findsidebar.ui
)

target_link_libraries(ui
    PUBLIC

    Qt6::Widgets
    utils_algorithm
)
