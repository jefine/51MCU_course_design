// index.js
// 获取应用实例
const app = getApp()
var mqtt = require('../../utils/mqtt.min.js')
var client = null
Page({
  data: {
    text: 'hello'
  },
  
  onLoad() {
    this.connectMqtt()
    
  },
  connectMqtt:function(){
    const options = {
      connectTimeout:4000,
      clientId:'wxmini1',
      port:8084,
      username:'86fe9adc2a75f181efa129598c6844eb',
      password:'abc'
    }
    client = mqtt.connect('wxs:t.yoyolife.fun/mqtt',options)
    client.on('connect',(e)=>{
      console.log('mqtt connect success~')
      client.subscribe('/iot/40/device',{
        qos:0
      },function(err){
        if(!err){
          console.log('mqtt successfully subscribe!')
        }
      })
    })
    client.on('message',function(topic,message){
      console.log('received : '+message.toString()),
      change([topic,message].join(":"))
    })
    client.on('reconnect',(error)=>{
      console.log('reconnecting',error)
    })
    client.on('error',(error)=>{
      console.log('error : ',error)
    })   
    
  },
  open(){
    console.log("you have open it")
    client.publish('/iot/40/app', '{"set_led":0}')
  },
  close(){
    console.log("you have close it")
    client.publish('/iot/40/app', '{"set_led":1}')
  },
})
