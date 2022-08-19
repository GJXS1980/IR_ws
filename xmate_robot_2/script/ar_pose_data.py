#!/usr/bin/env python
# -*- coding: utf-8 -*-

from email.mime import image
import cv2
import numpy as np

import rospy

from sensor_msgs.msg import Image, RegionOfInterest
from cv_bridge import CvBridge, CvBridgeError
from geometry_msgs.msg import PoseStamped

class ArPose:
    def __init__(self):
        self.x, self.y, self.z = None, None, None
        # rospy.on_shutdown(self.cleanup)

        rospy.init_node("ar_pose_data", anonymous=True)
        rospy.Subscriber("/aruco_single1/pose", PoseStamped, self.ar_callback, queue_size=1)
        rospy.Subscriber("/aruco_single/pose", PoseStamped, self.ar_callback, queue_size=1)
        # self.image_pub = rospy.Publisher("cv_bridge_image", Image, queue_size=1)
        rate = rospy.Rate(10) # 发布频率为10hz
        rospy.spin()
        while not rospy.is_shutdown():
            # print(self.x, self.y, self.z)
            # self.move_base.wait_for_result()
            rospy.sleep(1)

    def ar_callback(self, data):
        self.x = data.pose.position.x
        self.y = data.pose.position.y
        self.z = data.pose.position.z
        print(self.x, self.y, self.z)

if __name__== '__main__' :
    try:
        ArPose()
	#rospy.spin()
    except rospy.ROSInterruptException:
        rospy.loginfo("ar pose data.")
