# license dhsimpson
# Roomba 
from __future__ import print_function
import pycreate2
import time
import threading

r_vel = 500
l_vel = 500
r_curr = 0
l_curr = 0
rad = 0
def go(dummy):
    while True:
        bot.drive_direct(r_vel,l_vel)
        time.sleep(2)
  
def get_Motor_Speed(dummy): # opcode motor_current has motor speed -> Yes!!
    global r_curr, l_curr, r_vel, l_vel
    while True:
        sensor = bot.get_sensors()
        l_curr = sensor[44]
        r_curr = sensor[45]
        print("left : ",l_curr,"right : ",r_curr)
        if l_curr > r_curr:
            diff = l_curr-r_curr
            print("left is ",diff,"faster")
        elif l_curr > r_curr:
            diff = r_curr-l_curr
            print("Right",diff,"faster")
        else:
            print("Motor speed equals")
	print("Current_Radius: ",sensor[30])
        time.sleep(0.5)


def main():
    
    # init roomba 
    global bot, path
    port = '/dev/ttyUSB0'  
    baud = {
		'default': 115200,
		'alt': 19200  # shouldn't need this unless you accidentally set it to this
	}

    bot = pycreate2.Create2(port=port, baud=baud['default'])

    bot.start() 
    bot.safe()
    
    t_go = threading.Thread(target=go,args=(3,))
    t_get_Motor_Speed = threading.Thread(target=get_Motor_Speed,args=(3,))
    
    t_go.start()
    t_get_Motor_Speed.setDaemon(True)
    t_get_Motor_Speed.start()
    t_get_Motor_Speed.join()
    
    
    
    
if __name__ == "__main__":
    main()
    
    
	