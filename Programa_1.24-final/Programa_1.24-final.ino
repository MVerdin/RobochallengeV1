#include <NewPing.h>
#include <Servo.h>

//Conexiones
const int triggerder = 28;
const int echoder = 30;
const int triggerizq = 24;
const int echoizq = 26;
const int triggeratr = 32;
const int echoatr = 34;
const int pinservo1 = 48;
//const int sensorInfrarrojo = 13;
const int motorrodillo = 38;
const int motor1a = 40;
const int motor1b = 42;
const int motor2a = 44;
const int motor2b = 46;
const int pinencoderder = 50;
const int pinescaner = 52;

//Dimensiones
const int anchoarena = 210;


//Ajustes escaner ultrasonico
const int resolucion_angulo = 8;
const int retardopaso = 2 * resolucion_angulo + 50;
const int iteraciones = 5;
const int distanciamaxima = 130;
const int anguloMin = 50, anguloMax = 130;
int escalad1 = 2, escalad2 = 25;
float sensibilidad = (40 * escalad1 * escalad2) / (resolucion_angulo * resolucion_angulo) ;
int rangosensibilidad = 70;

//Ajustes sensores ultrasonicos
const int tolerancia = 50;
const int paso = 10;
const int sensibilidadUS = 60;
const int iteracionesUS = 1;
const int distanciamaximaUS=200;

//Ajustes movimiento
const int errorangulo = 20;
const int intentosescaneo = 1;

//Constantes
const boolean derecha = true, izquierda = false;
const boolean adelante = true, atras = false;

//Declaracion de variables
int dist = 0;
int i, contador;
int angulo;
int coordenadaspolares[180];
int angulobjetivo;
int direccionobjetivoUS = 0;//1 derecha, 2 izquierda, 3 atras, 0 ninguno
boolean objetivoencontrado = false;
boolean avanzando, retrocediendo, girandoderecha, girandoizquierda;
int distanciaobjetivo;
int escaneosfallidos = 0;
int distanciaderecha, distanciaizquierda, distanciaatras;

//Declaracion de dispositivos
Servo servo;
NewPing sonarderecha(triggerder, echoder, distanciamaximaUS);
NewPing sonarizquierda(triggerizq, echoizq, distanciamaximaUS);
NewPing sonaratras(triggeratr, echoatr, distanciamaximaUS);
NewPing sonarescaner(pinescaner, pinescaner, distanciamaxima);






void setup() {
  // put your setup code here, to run once:
  servo.attach(pinservo1);
  pinMode(motor1a, OUTPUT);
  pinMode(motor1b, OUTPUT);
  pinMode(motor2a, OUTPUT);
  pinMode(motor2b, OUTPUT);
  //  pinMode(motorrodillo, OUTPUT);
  pinMode(pinencoderder, INPUT);
  Serial.begin(115200);
  Serial1.begin(9600);
  servo.write(90);
  detenerse();
  delay(5);
  //  digitalWrite(motorrodillo, LOW);

}

void loop() {
//  giraralcentro();
  escanear();
  if (objetivoencontrado) {
    atacar();
  }
  else {
    medirdistancias();
    avanzarcm(abs(distanciaatras-medirdistanciaangulo(90))/2, adelante);
    escanear();
    if (objetivoencontrado) {
      atacar();
    }
    else {
      girar(120, derecha);
      escanear();
      if (objetivoencontrado) {
        atacar();
      }
      else {
        girar(120, derecha);
        escanear();
        if (objetivoencontrado) {
          atacar();
        }

        else {
          girar(120, derecha);
          escanear();
          if (objetivoencontrado) {
            atacar();
          }
          else {
            girar(120, derecha);
            escanear();
            if (objetivoencontrado) {
              atacar();
            }
            while (medirdistanciaangulo(90) > 45) {
              avanzar();
            }
            detenerse();
          }
        }
      }
    }
  }
}





void atacar() {
  escaneosfallidos = 0;
  while (escaneosfallidos <= intentosescaneo) {
    if (objetivoencontrado) {
      escaneosfallidos = 0;
      if (angulobjetivo > (90 - errorangulo) && angulobjetivo < (90 + errorangulo)) {
        if (medirdistanciaangulo(90) > 40) {
          avanzar();
        }
        else{
        avanzar();
        delay(500);
        detenerse();
        }
      }
      else {
        if (angulobjetivo < 90) {
          girar(90 - angulobjetivo, derecha);
          avanzar();
        }
        else {
          girar(angulobjetivo - 90, izquierda);
          avanzar();
        }
      }
    }
    else {
      escaneosfallidos++;
    }
    escanear();
  }
  medirdistancias();
  girar((anguloMax - anguloMin) - 10, distanciaizquierda > distanciaderecha ? izquierda : derecha);
}



