# turtle_monitor

This package contains a demo of a monitor which subscribes to the velocity commands of a turtlesim turtle.

Required Software:
  * Ubuntu 12.04
  * ROS Groovy
  * turtlesim
  * C++11 compatible compiler
  * Boost 1.58 library

How to run the demo:
  * Copy the package in your source folder ( ~/ros/src$ )
  * Open a terminal window, change your directiory to your workspace directory
  * Start roscore:  
  
  ```
 roscore 
  ```

  * In a separate terminal window type the following command: 
  
    ```
    source devel/setup.bash
    ```
  * Build the executables in the root folder of your catkin workspace using the command : 
  
    ```
    catkin_make
    ```  
    or

    ```  
    catkin_make install --only-pkg-with-deps turtle_monitor
    ```

 * By running the following command the launchfile will set up the demo:

    ``` 
    roslaunch turtle_monitor turtle_monitor.launch
    ```

If any of the expressions above are new to you, search them on http://wiki.ros.org/
