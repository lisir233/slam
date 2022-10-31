execute_process(COMMAND "/home/loop/ros_workspace/cartographer_detailed_comments_ws/build_isolated/launch_tools/catkin_generated/python_distutils_install.sh" RESULT_VARIABLE res)

if(NOT res EQUAL 0)
  message(FATAL_ERROR "execute_process(/home/loop/ros_workspace/cartographer_detailed_comments_ws/build_isolated/launch_tools/catkin_generated/python_distutils_install.sh) returned error code ")
endif()
