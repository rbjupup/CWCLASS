#define SUPPORTCLASSNUM 1

#define CLASSNAME1 CString("测试类")
#define CWFUN1 CString("随手测试")
#define HELPCWFUN1 CString("测试类不具体包含某个功能,一般用于迅速用函数实现某个功能,实现后即可重写")


#define CLASSNAME2 CString("TCP通讯")
#define SOCKETFUN1 CString("INITCLIENT")
#define SOCKETFUN2 CString("INITSERVER")
#define SOCKETFUN3 CString("TESTSendSingal")
#define HELPSOCKETFUN3 CString("测试发送一个消息\r\n输入:  1:消息类型\r\n  0:吸盘吸取结果\r\n  1:客户端y轴到位\r\n \
  2：服务器发送吸嘴到位信号\r\n  3:发送上料完成信号 \r\n 4: 发送上料x轴到位\r\n 5:告诉轴下料吸嘴已经到位\r\n \
  6-9: 控制客户端运动停止\r\n 10：发送y轴偏移\r\n  \
  11:SendLoadSuckInPos\r\n  12:上料的时候发送吸盘是否吸紧\r\n  13: 告诉y轴下料线程完成\r\n 14: 客户端上线\r\n \
  15:客户端下线\r\n   16:机器已经准备好\r\n  17:蜂鸣器响\r\n 2:工位其中一个")
#define SOCKETFUN4 CString("TESTReadAllSingal")



#define CLASSNAME3 CString("串口通讯")
#define COMFUN1 CString("初始化")
#define COMFUN2 CString("读取绝对编码器的位置")



#define CLASSNAME4 CString("BCG界面设计")
#define BCGFUN1 CString("打开一个BCG对话框")
#define HELPBCGFUN1 CString("打开一个BCG对话框，显示bcg的按钮")
#define BCGFUN2 CString("波形图绘制")
#define HELPBCGFUN2 CString("先添加一个picctrl控件，然后按照软件里面那样初始化")



#define CLASSNAME5 CString("相机测试")
#define CAMFUN1 CString("打开相机")
#define HELPCAMFUN1 CString("加载一个相机并显示出来，现在支持的相机只有KSG，IMG,PTG")
#define CAMFUN2 CString("采集图片")
#define HELPCAMFUN2 CString("采集的图片将放在E;\\pic.bmp")


#define CLASSNAME6 CString("机器人")
#define ROBOFUN1 CString("翻译文字")
#define HELPROBOFUN1 CString("输入1：要转化的文字\r\n输入2：保存wav的路径\r\n输入3: 声音类型(0是小燕，1是小风)")
#define ROBOFUN2 CString("朗读文字")
#define HELPROBOFUN2 CString("输入1：要转化的文字\r\n输入2: 声音类型(0是小燕，1是小风，不输入)")
#define ROBOFUN3 CString("开始听写文字")
#define HELPROBOFUN3 CString("")
#define ROBOFUN4 CString("皮卡freestyle")
#define HELPROBOFUN4 CString("")
#define ROBOFUN5 CString("对话图灵")
#define HELPROBOFUN5 CString("说退出可以退出当前对话")



#define CLASSNAME7 CString("MFC控件")
#define MFCBTNFUN1 CString("透明按钮")
#define HELPMFCBTNFUN1 CString("操作步骤：\r\n1:添加GDI+\r\n2:复制黏贴")
#define MFCBTNFUN2 CString("Tab操作,多种类")
#define HELPMFCBTNFUN2 CString("当我们有两种对话框组,想通过一个变量切换tab下面的组,那么可以用这种方式,可拓展性很好\r\n输入1:加载的窗口类型")
#define MFCBTNFUN3 CString("图像控件的图像放大缩小平移")
#define HELPMFCBTNFUN3 CString("测试拓展CStatic类,使它能对图像进行缩放和拖动");


