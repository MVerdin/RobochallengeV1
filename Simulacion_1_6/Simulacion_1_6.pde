import processing.serial.*;

Serial myPort;  // Create object from Serial class
int val;      // Data received from the serial port
String mensaje;
int pendiente, segundapendiente;
int angulo, angulo2, angulo3, radio;
int anguloobjetivo=90;
int[] radios = new int[181];
int[] pendientes = new int[181];
int[] segundaspendientes = new int[181];
int anchoventana=600, altoventana=600;
int i;
static int angulomenor=50, angulomayor=130;
int resolucionescaner=8;
int escala=2;

void setup() 
{
  size(600, 600);
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 115200);
  myPort.bufferUntil(10);
  background(255, 255, 255);
}

void draw()
{
  background(255, 255, 255);
  stroke(0, 0, 0);
  for (i=0; i<=180; i++) {
    if (radios[i]!=0) {
      line(pol2cartX(radios[i], i), pol2cartY(radios[i], i), 
        pol2cartX(radios[i+resolucionescaner], i+resolucionescaner), 
        pol2cartY(radios[i+resolucionescaner], i+resolucionescaner));
    }
  }

  stroke(0, 255, 0);
  for (i=angulomenor+(resolucionescaner/2); i<=angulomayor-(resolucionescaner/2); i=i+resolucionescaner) {

    line(pol2cartX(pendientes[i]+100, i), pol2cartY(pendientes[i]+100, i), 
      pol2cartX((pendientes[i+resolucionescaner])+100, i+resolucionescaner), 
      pol2cartY((pendientes[i+resolucionescaner])+100, i+resolucionescaner));
  }
  
   stroke(255, 255, 0);
  for (i=angulomenor+resolucionescaner; i<=angulomayor-resolucionescaner; i=i+resolucionescaner) {

    line(pol2cartX(segundaspendientes[i]+100, i), pol2cartY(segundaspendientes[i]+100, i), 
      pol2cartX((segundaspendientes[i+resolucionescaner])+100, i+resolucionescaner), 
      pol2cartY((segundaspendientes[i+resolucionescaner])+100, i+resolucionescaner));
  }

  stroke(0, 0, 255);
  line((anchoventana/2), (altoventana/2), pol2cartX(radio, angulo), pol2cartY(radio, angulo));
  stroke(255,0,0);
  line((anchoventana/2),(altoventana/2),pol2cartX(200,anguloobjetivo),pol2cartY(200,anguloobjetivo));
}

void serialEvent(Serial p) {
  if ( myPort.available() > 0) {  // If data is available,
    mensaje = myPort.readStringUntil(10);
    leermensaje();
  }
}

void leermensaje() {
  try {
    if (mensaje!=null) {

      if (mensaje.charAt(0)=='M') {
        angulo = parseInt(trim(mensaje.substring(mensaje.indexOf('M')+1, mensaje.indexOf(" "))));
        radio = parseInt(trim(mensaje.substring(mensaje.indexOf(" ")+1)));
        radios[angulo]=radio;
        print('M');  
        print(angulo);
        print(" ");
        println(radio);
      } else if (mensaje.charAt(0)=='P') {
        angulo2 = parseInt(trim(mensaje.substring(mensaje.indexOf('P')+1, mensaje.indexOf(" "))));
        pendiente = parseInt(trim(mensaje.substring(mensaje.indexOf(" ")+1)));
        pendientes[angulo2] = pendiente;

        //        print('P');  
        //        print(angulo2);
        //        print(" ");
        //        println(pendiente+100);
      } else if (mensaje.charAt(0)=='S') {
        angulo3 = parseInt(trim(mensaje.substring(mensaje.indexOf('S')+1, mensaje.indexOf(" "))));
        segundapendiente = parseInt(trim(mensaje.substring(mensaje.indexOf(" ")+1)));
        segundaspendientes[angulo3] = segundapendiente;
      }else if (mensaje.charAt(0)=='O') {
        anguloobjetivo=parseInt(trim(mensaje.substring(mensaje.indexOf('O')+1)));
      }
      //else if (mensaje.charAt(0)=='R') {
      //  for (i=0; i<=180; i++) {
      //    print('P');  
      //    print(i);
      //    print(" ");
      //    println(pendientes[i]);
      //  }
      //}
    }
  }
  catch(StringIndexOutOfBoundsException e) {
  }
  catch(NullPointerException e) {
  }
  // catch(ArrayOutOffBoundsException e){}
}

float pol2cartX(int r, int a) {
  float X;
  X=(anchoventana/2)+escala*r*cos(radians(a));
  return X;
}

float pol2cartY(int r, int a) {
  float Y;
  Y=(altoventana/2)-escala*r*sin(radians(a));
  return Y;
}