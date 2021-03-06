NxN 격자 좌표계에서 단일/다수의 청소기 로봇이 목표 좌표로 이동하기 위한 자동화 드라이버입니다.  

# Master Branch

## 설명

Gazebo Sim에서 8대의 로봇 모델을 사용해서 8 puzzle 문제를 해결하는 노드가 들어있습니다.  

[Gazebo 시뮬레이터 영상](https://youtu.be/uGnAFQer308)  
룸바 8대를 가지고 A* 알고리즘을 실행하는 모습을 Gazebo Sim에서 구현하였습니다. 90 Degree 회전을 측정하는 센서의 오차값으로 인해 전달된 명령을 정확하게 수행하지 못하는 **한계**가 있습니다.

## 실행 방법

1. cd ~/catkin_ws/src
2. git clone https://github.com/ysonggit/gazebo_simple
3. cd ~/catkin_ws && catkin_make
4. roscore
5. roslaunch gazebo_simple eight_robots_world.launch
6. rosrun niklasjang_path_planning path_publisher
7. rostopic pub /result std_msgs/String "data: '[ t2,x1,y2,move-down t3,x2,y2,move-left ]'"

# pddl branch

## 설명 

서버에서 연산된 PDDL 결과를 실제 룸바에게 전달하고, 룸바는 전달받은 명령을 수행하는 드라이버가 저장되어 있습니다. 즉, 아래 두 영상의 기능을 수행하는 드라이버가 저장되어 있습니다.

[다중 룸바 단일 명령 수행 영상](https://youtu.be/i-6bfpRTpEc)  
룸바 2대를 가지고 PDDL 알고리즘을 실행하는 모습을 촬영하였습니다.  서버에서 연산된 PDDL 결과를 룸바에세 전송하였을 때 짧은 명령을 수행할 때에는 90도 회전이 정확하게 수행되는 모습입니다. 하지만 아래 링크의 영상처럼 연속된 명령을 수행하면 90도 회전을 측정하는 자이로 센서의 오차값으로 경계선에서 벗어나는 **한계**를 가지고 있습니다.

[다룸 룸바 연속 명령 수행 영상](https://youtu.be/GrBQo4zfoUY)  
룸바 2대를 가지고 PDDL 알고리즘을 실행하는 모습을 촬영하였습니다.  서버에서 연산된 PDDL 결과를 룸바에세 전송하였을 때 짧은 명령을 수행할 때에는 90도 회전이 정확하게 수행되는 모습입니다. 하지만 본 영상처럼 연속된 명령을 수행하면 90도 회전을 측정하는 자이로 센서의 오차값으로 경계선에서 벗어나는 **한계**를 가지고 있습니다.

## 실행 방법

1. cd ~/catkin_ws/src
2. git clone https://github.com/AutonomyLab/create_autonomy
3. git clone https://github.com/niklasjang/path_planning
4. cd ~/catkin_ws && catkin_make
5. cd ~/catkin_ws/src/path_planning
6. git checkout pddl
7. RunChecker를 실행할 machine과 라즈베리파이를 같은 네트워크에 연결
8. 룸바와 라즈페이파이 연결(모니터,키보드,마우스, 보조베터리도 연결)
9. path_planning/launch/ 디렉토리에 들어있는 create_2_r1.launch파일을 create_autonomy/ca_driver/launch/로 복사
10. create_2_r1.launch파일은 첫 번째 룸바가 실행할 노드
11. 2번째 룸바에서 실행하기 위해서는 create_2_r1.launch을 복사해서 create_2_r2.launch으로 이름을 바꾸고 파일 내용의 모든 r1을 r2으로 바꾸기
12. 첫 번째 라즈베리파이에서 roslaunch ca_driver create_2_r1.launch 실행 (H/W 드라이버)
13. 드라이버가 룸바의 상태를 모두 읽고 Ready. 메시지가 터미널에 뜰 때까지 대기(몇 분 걸림)
14. 같은 라즈베리파이에서 roslaunch niklasjang_path_planning r1.launch 실행(PDDL driver)
15. 운용하고 싶은 다른 룸바 및 라즈베리파이에도 12.~14.을 진행. 단 11. 을 수행
16. 서버에서 연산의 결과를 받음.
17. 서버 연동없이 테스트 하기 위해서는  rostopic pub /result std_msgs/String "data: '[ #target1,x1,y2,move-down #target1,x2,y2,move-left ]'"
18. rostopic pub /result std_msgs/String "data: '[ #target2,x1,y2,move-down #target2,x2,y2,move-left ]'"
19. 17~18을 운용하고자 하는 룸바의 수 많은 각 다른 터미널에서 실행. 단 result_receive_checker.cpp의 check 배열의 길이를 이에 맞게 조정.
20. RunChecker를 실행할 다른 Machine에서 rosrun niklasjang_path_planning result_receive_checker
21. 위 영상처럼 동작

## Tips

### Gazebo와 Rviz 연동

Gazebo와 Rviz를 연동하는 방법을 정리하였습니다. [여기](http://moorerobots.com/blog/post/3)의 내용을 요약한 것입니다.  

1. git clone
2. cd ~/catkin_ws/src
3. git clone https://github.com/niklasjang/mybot_ws.git
4. cd mybot_ws
5. catkin_make
6. git checkout navigation
7. pkg_name/src/urdf/mybot.gazebo에서 아래의 코드를 수정합니다.

```txt
#87 gpu_ray → ray
#89 <visualize>false</visualize> -> <visualize>true</visualize>
#115 libgazebo_ros_gpu_laser → libgazebo_ros_laser
```

- Layer로 맵 인식해서 rviz에서 시각화하기

1. roslaunch mybot_gazebo mybot_world.launch
2. roslaunch mybot_navigation gmapping_demo.launch
3. roslaunch mybot_description mybot_rviz_gmapping.launch
4. roslaunch mybot_navigation mybot_teleop.launch

- 인식한 맵 저장하기

1. mybot_ws/src/mybot_navigation에 maps/ 폴더 만들기
2. rosrun map_server map_saver -f ~/mybot_ws/src/mybot_navigation/maps/test_map
3. ls
4. test_map.pgm  test_map.yaml 확인

- 저장한 맵 로드하기

In Terminal 1, launch the Gazebo world  
In Terminal 2, start map building  
In Terminal 3, launch rviz  

1. roslaunch mybot_gazebo mybot_world.launch
2. roslaunch mybot_navigation amcl_demo.launch
3. roslaunch mybot_description mybot_rviz_amcl.launch

### Multi machine ROS 연동

0. 아래의 명령어로 각 Machine에서 IP를 확인.  

```t
hostname -I
```

1. 같은 네트워크 연결(동일 와이파이 or 동일 기기의 핫스팟에 연결)

2. 만약 machine 1의 ip가 192.168.43.46이고, machine 2의 IP는 192.168.43.200라면

```t
#Machine 1에서는 
#Gedit ~/.basrhc
export $ROS_MASTER_URI=http://192.168.43.46:11311
export $ROS_IP=192.168.43.46
```

```t
#Machine 2: 에서는
#Gedit ~/.basrhc
export $ROS_MASTER_URI=http://192.168.43.46:11311
export $ROS_IP=192.168.43.200
```

3. 하나의 Machine에서만 roscore
4. 다른 Machine에서 rosnode list를 입력해서 Master와 연결을 확인


### terminator

terminator를 설치하면 여러 개의 터미널을 한 번에 제어할 수 있어서 편리합니다.

### Script file

스크립트 파일을 작성해두면 킨 명령어를 쉽게 입력할 수 있습니다.  

아래의 roslaunch 명령어를 자주 사용해야한다면 아래와 같이 파일을 저장합니다.  

```cpp
/**
* file name : run_rviz.sh
* #!/bin/bash는 스크립트 파일을 bash로 실행할 것을 명시해주는 역할
*/
#!/bin/bash
roslaunch mybot_description mybot_rviz.launch
```

### Auto source environmental variable

catkin_make 실행하는 등 환경에 변화가 생기면 자주 아래의 명령어를 터미널에 입력해야 합니다.  

```cpp
source ~/catkin_ws/devel/setup.bash
```

위 명령어는 명시적으로 환경 변수와 path를 추가하는 것인데 위 명령어를 bashrc 파일에 추가하면 새로운 터미널을 실행할 때마다
자동으로 추가해주어서 편리합니다. 아래의 명령어로 bashrc 파일을 열고 제일 아래에 붙혀넣으면 됩니다.

```cpp
gedit ~/.bashrc
```

### Static Topic Publish Test

서버와 연동없이 드라이버 자체적으로 테스트를 할 때는 아래의 명령어를 사용합니다. 

1. roslaunch mybot_gazebo mybot_world.launch
2. rostopic pub /cmd_vel geometry_msgs/Twist "linear:
  x: 0.0  
  y: 0.0  
  z: 0.0  
angular:  
  x: 0.0  
  y: 0.0  
  z: 0.0"  


