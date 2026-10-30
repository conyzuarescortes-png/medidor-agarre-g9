/*
  Medidor de presion de agarre - Grupo 9 (UDD, Electronica Biomedica)
  Juego del pajaro controlado por presion: BMP180 + ESP32-C3 + pagina web.

  Placa:     LILYGO T-OI Plus (ESP32-C3). En Arduino IDE: "ESP32C3 Dev Module"
             (o "TTGO T-OI PLUS RISC-V ESP32-C3"), USB CDC On Boot: Disabled.
  Sensor:    BMP180 por I2C. SDA = GPIO19, SCL = GPIO18 (conector Grove / J3 de la PCB).
  Biblioteca: Adafruit BMP085 Library (compatible con BMP180) + Adafruit BusIO.

  Uso: cargar el programa, conectarse a la red WiFi "Agarre-G9" (clave 12345678)
       y abrir http://192.168.4.1 en el navegador.

  Estado: version usada en las pruebas iniciales. Pendiente para la siguiente entrega:
  lectura de los pulsadores de la PCB (GPIO6 = CERO, GPIO7 = INICIO/STOP) y
  LED de umbral (GPIO4).
*/
#include <Wire.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_BMP085.h>

const char* RED   = "Agarre-G9";
const char* CLAVE = "12345678";

Adafruit_BMP085 bmp;
WebServer server(80);

long presionCero = 0, dif = 0, maxDif = 0;   // Pa
bool fueraRango = false;
unsigned long ultimaLectura = 0;

