#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import rospy
import threading
import time
from service_example.srv import Socket_srv, Socket_srvResponse
from std_msgs.msg import Float32MultiArray, Int32, Float32,String

class ARV_PLC_Socket():
    def __init__(self):
        rospy.init_node("ARV_PLC_Socket", anonymous=False)

        host_ip = rospy.get_param("~host_ip",'192.168.4.100')
        host_post = rospy.get_param("~host_post", 10009)

        rospy.on_shutdown(self.shutdown)
        rate = rospy.get_param("~Rate", 10)
        self.rate = rospy.Rate(rate)

        self.Socket_Server = rospy.Service("/Socket_Server", Socket_srv, self.socket_handle)
        
        rospy.wait_for_service("ARV_Socket_server")                                #与ARV_frame节点通过服务通信机制通信
        self.ARV_Client = rospy.ServiceProxy("ARV_Socket_server", Socket_srv)
        rospy.loginfo('ARV_Socket_server连接成功')

        #   创建一个socket对象
        self.Socket_Client = socket.socket()                       #PLC作为服务器端，PC作为客户端

        #   连接127.0.0.1:8080的客户端
        try:
            self.Socket_Client.connect((host_ip,host_post))
            rospy.loginfo("PLC服务器连接成功")

            msg_recv_thr = threading.Thread(target=self.msg_recv)   #将函数msg_recv放入线程中
            msg_recv_thr.start()

        except Exception as e:
            print(e)

        while not rospy.is_shutdown():
            rospy.Rate.sleep(self.rate)
    
    def msg_send(self,cmd):
        cmd = cmd
        while True:
                self.Socket_Client.sendall(cmd) #发送命令
                self.msg = self.Socket_Client.recv(8)
                print(self.msg)
                #time.sleep(2.0)
                if self.msg and (self.msg[0:2] == 'NG' or self.msg[0:2] == 'OK'):
                    print("收到PLC的回复 ：%s" % self.msg)
                else:
                    continue
                rospy.logdebug(cmd)
                rospy.sleep(0.05)

                if cmd == 'BJJ_SLWC':
                    if self.msg[0:2] =='NG':
                        continue
                    elif self.msg[0:2] == 'OK':
                        return self.msg[0:2]
                if cmd == 'BJJ_XLWC':
                    if self.msg[0:2] =='NG':
                        continue
                    elif self.msg[0:2] == 'OK':
                        return self.msg[0:2]

    def msg_recv(self):
        self.msg = ''
        print('线程msg:%s' % self.msg)
        while True:
            self.msg = self.Socket_Client.recv(8)            #循环接收PLC的信息，注意没有消息时会阻塞
            if(self.msg):
                print('收到数据%s' % self.msg)
            if self.msg == 'BJJ_SLSQ':
                self.Socket_Client.sendall(bytes('OK'))
                rospy.logdebug("接收到PLC发出信息：%s , 钣金件上料申请" % self.msg)
                response  = self.ARV_Client(self.msg)     #等待消息返回时会阻塞
            
            if self.msg == 'BJJ_XLSQ':
                self.Socket_Client.sendall(bytes('OK'))
                rospy.logdebug("接收到PLC发出信息：%s , 钣金件下料申请" % self.msg)
                response  = self.ARV_Client(self.msg)     #等待消息返回时会阻塞

    def socket_handle(self,req):             #接收到ARV_Frame的信息后，将信息发送给PLC
        cmd = req.request
        print(cmd)
        response = self.msg_send(cmd)
        rospy.sleep(0.05)
        return response                  #将PLC的回应作为服务的回应发送

    def shutdown(self):
        pass
if __name__ == "__main__":
    ARV_PLC_Socket()
