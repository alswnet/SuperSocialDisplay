//https://www.npmjs.com/package/get-json
//https://www.npmjs.com/package/mqtt
console.log("Inicianodo Servidor")

var getJSON = require('get-json')
let IDCanal = "UCS5yb75qx5GFOG-uV5JLYlQ";
let APIKey = "xxx"
let CantidadSub = "0";

// Esta informacion se tiene que cambiar dependiendo tu cuenta
// y el Broker MQTT que utilices
let BrokerMQTT = 'broker.shiftr.io';
let PuertoMQTT = 80;
let ClienteIDMQTT = "MQTT-P5";
let UsuarioMQTT = 'chepecarlos';
let ContrasenaMQTT = 'secretoespecial';
let OpcionesMQTT = {
  port: PuertoMQTT,
  username: UsuarioMQTT,
  password: ContrasenaMQTT,
  clientId: "ServidorRedes"
}

var mqtt = require('mqtt')
var client = mqtt.connect("ws://" + BrokerMQTT, OpcionesMQTT)

client.on('connect', function() {
  console.log("MQTT Conectado");
})

client.on('message', function(topic, message) {
  console.log(message.toString())
  client.end()
})

function ColsultarYoutube() {
  getJSON("https://www.googleapis.com/youtube/v3/channels?part=statistics&id=" + IDCanal + "&key=" + APIKey, function(error, response) {
    CantidadSub = response.items[0].statistics.subscriberCount;
  });
}

function EnviarYoutube() {
  console.log("Enviando: " + CantidadSub);
  client.publish('ALSW/Redes/Youtube', CantidadSub);
}

ColsultarYoutube();
setInterval(EnviarYoutube, 5000);
setInterval(ColsultarYoutube, 10000);
