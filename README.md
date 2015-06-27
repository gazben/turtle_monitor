# turtle_monitor

This package contains a demo of a monitor which subscribes to the velocity commands of a turtlesim turtle.

To run the package, you must have an Ubuntu 12.04 operating system and ROS-groovy installed on your workstation.

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
  catkin_make --only-pkg-with-deps turtle_monitor
  ```

 - By running the following command the launchfile will set up the demo:

  ``` 
  roslaunch turtle_monitor demo.launch
  ```

If any of the expressions above are new to you, search them on http://wiki.ros.org/
