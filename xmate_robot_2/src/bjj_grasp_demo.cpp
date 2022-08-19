#include "ros/ros.h"
#include "hg_ai_robot.h"
#include "hg_ai_robot.cpp"
//�궨��max min����
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))

//AGV��ǰĿ���
int AGV_Current_Goal = 0;
//װ��̨״̬
int ZPT_Current_State = 0;
//��е��״̬
int Robot_Current_State = 0;

//AGV�ص�
void AGV_CallBack(const std_msgs::Int32::ConstPtr& msg)
{
    AGV_Current_Goal = msg->data;
}

//װ��̨�ص�
void ZPT_CallBack(const std_msgs::Int32::ConstPtr& msg)
{
    ZPT_Current_State = msg->data;
}

//��е�ۻص�
void Robot_CallBack(const std_msgs::Int32::ConstPtr& msg)
{
    Robot_Current_State = msg->data;
}

int main(int argc, char *argv[])
{   //  ��ʼ��ROS�ڵ�
    ros::init(argc, argv, "ros_core");
    //  ����ROS���
    ros::NodeHandle n; 
    ros::NodeHandle node("~");

    //  ��ʼ����������
    double move_sped = 0.2;
    double move_dev_LR;
    double move_dev_FB;
    bool start_moment = false;
    int no_data_time = 0;
    bool move_left = false;
    bool move_right = false;
    bool move_forward = false;
    bool move_backward = false;

    //  ��е���ƶ��ٶ�
    node.param("move_sped", move_sped, 0.2);
    //  �������ؼ���
    node.param("monitor_state", start_moment, false);
    //  ƫ�ƾ��� ����
    node.param("move_deviation_LR", move_dev_LR, 0.05);
    //  ƫ�ƾ��� ǰ��
    node.param("move_deviation_FB", move_dev_FB, 0.05);
    //  δ���յ����ݴ���

    //  ����ƫ�ƾ�������
    if(move_dev_LR>0.1)
    {
        move_dev_LR = 0.1;
    }
    if(move_dev_LR<0.01)
    {
        move_dev_LR = 0.01;
    }

    //  ǰ��ƫ�ƾ�������
    if(move_dev_FB>0.07)
    {
        move_dev_FB = 0.07;
    }
    if(move_dev_FB<0.01)
    {
        move_dev_FB = 0.01;
    }
        
    //  ��ʼ����е�۲���
    HG_AI_Robot Robot_Interface;
    Robot_Interface.Jog0_Robot_Moment = 3;  // �ؽ�0Ĭ���������
    Robot_Interface.Jog4_Robot_Moment = 25; //�ؽ���Ĭ���������
    Robot_Interface.Start_Moment_Thread = start_moment; //Ĭ�Ͽ����ؽ����ؼ����߳�
    Robot_Interface.Move_Deviation = move_dev_LR;
    Robot_Interface.Move_Deviation1 = move_dev_FB;

    //���ӻ�е��: ip��ַ���˿ں�
    xmate::Robot robot(Robot_Interface.ipaddr, Robot_Interface.port);
    sleep(1.0);
    //��ʼ���������ϵ磬��ʼ��λ�ˣ��ؽڽǣ���צ��
    Robot_Interface.Init(robot);
    //��ȡ�ϵ�״̬
    int power_state = Robot_Interface.Robot_Get_Power(robot);
    //����ar��
    ros::Subscriber marker_sub = n.subscribe("/aruco_single/pose", 1, &HG_AI_Robot::Get_UR_Pose, &Robot_Interface);
    ros::Subscriber marker_sub2 = n.subscribe("/aruco_single/pose2", 1, &HG_AI_Robot::Get_UR_Pose2, &Robot_Interface);
    
    ros::Subscriber marker_sub3 = n.subscribe("/aruco_single1/pose", 1, &HG_AI_Robot::Get_UR_Pose, &Robot_Interface);
    ros::Subscriber marker_sub4 = n.subscribe("/aruco_single1/pose2", 1, &HG_AI_Robot::Get_UR_Pose2, &Robot_Interface);
        
    //�ж��ϵ�״̬��δ�ϵ����ϵ�Start_Moment_Thread
    if(!power_state)
    {
        Robot_Interface.Robot_Set_Power(1,robot);
    }

    //��ʼ������
 	float sx ,sy ,sz ,ww ,wx ,wy, wz;
    //  ��е��ƽ���˶�
	float sx1 ,sy1 ,sz1 ,ww1 ,wx1 ,wy1, wz1;
    //  ��е��ƽ���˶�У׼
    float sx2 ,sy2 ,sz2 ,ww2 ,wx2 ,wy2, wz2;
     //  ��е��ƽ���˶�����
    float sx3 ,sy3 ,sz3 ,ww3 ,wx3 ,wy3, wz3;

    //����˳���Ӧ��AR��ID
    int BJJ_ID = 4;

    //�ӽ������ץȡ����
    double bjj_place_x,bjj_place_y,bjj_place_z;
    double bjj_grasp_x,bjj_grasp_y,bjj_grasp_z;

    double robot_angle = 0.0;   // У׼�Ƕ�

    //��������ź�
    std_msgs::Int32 tag;

    //����PI�͹̶�λ��
    const double PI = 3.14159;
    std::array<double, 7> q_init;

    // // ��е��ץȡʶ��λ��
    // std::array<double, 7> grasp_identify_pose = {{(-95.365406 * PI / 180), (-14.531547* PI / 180), (5.96469726 * PI / 180), (-69.987455 * PI / 180), (-2.7401447 * PI / 180), (-94.285182 * PI / 180), (-4.0333042 * PI / 180)}};

    // // ��е�۷��õ�������������̨��תλ��
    // std::array<double, 7> grasp_fixed_middle_pose = {{(-30.278141 * PI / 180), (-14.834426* PI / 180), (6.0488525 * PI / 180), (-74.248970 * PI / 180), (-2.8477249 * PI / 180), (-90.563409 * PI / 180), (-2.7336559 * PI / 180)}};

    // // ���Ϸ���λ��
    // std::array<double,7> grasp_id1_pubsh = {{(-18.750915 * PI / 180), (-5.1412582 * PI / 180), (3.69308166 * PI / 180), (-101.94888 * PI / 180), (-1.8495998 * PI / 180), (-72.657669 * PI / 180), (-15.904083 * PI / 180)}};
    // std::array<double,7> pubsh_pose = {{(-18.750915 * PI / 180), (-5.1412582 * PI / 180), (3.69308166 * PI / 180), (-101.94888 * PI / 180), (-1.8495998 * PI / 180), (-72.657669 * PI / 180), (-15.904083 * PI / 180)}};
    
    // //��е��ʶ��λ��
    // std::array<double, 7> place_identify_pose = {{(-95.365406 * PI / 180), (-14.531547* PI / 180), (5.96469726 * PI / 180), (-69.987455 * PI / 180), (-2.7401447 * PI / 180), (-94.285182 * PI / 180), (-4.0333042 * PI / 180)}};
    // // ��е�۷��õ���������װ��̨��תλ��
    // std::array<double, 7> place_fixed_middle_pose = {{(-30.278141 * PI / 180), (-14.834426* PI / 180), (6.0488525 * PI / 180), (-74.248970 * PI / 180), (-2.8477249 * PI / 180), (-90.563409 * PI / 180), (-2.7336559 * PI / 180)}};

    // ������������ץȡ�ӽ������
    node.param("bjj_grasp_x", bjj_grasp_x, 0.015);  //  ��������ץȡid1����ʱ��е�۵�x����ƫ����(�ѿ�������ϵ)����������
    node.param("bjj_grasp_y", bjj_grasp_y, 0.004);  //����������ץȡid1����ʱ��е�۵�y����ƫ����(�ѿ�������ϵ)��������ǰ
    node.param("bjj_grasp_z", bjj_grasp_z, 0.040134663);  //����������ץȡid1����ʱ��е�۵�z����ƫ����(�ѿ�������ϵ)�����¼�С

    // ���÷����ӽ������
    node.param("bjj_place_x", bjj_place_x, 0.007);  // �������ϵ�װ��̨ʱ��е�۵�x����ƫ����(�ѿ�������ϵ)����������(Ĭ�ϣ�0.007)
    node.param("bjj_place_y", bjj_place_y, 0.053);  //���������ϵ�װ��̨ʱ��е�۵�y����ƫ����(�ѿ�������ϵ)��������ǰ(Ĭ�ϣ�0.053)
    node.param("bjj_place_z", bjj_place_z, 0.253);    //���������ϵ�װ��̨ʱ��е�۵�z����ƫ����(�ѿ�������ϵ)����������(Ĭ�ϣ�0.253)

    //  ��е�۷���������ɱ�־λ
    ros::Publisher place_pub = n.advertise<std_msgs::Int32>("PlaceDone", 1000);
    //  ��Ʒץȡ
    ros::Publisher assemble_pub = n.advertise<std_msgs::Int32>("AssembleDone", 1000);

    //�ٶ�����
    move_sped = min(move_sped, 0.3);

    ros::Rate loop_rate(10);

	ros::Subscriber agv_flag = n.subscribe("agv_state", 1, AGV_CallBack);
    Robot_Interface.Stop_Moment_Thread = true;

/*--------------------id1,��ĸ--------------------*/
    try
    {
        Robot_Interface.Robot_Set_Speed(move_sped,robot);
        //  ץȡ�м�λ��
        Robot_Interface.Robot_MoveJ(Robot_Interface.place_fixed_middle_pose,robot);
        //  ץȡʶ��λ��
        Robot_Interface.Robot_MoveJ(Robot_Interface.place_identify_pose,robot);

/*-----------------------------id3, �ӽ��-----------------------------*/
        while(AGV_Current_Goal != 3)
        {
            ros::spinOnce();
            loop_rate.sleep();
        }
        
        //  ���֮ǰʶ�𵽵���������
        Robot_Interface.clean_ar_data();
        Robot_Interface.clean_ur_data();
        sleep(0.5);
        while(Robot_Interface.Ur_Pose[0][0]==0.0)
        {
            no_data_time +=1;
            if(no_data_time>5 && !move_forward)
            {
                Robot_Interface.Robot_MoveL(0.0,Robot_Interface.Move_Deviation1,0.0,robot);
                move_forward = true;
                no_data_time = 0;
            }
            else if(no_data_time > 5 && !move_backward)
            {
                Robot_Interface.Robot_MoveJ(Robot_Interface.place_identify_pose,robot);
                Robot_Interface.Robot_MoveL(0.0,-Robot_Interface.Move_Deviation1,0.0,robot);
                move_backward = true;
                no_data_time = 0;
            }
            else if(no_data_time>5 && !move_left)
            {
                Robot_Interface.Robot_MoveJ(Robot_Interface.place_identify_pose,robot);
                Robot_Interface.Robot_MoveL(Robot_Interface.Move_Deviation,0.0,0.0,robot);
                move_left = true;
                no_data_time = 0;
            }
            else if(no_data_time > 5 && !move_right)
            {
                Robot_Interface.Robot_MoveJ(Robot_Interface.place_identify_pose,robot);
                Robot_Interface.Robot_MoveL(-Robot_Interface.Move_Deviation,0.0,0.0,robot);
                move_right = true;
                no_data_time = 0;
            }

            ros::spinOnce();
            loop_rate.sleep();
        }

       if (move_forward && !move_backward)
        {
            std::cout<<"move forward"<<std::endl;
            move_forward = move_backward = false;
        }
        else if(move_forward && move_backward)
        {
            std::cout<<"move backword"<<std::endl;
            move_forward = move_backward = false;
        }

       if (move_left && !move_right)
        {
            std::cout<<"move left"<<std::endl;
            move_left = move_right = false;
        }
        else if(move_left && move_right)
        {
            std::cout<<"move right"<<std::endl;
            move_left = move_right = false;
        }

        sx = Robot_Interface.Ur_Pose[0][0];
        sy = Robot_Interface.Ur_Pose[0][1];
        sz = Robot_Interface.Ur_Pose[0][2];
        ww = Robot_Interface.Ur_Pose[0][3];
        wx = Robot_Interface.Ur_Pose[0][4];
        wy = Robot_Interface.Ur_Pose[0][5];
        wz = Robot_Interface.Ur_Pose[0][6];
        std::cout << "����ʶ��ar���꣺" <<std::endl;
        std::cout << "datax:" <<sx<<std::endl;
        std::cout << "datay:" <<sy<<std::endl;
        std::cout << "dataz:" <<sz<<std::endl;

        //������Ԫ��
        Eigen::Quaterniond quaternion(ww,wx,wy,wz);
        //��Ԫ��תŷ����
        Eigen::Vector3d eulerAngle=quaternion.matrix().eulerAngles(2,1,0);
        //����ת�Ƕ�
        robot_angle = (eulerAngle(0) * 180) / PI;
        if(robot_angle > 90.0)
        {
	            robot_angle = 180.0 - robot_angle;	
	    }
        if(robot_angle > 90)
        {
            //  ��е��ĩ����ת
            Robot_Interface.Robot_MoveR(-PI/2 + eulerAngle(0), robot);
        }
        else
        {
            //  ��е��ĩ����ת
            Robot_Interface.Robot_MoveR(eulerAngle(0) - PI/2, robot);
        }

        std::cout<<"AR��Ƕȣ� "<< robot_angle <<std::endl;
        sleep(1.0);
	    Robot_Interface.clean_ur_data();

        /*-----����У׼-----*/
        //�ȴ���ά������
	    while(Robot_Interface.Ur_Pose[0][0]==0.0)
        {
            ros::spinOnce();
            loop_rate.sleep();
        }
	
        sx = Robot_Interface.Ur_Pose[0][0];
        sy = Robot_Interface.Ur_Pose[0][1];
        sz = Robot_Interface.Ur_Pose[0][2];
        ww = Robot_Interface.Ur_Pose[0][3];
        wx = Robot_Interface.Ur_Pose[0][4];
        wy = Robot_Interface.Ur_Pose[0][5];
        wz = Robot_Interface.Ur_Pose[0][6];
        //Robot_Interface.clean_ur_data();
        std::cout << "ת����ar���꣺" <<std::endl;
        std::cout << "datax:" <<sx<<std::endl;
        std::cout << "datay:" <<sy<<std::endl;
        std::cout << "dataz:" <<sz<<std::endl;

        Robot_Interface.Robot_MoveL(sx-0.05,-sy+0.02,0.0,robot);
        sleep(1.0);
        Robot_Interface.clean_ur_data();

        /*-----����У׼-----*/
        //�ȴ���ά������
	    while(Robot_Interface.Ur_Pose[0][0]==0.0)
        {
            ros::spinOnce();
            loop_rate.sleep();
        }
	
        sx = Robot_Interface.Ur_Pose[0][0];
        sy = Robot_Interface.Ur_Pose[0][1];
        sz = Robot_Interface.Ur_Pose[0][2];
        ww = Robot_Interface.Ur_Pose[0][3];
        wx = Robot_Interface.Ur_Pose[0][4];
        wy = Robot_Interface.Ur_Pose[0][5];
        wz = Robot_Interface.Ur_Pose[0][6];
        //Robot_Interface.clean_ur_data();
        std::cout << "����ƽ��У׼��ar����" <<std::endl;
        std::cout << "datax:" <<sx<<std::endl;
        std::cout << "datay:" <<sy<<std::endl;
        std::cout << "dataz:" <<sz<<std::endl;
        
        //  ��е��ʶ�𾫶�У׼
        if(sy > 0.0616)
        {
            sy = (sy - 0.0616);
            std::cout << sy <<std::endl;
        }
        else
        {
            sy = -(0.0616 - sy);
        }
        if(sx > 0.0035)
        {
            sx = (sx - 0.0035);
        }
        else
        {
            sx = -(0.0035 - sx);
        }
        Robot_Interface.Robot_MoveL(sx,-sy,0.0,robot);
        std::cout << "���ȼ��������ƽ����" <<std::endl;
        std::cout << "datax:" <<sx<<std::endl;
        std::cout << "datay:" <<sy<<std::endl;
        std::cout << "dataz:" <<sz<<std::endl;
        sleep(1.0);
        Robot_Interface.clean_ur_data();
        
        /*-----��ȥץ-----*/
        while(Robot_Interface.Ur_Pose[0][0]==0.0)
        {
            ros::spinOnce();
            loop_rate.sleep();
        }
        sx = Robot_Interface.Ur_Pose[0][0];
        sy = Robot_Interface.Ur_Pose[0][1];
        sz = Robot_Interface.Ur_Pose[0][2];
        ww = Robot_Interface.Ur_Pose[0][3];
        wx = Robot_Interface.Ur_Pose[0][4];
        wy = Robot_Interface.Ur_Pose[0][5];
        wz = Robot_Interface.Ur_Pose[0][6];
        //Robot_Interface.clean_ur_data();
        std::cout << "ץȡǰar����" <<std::endl;
        std::cout << "datax:" <<sx<<std::endl;
        std::cout << "datay:" <<sy<<std::endl;
        std::cout << "dataz:" <<sz<<std::endl;
        
        Robot_Interface.Robot_MoveL(sx-bjj_grasp_x,-sy+bjj_grasp_y,0.0,robot);
        Robot_Interface.Robot_MoveL(0.0,0.0,-sz+bjj_grasp_z,robot);
        sleep(1.0);
        Robot_Interface.clean_ur_data();

        bool close_state = Robot_Interface.Robot_Grasp_Control(0,robot);
        if(close_state)
        {
            //����
            Robot_Interface.Robot_MoveL(0.0,0.0,bjj_place_z,robot);
	        Robot_Interface.Robot_MoveJ(Robot_Interface.grasp_identify_pose,robot);
            //�м�λ��
            Robot_Interface.Robot_MoveJ(Robot_Interface.grasp_fixed_middle_pose,robot);
            //��ʼλ��
            Robot_Interface.Robot_MoveJ(Robot_Interface.pubsh_pose,robot);
            //���ã���λ��
            Robot_Interface.Robot_MoveL(bjj_place_x,bjj_place_y,0.0,robot);
            //����
            Robot_Interface.Robot_MoveL(0.0,0.0,-bjj_place_z,robot);
        }
	    bool open_state = Robot_Interface.Robot_Grasp_Control(1,robot);
	    //����
        Robot_Interface.Robot_MoveL(0.0,0.0,bjj_place_z,robot);
        if(open_state)
        {
            //��ʼλ��
            Robot_Interface.Robot_MoveJ(Robot_Interface.pubsh_pose,robot);
        }

    }
    catch (xmate::ControlException &e) 
    {
        std::cout << e.what() << std::endl;
    }
    /*---------ץȡ2�Ž���----------*/
    /*---------�����ķָ���---------*/
    
    //����������s�����ź�
    tag.data = 1;
    place_pub.publish(tag);
    tag.data = 0;
    ros::spin();
    return 0;
}
