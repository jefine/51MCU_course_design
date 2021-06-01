package com.demo.mqtt_project;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.provider.Settings;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/*
//  第二节开始  目前Java0基础
第二节：
控件（常用的）：   控件（扩展）拖动条 进度条 浏览器框 地图 单选框 复选框
按钮 图片按钮
文本框
编辑框
图片框
选择开关
 wrap_content   自适应
 match_parent    充满父控件
 android:id="@+id/bt_1"   用来和JAVA文件通讯或者说是 绑定事件的
LinearLayout  线性布局
android:orientation="vertical"  设置布局方向  vertical垂直  horizontal水平
android:layout_margin="10dp"    边距

第三节开始   目前java 0基础
安卓开发  要多调试  多刷程序（因为你不知道你的APP程序会什么时候崩溃！！！！）
java里面的操作  大部分都类似于单片机的函数

控件的ID  是java文件与XML文件通讯的介质  类似于控件的 号码牌（唯一）
按钮：
单击事件有很多种实现方法  咱们讲最简单最常用的一种

按钮单机    用来发送命令控制硬件 例如：open door
文本框更新数据   用来接收硬件上报的传感器值  例如：温度 25.6


咱们 不凭空的学安卓开发  咱们是有目的的 直接面向项目和实战。
剩下的交给举一反三

第四节：MQTT Jar导入
复制粘贴！！！
Mqtt_init()
Mqtt_connect()
startReconnect()
爆红不用管！！！
最后赋值
Handler()
#################################
MQTT的知识  保证每个人都能连我的服务器   每个人的ID（MQTT要求唯一）都这设置为自己的QQ号
直接开搞  先跑起来 再讲原理
需要网络权限  和授权！（Android 6.0 以后需要动态授权后面再讲）
连接成功！
开始订阅 topic
拿到数据   想干嘛干嘛   随意做UI
下发控制
发布消息到指定 topic
拷贝发布函数
测试APP发消息  成功！！！
硬件  得到消息  判断 是不是“open_led ”然后 点灯！！
APP   得到消息  判断 传感器 数据  然后  UI展示   温度：56.3

剩下的  交给举一反三！！


 */

public class MainActivity extends AppCompatActivity {
    private String host = "tcp://106.13.150.28:1883";
    private String userName = "android";
    private String passWord = "android";
    private String mqtt_id = "565402462"; //不可重复
    private String mqtt_sub_topic = "565402462";
    private String mqtt_pub_topic = "565402462_PC";
    private ScheduledExecutorService scheduler;
    private Button btn_1;  //类似于单片机开发里面的   参数初始化
    private ImageView image_1;
    private TextView text_test;
    private MqttClient client;
    private MqttConnectOptions options;
    private Handler handler;

    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        //这里是界面打开后 最先运行的地方
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main); // 对应界面UI
        //一般先用来进行界面初始化 控件初始化  初始化一些参数和变量。。。。。
        //不恰当比方    类似于 单片机的   main函数
        btn_1 = findViewById(R.id.btn_1); // 寻找xml里面真正的id  与自己定义的id绑定
        btn_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 这里就是单机之后执行的地方
                // 玩单片机  最常用的就是调试  printf（“hello”）
                System.out.println("hello");
                //更直观的方法   用弹窗：toast
                //在当前activity 显示内容为“hello”的短时间弹窗
                Toast.makeText(MainActivity.this,"hello" ,Toast.LENGTH_SHORT).show();
            }
        });
        //到这里  你已经学会了基本的安卓开发
        // 按钮单机事件你会了   图片单机呢？？？
        image_1 =findViewById(R.id.image_1);
        image_1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this,"我是第一个图片" ,Toast.LENGTH_SHORT).show();
                publishmessageplus(mqtt_pub_topic,"open_led");

            }
        });
        //  两个控件联动    按钮单机 更改 textview 的内容
        text_test =findViewById(R.id.text_test);
//**********************************************************//
        Mqtt_init();
        startReconnect();

        handler = new Handler() {
            @SuppressLint("SetTextI18n")
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what){
                    case 1: //开机校验更新回传
                        break;
                    case 2:  // 反馈回传

                        break;
                    case 3:  //MQTT 收到消息回传   UTF8Buffer msg=new UTF8Buffer(object.toString());
                        Toast.makeText(MainActivity.this,msg.obj.toString() ,Toast.LENGTH_SHORT).show();
                        text_test.setText(msg.obj.toString());
                        break;
                    case 30:  //连接失败
                        Toast.makeText(MainActivity.this,"连接失败" ,Toast.LENGTH_SHORT).show();
                        break;
                    case 31:   //连接成功
                        Toast.makeText(MainActivity.this,"连接成功" ,Toast.LENGTH_SHORT).show();
                        try {
                            client.subscribe(mqtt_sub_topic,1);
                        } catch (MqttException e) {
                            e.printStackTrace();
                        }
                        break;
                    default:
                        break;
                }
            }
        };
    }

    private void Mqtt_init()
    {
        try {
            //host为主机名，test为clientid即连接MQTT的客户端ID，一般以客户端唯一标识符表示，MemoryPersistence设置clientid的保存形式，默认为以内存保存
            client = new MqttClient(host, mqtt_id,
                    new MemoryPersistence());
            //MQTT的连接设置
            options = new MqttConnectOptions();
            //设置是否清空session,这里如果设置为false表示服务器会保留客户端的连接记录，这里设置为true表示每次连接到服务器都以新的身份连接
            options.setCleanSession(false);
            //设置连接的用户名
            options.setUserName(userName);
            //设置连接的密码
            options.setPassword(passWord.toCharArray());
            // 设置超时时间 单位为秒
            options.setConnectionTimeout(10);
            // 设置会话心跳时间 单位为秒 服务器会每隔1.5*20秒的时间向客户端发送个消息判断客户端是否在线，但这个方法并没有重连的机制
            options.setKeepAliveInterval(20);
            //设置回调
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //连接丢失后，一般在这里面进行重连
                    System.out.println("connectionLost----------");
                    //startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish后会执行到这里
                    System.out.println("deliveryComplete---------"
                            + token.isComplete());
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message)
                        throws Exception {
                    //subscribe后得到的消息会执行到这里面
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //收到消息标志位
                    msg.obj = topicName + "---" + message.toString();
                    handler.sendMessage(msg);    // hander 回传
                }
            });
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
    private void Mqtt_connect() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    if(!(client.isConnected()) )  //如果还未连接
                    {
                        client.connect(options);
                        Message msg = new Message();
                        msg.what = 31;
                        handler.sendMessage(msg);
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    Message msg = new Message();
                    msg.what = 30;
                    handler.sendMessage(msg);
                }
            }
        }).start();
    }
    private void startReconnect() {
        scheduler = Executors.newSingleThreadScheduledExecutor();
        scheduler.scheduleAtFixedRate(new Runnable() {
            @Override
            public void run() {
                if (!client.isConnected()) {
                    Mqtt_connect();
                }
            }
        }, 0 * 1000, 10 * 1000, TimeUnit.MILLISECONDS);
    }
    private void publishmessageplus(String topic,String message2)
    {
        if (client == null || !client.isConnected()) {
            return;
        }
        MqttMessage message = new MqttMessage();
        message.setPayload(message2.getBytes());
        try {
            client.publish(topic,message);
        } catch (MqttException e) {

            e.printStackTrace();
        }
    }





}
