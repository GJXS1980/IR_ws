#!/usr/bin/env python
# -*- coding: utf-8 -*-

from email.mime import image
import cv2
import numpy as np

import rospy

from sensor_msgs.msg import Image, RegionOfInterest
from cv_bridge import CvBridge, CvBridgeError

class Findposition:
    def __init__(self):
        # rospy.on_shutdown(self.cleanup)

        rospy.init_node("deshade_node", anonymous=True)
        self.bridge = CvBridge()
        rospy.Subscriber("/usb_cam/image_raw", Image, self.image_callback, queue_size=1)

        self.image_pub = rospy.Publisher("cv_bridge_image", Image, queue_size=1)
        rate = rospy.Rate(10) # 发布频率为10hz
        rospy.spin()
        while not rospy.is_shutdown():
            # self.move_base.wait_for_result()
            rospy.sleep(1)

    def image_callback(self, data):
        # 使用cv_bridge将ROS的图像数据转换成OpenCV的图像格式
        try:
            self.cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")     
            frame = np.array(self.cv_image, dtype=np.uint8)
        except CvBridgeError:
            pass

        # 将图像从RGB转成灰度图
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)  
        pixel = int(np.mean(gray [gray  > 90]))
        gray [gray  > 50] = pixel
        IMG_OUT = cv2.cvtColor(gray, cv2.COLOR_GRAY2RGB)
        self.image_pub.publish(self.bridge.cv2_to_imgmsg(IMG_OUT, "bgr8"))


if __name__== '__main__' :
    try:
        Findposition()
	#rospy.spin()
    except rospy.ROSInterruptException:
        rospy.loginfo("Deshade finished.")
