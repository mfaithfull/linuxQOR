cmake_minimum_required(VERSION 3.10.0)
project(QOR VERSION 0.1.0 LANGUAGES C CXX)

set(target qor_tui)

add_library(${target} SHARED qor_tui.cpp
screen/colourinfo.cpp
screen/colour.cpp
screen/terminal.cpp
screen/box.cpp
screen/pixel.cpp
screen/image.cpp
screen/string.cpp
screen/screen.cpp
screen/termscreen.cpp
dom/util.cpp
dom/vbox.cpp
dom/underlineddouble.cpp
dom/underlined.cpp
dom/text.cpp
dom/table.cpp
dom/strikethrough.cpp
dom/spinner.cpp
dom/size.cpp
dom/separator.cpp
dom/selectionstyle.cpp
dom/selection.cpp
dom/scrollindicator.cpp
dom/reflect.cpp
dom/paragraph.cpp
dom/nodedecorator.cpp
dom/node.cpp
dom/lineargradient.cpp
dom/italic.cpp
dom/inverted.cpp
dom/hyperlink.cpp
dom/hbox.cpp
dom/gridbox.cpp
dom/graph.cpp
dom/gauge.cpp
dom/frame.cpp
dom/focus.cpp
dom/flexboxhelper.cpp
dom/flexboxconfig.cpp
dom/flexbox.cpp
dom/flex.cpp
dom/dim.cpp
dom/dbox.cpp
dom/compositedecorator.cpp
dom/colour.cpp
dom/clearunder.cpp
dom/canvas.cpp
dom/box_helper.cpp
dom/border.cpp
dom/bold.cpp
dom/blink.cpp
dom/automerge.cpp
ui/animation.cpp
ui/component.cpp
ui/componentoptions.cpp
ui/container.cpp
ui/event.cpp
ui/input.cpp
ui/inputparser.cpp
ui/loop.cpp
ui/renderer.cpp
ui/screeninteractive.cpp
ui/task.cpp
ui/taskqueue.cpp
ui/taskrunner.cpp
ui/util.cpp
ui/maybe.cpp
ui/catchevent.cpp
widgets/button.cpp
widgets/checkbox.cpp
widgets/collapsible.cpp
widgets/dropdown.cpp
widgets/hoverable.cpp
widgets/menu.cpp
widgets/modal.cpp
widgets/radiobox.cpp
widgets/resizablesplit.cpp
widgets/slider.cpp
widgets/toggle.cpp
widgets/window.cpp
)

include ("${CMAKE_SOURCE_DIR}/cmake/flags.cmake")

target_compile_definitions(${target} PRIVATE UNICODE QOR_TUI)
target_link_libraries(${target} qor_module qor_datastructures qor_objectcontext qor_role qor_application qor_workflow qor_signals qor_error
qor_property)

include ("${CMAKE_SOURCE_DIR}/cmake/postbuild.cmake")

