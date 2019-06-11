//https://www.npmjs.com/package/get-json
//https://www.npmjs.com/package/mqtt

console.log("Inicianodo Servidor")

var getJSON = require('get-json')
let IDCanal = "UCS5yb75qx5GFOG-uV5JLYlQ";
let APIKey = "AIzaSyBZrzRZblsHgHn1yLc7U9ioH9Jz9n8CdBw"
let CantidadYoutube = "0";
let CantidadIntagram = "0";

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

let UsuarioInstagram = "alswnet";

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
    CantidadYoutube = response.items[0].statistics.subscriberCount;
  });
}

function EnviarYoutube() {
  console.log("Enviando: " + CantidadYoutube);
  client.publish('ALSW/Redes/Youtube', CantidadYoutube);
}

//ColsultarYoutube();
//ColsultarInstagram();
//setInterval(EnviarYoutube, 5000);
//setInterval(ColsultarYoutube, 10000);

//import Instagram from 'node-instagram';
// or
const Instagram = require('node-instagram').default;

// Create a new instance.
const instagram = new Instagram({
  clientId: 'c9094c14f42b4a3a89e7ecff2d28fe9e',
  clientSecret: '0e829ac146bb46f4b481f6de7461d4a3'
});

// You can use callbacks or promises
instagram.get('users/self', (err, data) => {
  if (err) {
    // an error occured
    console.log(err);
  } else {
    console.log(data);
  }
});
