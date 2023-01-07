add_library(ui STATIC
    dialog/txtsetdialog/txtsetdialog.cpp
    dialog/txtsetdialog/txtsetdialog.ui
    dialog/txtlistsetdialog/txtlistsetdialog.cpp
    dialog/txtlistsetdialog/txtlistsetdialog.ui
    dialog/nodesummary/nodesummary.cpp
    dialog/nodesummary/nodesummary.ui
    dialog/iosummary/iosummary.cpp
    dialog/iosummary/iosummary.ui
)

target_link_libraries(ui
    PUBLIC

    Qt6::Widgets
)
