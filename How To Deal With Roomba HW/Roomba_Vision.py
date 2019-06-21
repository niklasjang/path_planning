# -*- coding: utf-8 -*-
"""
Created on Thu May 30 00:39:13 2019

@author: dhsimpson
"""
import cv2
import numpy as np

cnt_Straight = 0

isBlack = False
th_Black = 128
prev_Black = 0



#############################################################################################################################################
"""1. is roomba front"""
#############################################################################################################################################
#중앙으로 부터 x축 -30~+30 픽셀 / y축 0~+30 픽셀의 위치 안에서 초록색이 검출된다면 
#( 룸바에 초록색 스티커를 둘레 붙여놓는다. ) 룸바라고 판단한다.
def isRoomba(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    x = int(frame.shape[1]/2)
    for i in range(x-30, x+30):
        for j in range(0, 30):
            if 110 <= hsv[j,i,0] <= 130 and 100 <= hsv[j,i,1] <= 255 and 100 <= hsv[j,i,2] <= 255:
                return True
    return False
#############################################################################################################################################
"""2. roomba passed"""
#############################################################################################################################################
# 임의의 명령에 있어, Straight 를 해야 되는 경우였다면 Main Roomba Node 에서 
#straight 함수를 실행시켰을 것으로 가정 

def roomba_Straight(frame,gray,line_Count): 
    y = int(frame.shape[0]/2)
    x = int(frame.shape[1]/2)
    
    if frame[y-1,x-1] == 255: # 선은 엣지이므로 흰색 
        line_Count += 1
    
    if line_Count >= 2 and gray[y-1,x-1] == 0: #선을 넘어가서 다음 선을 발견했을 때 1로 초기화
        line_Count = 1
        return True
    else:
        return False
############################################################################################################################################# 
"""3. roomba tilted"""
#############################################################################################################################################
def tilt(frame):
    y = frame.shape[0]
    x = frame.shape[1]
    y1 = 0
    y2 = 0
    tilt = 0.0
    
    for j in range(y-1,0,-1):
            if frame[j,0] == 255:
                y1 = j
                break
    for j in range(y-1,0,-1):
            if frame[j,x-1] == 255:
                y2 = j
                break
    if x!= 0:
        tilt = (y2-y1)/(x-0)
    print("기울기 :",tilt)
    return tilt 