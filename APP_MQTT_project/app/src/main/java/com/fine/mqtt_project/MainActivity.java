package com.fine.mqtt_project;

import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.Button;
import android.widget.*;
import android.widget.Toast;

import org.eclipse.paho.client.mqttv3.*;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {
    private String host = "tcp://39.103.183.108:1883";
    private String userName = "86fe9adc2a75f181efa129598c6844eb";
    private String passWord = "abc";
    private String mqtt_id = "app"; //
    private String mqtt_sub_topic = "/iot/40/device"; //
    private String mqtt_pub_topic = "/iot/40/app"; //
    private ScheduledExecutorService scheduler;
    private boolean led_flag = true;
    private Button btn_1;
    private ImageView image_1;
    private TextView textView_prad;
    private TextView text_test;
    private MqttClient client;
    private MqttConnectOptions options;
    private Handler handler;

    @SuppressLint("HandlerLeak")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button btn = findViewById(R.id.button2);
        text_test = findViewById(R.id.textView);
        textView_prad = findViewById(R.id.textView2);
        btn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                String msgled = new String();
                if(led_flag)msgled = "{\"set_led\":0}";
                else msgled = "{\"set_led\":1}";
                Toast.makeText(MainActivity.this,msgled,Toast.LENGTH_SHORT).show();
                try {
                    client.publish(mqtt_pub_topic,msgled.getBytes(),0,true);
                    led_flag = !led_flag;
                } catch (MqttException e) {
                    e.printStackTrace();
                }
            }

        });

        Mqtt_init();
        startReconnect();

        handler = new Handler() {
            @SuppressLint("SetTextI18n")
            public void handleMessage(Message msg) {
                super.handleMessage(msg);
                switch (msg.what){
                    case 1: //????????????????????????
                        break;
                    case 2:  // ????????????

                        break;
                    case 3:  //MQTT ??????????????????   UTF8Buffer msg=new UTF8Buffer(object.toString());
                        String val = "null";
                        //String msg_val = msg.obj.toString().substring(msg.obj.toString().indexOf("---"));
                       try {
                           val = msg.obj.toString().substring(msg.obj.toString().indexOf("---")+3);//.substring(msg.obj.toString().indexOf(":"),msg.obj.toString().indexOf("}");
                           //Toast.makeText(MainActivity.this,val,Toast.LENGTH_SHORT).show();
                       } catch (Exception e) {
                           e.printStackTrace();
                       }
                       switch(val.substring(0,5)){
                           case "temp:":{
                               text_test.setText("???????????????" + val.substring(6));
                               break;
                           }
                           case "prad:":{
                               textView_prad.setText("??????????????? "+val.substring(6));
                               break;
                           }
                       }

                        //Toast.makeText(MainActivity.this,msg.obj.toString(),Toast.LENGTH_SHORT).show();
                        break;
                    case 30:  //????????????
                        Toast.makeText(MainActivity.this,"????????????" ,Toast.LENGTH_SHORT).show();
                        break;
                    case 31:   //????????????
                        Toast.makeText(MainActivity.this,"????????????" ,Toast.LENGTH_SHORT).show();
                        try {
                            client.subscribe(mqtt_sub_topic,0);
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
            //host???????????????test???clientid?????????MQTT????????????ID?????????????????????????????????????????????MemoryPersistence??????clientid??????????????????????????????????????????
            client = new MqttClient(host, mqtt_id,
                    new MemoryPersistence());
            //MQTT???????????????
            options = new MqttConnectOptions();
            //??????????????????session,?????????????????????false??????????????????????????????????????????????????????????????????true??????????????????????????????????????????????????????
            options.setCleanSession(false);
            //????????????????????????
            options.setUserName(userName);
            //?????????????????????
            options.setPassword(passWord.toCharArray());
            // ?????????????????? ????????????
            options.setConnectionTimeout(10);
            // ???????????????????????? ???????????? ??????????????????1.5*20????????????????????????????????????????????????????????????????????????????????????????????????????????????
            options.setKeepAliveInterval(20);
            //????????????
            client.setCallback(new MqttCallback() {
                @Override
                public void connectionLost(Throwable cause) {
                    //????????????????????????????????????????????????
                    System.out.println("connectionLost----------");
                    //startReconnect();
                }
                @Override
                public void deliveryComplete(IMqttDeliveryToken token) {
                    //publish?????????????????????
                    System.out.println("deliveryComplete---------"
                            + token.isComplete());
                }
                @Override
                public void messageArrived(String topicName, MqttMessage message)
                        throws Exception {
                    //subscribe???????????????????????????????????????
                    System.out.println("messageArrived----------");
                    Message msg = new Message();
                    msg.what = 3;   //?????????????????????
                    msg.obj = topicName + "---" + message.toString();
                    handler.sendMessage(msg);    // hander ??????
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
                    if(!(client.isConnected()) )  //??????????????????
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