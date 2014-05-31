set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(${Qt5Widgets_DEFINITIONS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

set(SOURCES	main.cpp
        MainWindow.cpp
		Person.cpp
		Group.cpp
		GroupList.cpp
		GroupListModel.cpp
		GroupListDialog.cpp
		Receipt.cpp
		ConvertToAmount.cpp
		Payee.cpp
		ReceiptUI.cpp
		AddPayerDialog.cpp
		ReceiptCollection.cpp
		ReceiptPayerModel.cpp
		ReceiptCollectionModel.cpp)
set(HEADERS	MainWindow.h
		Person.h
		Group.h
		GroupList.h
		GroupListModel.h
		GroupListDialog.h
		Receipt.h
		ConvertToAmount.h
		Payee.h
		QStringSerialize.h
		ReceiptUI.h
		AddPayerDialog.h
		ReceiptCollection.h
		ReceiptPayerModel.h
		ReceiptCollectionModel.h)

qt5_wrap_ui(UI_HDRS	MainWindow.ui
			GroupList.ui
			GroupListDialog.ui
			ReceiptUI.ui
			AddPayerDialog.ui)
add_executable(BillsSplitter ${SOURCES} ${HEADERS} ${UI_HDRS})
target_link_libraries(BillsSplitter ${Qt5Widgets_LIBRARIES})

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS serialization date_time locale thread system)

target_link_libraries(BillsSplitter ${Boost_LIBRARIES})