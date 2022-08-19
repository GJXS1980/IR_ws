#!/usr/bin/env python
# -*- coding:utf8 -*-

import sys
from PyQt5.QtWidgets import QApplication, QWidget
from PyQt5 import QtCore
from PyQt5.QtCore import *
from threading import *
import time
import rospy
from geometry_msgs.msg import Pose, Point, Quaternion, Twist



# 声明窗口
class Window(QWidget):
    # 初始化
    def __init__(self):
        super(QWidget, self).__init__()
        rospy.init_node('send_goals_node', anonymous=False)
        self.cmd_vel = rospy.Publisher('/cmd_vel', Twist, queue_size=5)
        self.agv_move_msg = Twist()
        self.vel_fast = 1
        self.initUI()

        self.key_control = []
        # 显示定时器
        self.timer = QTimer(self)
        self.timer.timeout.connect(self.resutl)
        self.timer.start()

    def resutl(self):
        print(self.key_control)
        if len(self.key_control) == 1:
            if 'w' in self.key_control:
                self.agv_move_msg.linear.x = 0.1 * self.vel_fast
                self.agv_move_msg.angular.z = 0.0 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)
                print("front")
            elif 's' in self.key_control:
                self.agv_move_msg.linear.x = -0.1 * self.vel_fast
                self.agv_move_msg.angular.z = 0.0 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)
                print("back")
            else:
                self.agv_move_msg.linear.x = 0.0 * self.vel_fast
                self.agv_move_msg.angular.z = 0.0 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)

        
        elif len(self.key_control) == 2:
            if 'a' in self.key_control and 'w' in self.key_control:
                print("turn left")
                self.agv_move_msg.linear.x = 0.1 * self.vel_fast
                self.agv_move_msg.angular.z = 0.1 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)
            if 'd' in self.key_control and 'w' in self.key_control:
                print("turn right")
                self.agv_move_msg.linear.x = 0.1 * self.vel_fast
                self.agv_move_msg.angular.z = -0.1 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)
            if 'a' in self.key_control and 's' in self.key_control:
                print("left back")
                self.agv_move_msg.linear.x = -0.1 * self.vel_fast
                self.agv_move_msg.angular.z = -0.1 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)
            if 'd' in self.key_control and 's' in self.key_control:
                print("right back")
                self.agv_move_msg.linear.x = -0.1 * self.vel_fast
                self.agv_move_msg.angular.z = 0.1 * self.vel_fast
                self.cmd_vel.publish(self.agv_move_msg)
        else:
            print("stop")
            self.agv_move_msg.linear.x = 0.0
            self.agv_move_msg.angular.z = 0.0
            self.cmd_vel.publish(self.agv_move_msg)

    # 设置窗口的参数
    def initUI(self):
        self.setGeometry(300, 300, 300, 200)
        self.setFixedWidth(300)
        self.setFixedHeight(200)
        self.setWindowTitle('按键检测')
        # self.show()

    # 重写键盘事件
    def keyPressEvent(self, event):
        if (event.key() == (Qt.Key_A or Qt.Key_a)):

            # print('测试：A')
            if 'a' in self.key_control:
                pass
            elif len(self.key_control)<2:
                self.key_control.append('a')
            else:
                pass

        if (event.key() == (Qt.Key_U or Qt.Key_u)):
            self.vel_fast += 1
            self.vel_fast = min(self.vel_fast, 5)
        
        if (event.key() == (Qt.Key_J or Qt.Key_j)):
            self.vel_fast -= 1
            self.vel_fast = max(self.vel_fast, 1)

        if (event.key() == (Qt.Key_W or Qt.Key_w)):
            # print('测试：w')
            if 'w' in self.key_control:
                pass
            elif len(self.key_control)<2:
                self.key_control.append('w')
            else:
                pass
        
        if (event.key() == (Qt.Key_S or Qt.Key_s)):
            # print('测试：w')
            if 's' in self.key_control:
                pass
            elif len(self.key_control)<2:
                self.key_control.append('s')
            else:
                pass
        
        if (event.key() == (Qt.Key_D or Qt.Key_d)):
            # print('测试：w')
            if 'd' in self.key_control:
                pass
            elif len(self.key_control)<2:
                self.key_control.append('d')
            else:
                pass

    def keyReleaseEvent(self, event):
        if event.isAutoRepeat():
            pass
        elif (event.key() == (Qt.Key_A or Qt.Key_a)):
            # print("A释放")
            if 'a' in self.key_control:
                self.key_control.remove('a')
        elif (event.key() == (Qt.Key_W or Qt.Key_w)):
            # print("A释放")
            if 'w' in self.key_control:
                self.key_control.remove('w')
        elif (event.key() == (Qt.Key_S or Qt.Key_s)):
            # print("A释放")
            if 's' in self.key_control:
                self.key_control.remove('s')
        elif (event.key() == (Qt.Key_D or Qt.Key_d)):
            # print("A释放")
            if 'd' in self.key_control:
                self.key_control.remove('d')
        else:
            pass


if __name__ == '__main__':
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())