#define CLASSNAME8 CString("C++爬虫")
#define MONFUN1 CString("简易网页监听")
#define HELPMONBTNFUN1 CString("编写中")
#define MONFUN2 CString("测试MFCBrower")
#define HELPMONFUN2 CString("用来测试MFCBrower点击网页并且获得html信息")
#define MONFUN3 CString("数据监控")
#define HELPMONFUN3 CString("用来监控数据是否大于临界值\r\n输入1：网址\r\n输入2：数据数目（NUMDATA）\r\n输入3---2+NUMDATA：\
数据在html代码中前面部分\r\n输入3+NUMDATA---2+2*NUMDATA:数据在html代码中前面部分\r\n数据3+2*NUMDATA:需要监控的数量（WATCHNUM）\r\n\
数据4+2*NUMDATA---3+2*NUMDATA+WATCHNUM:需要监控的编号\r\n数据4+2*NUMDATA+WATCHNUM---3+2*NUMDATA+2*WATCHNUM:需要监控的数值\r\n\
数据4+2*NUMDATA+2*WATCHNUM:发送数据到邮箱\r\n数据5+2*NUMDATA+2*WATCHNUM:最小数据数量\r\n数据6+2*NUMDATA+2*WATCHNUM:最长等待时间\r\n")
#define MONFUN4 CString("连续监控")
#define HELPMONFUN4 CString("用来监控一系列网址，通过数据监控保存的ini文件进行设置\r\n输入1：监控网址数（WEBNUM）\r\n\
输入2-1+WEBNUM:ini文件路径\r\n输入2+WEBNUM：刷新间隔时间")



#define CLASSNAME9 CString("机器视觉")
#define CVFUN1 CString("切割图像")
#define HELPCVFUN1 CString("输入1:输入图像路径\r\n输入2:输出图像文件夹路径\r\n输入3:X方向分割次数\r\n输入4:Y方向分割次数\r\n")
#define CVFUN2 CString("图像运算")
#define HELPCVFUN2 CString("输入1:输入图像路径1\r\n输入2:输入图像路径2\r\n输入3:输出图像路径\r\n输入4:X方向偏移\r\n输入5:X方向偏移\r\n输入6:计算类型，0是加1是减\r\n")
#define CVFUN3 CString("点云生成灰度图")
#define HELPCVFUN3 CString("输入1:输入图文件路径1\r\n输入2:输出图像路径2\r\n输入3:点云文件类型，0是csv\r\n")
#define CVFUN4 CString("灰度图阈值分割")
#define HELPCVFUN4 CString("输入1:输入图像路径1\r\n输入2:输出图像路径2\r\n输入3:阈值\r\n")
#define CVFUN5 CString("图像轮廓寻找")
#define HELPCVFUN5 CString("说明：找轮廓并会知道原图上\r\n输入1:输入图像路径1\r\n输入2:输出图像路径2\r\n输入3:方法\r\n")
#define CVFUN6 CString("漫水填充")
#define HELPCVFUN6 CString("说明：使用种子点填充相似像素\r\n输入1:输入图像路径1\r\n")
#define CVFUN7 CString("ROI分割")
#define HELPCVFUN7 CString("说明：使用ROI将图片的感兴趣区域提取出来\r\n输入1:输入图像路径\r\n输入2:输出图像路径\r\n输入3:x\r\n输入4:y\r\n输入5:宽\r\n输入6:高\r\n")
#define CVFUN8 CString("动态阈值")
#define HELPCVFUN8 CString("说明：自制往某个方向的动态阈值,\r\n输入1:输入图像路径\r\n输入2:输出图像路径\r\n输入3:迭代宽度\r\n输入4:阈值宽度\r\n输入5:动态方向\r\n输入6:初始阈值\r\n")
#define CVFUN9 CString("获取轮廓外包框")
#define HELPCVFUN9 CString("说明：通过调节最小轮廓,将轮廓分别提取出来\r\n输入1:输入图像路径\r\n输入2:输出图像路径\r\n输入3:最小轮廓面积\r\n")
#define CVFUN10 CString("深视智能")
#define HELPCVFUN10 CString("说明：请先按照点云转灰度图-ROI分割-动态阈值-获取轮廓外包框-深视智能的顺序处理,会得到相应的结果\r\n")
#define CVFUN11 CString("平面拟合")
#define HELPCVFUN11 CString("说明：拟合出一个三维平面,会得到相应的结果\r\n")
#define CVFUN12 CString("平面拟合2")
#define HELPCVFUN12 CString("说明：拟合出一个三维平面,会得到相应的结果\r\n输入1:灰度图\r\n输入2:背景mask图\r\n输入3:输出的距离图")
#define CVFUN13 CString("抖音视觉")
#define HELPCVFUN13 CString("说明：输出抖音那样的脸变形功能\r\n输入无")
#define CVFUN14 CString("直方图均衡化")
#define HELPCVFUN14 CString("输入1:原始图像路径\r\n输入2:保存路径")
#define CVFUN15 CString("显示灰度直方图")
#define HELPCVFUN15 CString("输入1:原始图像路径\r\n输入2:保存路径")