set(Test_SRCS
    main.cpp
)

include_directories(${PCL_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS} ./testSetup ./testDef ./testExecution ./testEval ./testUtil)

add_executable (Test_Exe ${Test_SRCS} ./testSetup/testSetup.cpp 
                                      ./testExecution/testExecution.cpp
                                      ./testEval/testEval.cpp
                                      ./testUtil/testUtil.cpp)

target_link_libraries(Test_Exe ${PCL_LIBRARIES} ${catkin_LIBRARIES})

install(TARGETS Test_Exe RUNTIME DESTINATION bin)