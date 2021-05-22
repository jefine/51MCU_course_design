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
    client = mqtt.connect('wxs://39.103.183.108/mqtt',options)
    client.on('connect',(e)=>{
      console.log('mqtt connect success~')
      client.subscribe('/iot/40/app',{
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
  taphere(){
    console.log("you have tap it")
  },
  change:function(e){
    this.setData({text : e.toString()}),
    console.log('change is over')
  }
})