const char PAGINA[] PROGMEM = R"rawliteral(
<!DOCTYPE html><html lang="es"><head><meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Pajaro de agarre</title>
<style>
body{margin:0;padding:8px;background:#111;color:#eee;font-family:sans-serif;text-align:center}
canvas{width:100%;max-width:700px;border-radius:8px;touch-action:none}
.row{margin:8px}
button{font-size:16px;padding:10px 16px;margin:4px;border:0;border-radius:8px;background:#2196f3;color:#fff}
input{width:70%}
</style></head><body>
<h3>Aprieta para subir, suelta para bajar</h3>
<canvas id="c" width="700" height="400"></canvas>
<div class="row">Presion: <b id="p">0.00</b> kPa &nbsp;|&nbsp; Maximo: <b id="m">0.00</b> kPa</div>
<div class="row">Sensibilidad: llegar arriba con <b id="ev">5</b> kPa<br>
<input type="range" id="esc" min="1" max="15" step="0.5" value="5"></div>
<button onclick="fetch('/cero')">Poner en cero</button>
<button onclick="reiniciar()">Reiniciar juego</button>
<script>
const c=document.getElementById('c'),g=c.getContext('2d');
const W=c.width,H=c.height;
const BX=140,R=16,HUECO=140,VEL=2.5,ANCHO=60;
let kpa=0,birdY=H-40,cols=[],puntaje=0,record=0,vivo=true,t=0;
const esc=document.getElementById('esc');
esc.oninput=()=>document.getElementById('ev').textContent=esc.value;
c.onclick=()=>{if(!vivo)reiniciar();};
async function leer(){
  try{
    const d=await (await fetch('/datos')).json();
    kpa=d.dif/1000;
    document.getElementById('p').textContent=kpa.toFixed(2);
    document.getElementById('m').textContent=(d.max/1000).toFixed(2);
  }catch(e){}
  setTimeout(leer,100);
}
leer();
function nuevaColumna(){const margen=50;cols.push({x:W,y:margen+Math.random()*(H-2*margen-HUECO),ok:false});}
function reiniciar(){cols=[];puntaje=0;vivo=true;t=0;}
function pajaro(x,y){
  const ala=Math.sin(Date.now()/80)*5;
  g.fillStyle='#42a5f5';g.beginPath();g.arc(x,y,R,0,2*Math.PI);g.fill();
  g.fillStyle='#1565c0';g.beginPath();g.ellipse(x-5,y+3+ala,9,5,0,0,2*Math.PI);g.fill();
  g.fillStyle='#fff';g.beginPath();g.arc(x+6,y-5,5,0,2*Math.PI);g.fill();
  g.fillStyle='#000';g.beginPath();g.arc(x+8,y-5,2.5,0,2*Math.PI);g.fill();
  g.fillStyle='#ff7043';g.beginPath();g.moveTo(x+R-2,y-1);g.lineTo(x+R+10,y+3);g.lineTo(x+R-2,y+7);g.fill();
}
function loop(){
  const frac=Math.max(0,Math.min(kpa/parseFloat(esc.value),1));
  const objetivoY=(H-R-10)-frac*(H-2*R-20);
  birdY+=(objetivoY-birdY)*0.2;
  if(vivo){
    t++;
    if(t%110===0) nuevaColumna();
    cols.forEach(o=>o.x-=VEL);
    cols=cols.filter(o=>o.x>-ANCHO);
    for(const o of cols){
      if(!o.ok && o.x+ANCHO<BX-R){o.ok=true;puntaje++;if(puntaje>record)record=puntaje;}
      const enX=BX+R>o.x && BX-R<o.x+ANCHO;
      const enHueco=birdY-R>o.y && birdY+R<o.y+HUECO;
      if(enX && !enHueco) vivo=false;
    }
  }
  g.fillStyle='#bde3f7';g.fillRect(0,0,W,H);
  g.fillStyle='#6d4c41';
  cols.forEach(o=>{g.fillRect(o.x,0,ANCHO,o.y);g.fillRect(o.x,o.y+HUECO,ANCHO,H-o.y-HUECO);});
  g.fillStyle='#66bb6a';g.fillRect(0,H-8,W,8);
  pajaro(BX,birdY);
  g.fillStyle='#000';g.textAlign='center';g.font='bold 30px sans-serif';
  g.fillText(puntaje,W/2,40);
  g.font='16px sans-serif';g.textAlign='left';g.fillText('Record: '+record,10,24);
  if(!vivo){
    g.fillStyle='rgba(0,0,0,0.5)';g.fillRect(0,0,W,H);
    g.fillStyle='#fff';g.textAlign='center';
    g.font='bold 36px sans-serif';g.fillText('Chocaste! Puntaje: '+puntaje,W/2,H/2);
    g.font='20px sans-serif';g.fillText('Toca la pantalla para jugar de nuevo',W/2,H/2+40);
  }
  requestAnimationFrame(loop);
}
loop();
</script></body></html>
)rawliteral";

// Promedia n lecturas para obtener una referencia estable (puesta a cero)
long promedioPresion(int n) {
  long suma = 0;
  for (int i = 0; i < n; i++) suma += bmp.readPressure();
  return suma / n;
}

void ponerEnCero() {
  presionCero = promedioPresion(20);
  maxDif = 0;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(19, 18);   // SDA, SCL

  if (!bmp.begin()) {
    Serial.println("ERROR: BMP180 no encontrado. Revisa SDA/SCL y alimentacion.");
    while (1) delay(100);
  }
  ponerEnCero();

  WiFi.softAP(RED, CLAVE);
  Serial.print("Conectate a la red ");
  Serial.print(RED);
  Serial.print(" y abre http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() { server.send_P(200, "text/html", PAGINA); });
  server.on("/datos", []() {
    String j = "{\"dif\":" + String(dif) + ",\"max\":" + String(maxDif) +
               ",\"sat\":" + String(fueraRango ? "true" : "false") + "}";
    server.send(200, "application/json", j);
  });
  server.on("/cero",  []() { ponerEnCero(); server.send(200, "text/plain", "ok"); });
  server.on("/reset", []() { maxDif = 0;    server.send(200, "text/plain", "ok"); });
  server.begin();
}

void loop() {
  server.handleClient();

  if (millis() - ultimaLectura >= 100) {   // ~10 lecturas por segundo
    ultimaLectura = millis();
    long p = bmp.readPressure();           // presion absoluta (Pa)
    dif = p - presionCero;                 // variacion respecto del cero
    if (dif > maxDif) maxDif = dif;
    fueraRango = (p > 110000);             // limite superior del BMP180

    // Registro temporal por puerto serie (compatible con Serial Plotter)
    Serial.print("Dif_Pa:");
    Serial.print(dif);
    Serial.print(" Max_Pa:");
    Serial.println(maxDif);
  }
}
