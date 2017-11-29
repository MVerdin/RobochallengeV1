//Funciones para controlar el escaner ultrasonico delantero

//Funcion basica para obtener medicion del sensor
int medirdistancia() {
  int distancia;
  distancia = sonarescaner.convert_cm(sonarescaner.ping_median(iteraciones));
  distancia = distancia != 0 ? distancia : distanciamaxima;
//  int i;
//  float sensorValue = 0;
//  for (i = 0; i < iteraciones; i++) {
//    analogRead(sensorInfrarrojo);
//    sensorValue = sensorValue + analogRead(sensorInfrarrojo);
//    delay(40);
//  }
//  sensorValue = sensorValue / iteraciones;
//  float cm = 10650.08 * pow(sensorValue, -0.935) - 10;
//  cm = cm > 150 ? 150 : cm;
//  return roundf(cm);
  return distancia;
}

//Situa el servomotor a un angulo determinado y regresa la medicion obtenida
int medirdistanciaangulo(int angulo) {
  int retardo = 2 * abs(servo.read() - angulo) + 40;
  servo.write(angulo);
  delay(retardo);
  return medirdistancia();
}

//Realiza un escaneo completo con un campo de vision determinado por anguloMin y anguloMax
//Guarda los datos en un vector llamado coordenadaspolares
void escanear() {
  if (servo.read() < 90) {
    for (angulo = anguloMin; angulo <= anguloMax; angulo = angulo + resolucion_angulo) {
      servo.write(angulo);
      delay(retardopaso);
      coordenadaspolares[angulo] = medirdistancia();
      Serial.print('M');
      Serial.print(angulo);
      Serial.print(' ');
      Serial.println(coordenadaspolares[angulo]);
      Serial1.print('M');
      Serial1.print(angulo);
      Serial1.print(' ');
      Serial1.println(coordenadaspolares[angulo]);

    }
  }
  else {
    for (angulo = anguloMax; angulo >= anguloMin; angulo = angulo - resolucion_angulo) {
      servo.write(angulo);
      delay(retardopaso);
      coordenadaspolares[angulo] = medirdistancia();
      Serial.print('M');
      Serial.print(angulo);
      Serial.print(' ');
      Serial.println(coordenadaspolares[angulo]);
      Serial1.print('M');
      Serial1.print(angulo);
      Serial1.print(' ');
      Serial1.println(coordenadaspolares[angulo]);

    }
  }
  analizarDatos();
}

//Obtiene la primera y segunda derivada de los datos guardados en el escaneo
//Busca patrones en la primera y segunda derivada que sugieran la presencia de un objetivo
//Si se encuentra un objetivo pone en verdadero la variable booleana global "objetivoencontrado"
int analizarDatos() {
  float primeratazadecambio[180];
  float segundatazadecambio[180];
  int j, k, l, m;
  int valor1, valor2, angulo1, angulo2, angulo3;

  boolean punto1 = false, punto2 = false, punto3 = false, punto4 = false, punto5 = false;
  objetivoencontrado = false;
  for (j = anguloMin; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {
    primeratazadecambio[(2 * j + resolucion_angulo) / 2] = (coordenadaspolares[j] -
        coordenadaspolares[j + resolucion_angulo]) * escalad1 / resolucion_angulo;
  }

  for (j = anguloMin + (resolucion_angulo / 2); j <= anguloMax - (resolucion_angulo / 2); j = j + resolucion_angulo) {
    Serial.print('P');
    Serial.print(j);
    Serial.print(' ');
    Serial.println(primeratazadecambio[j]);
  }
  for (j = anguloMin + (resolucion_angulo / 2); j <= anguloMax - (3 * resolucion_angulo / 2); j = j + resolucion_angulo) {
    segundatazadecambio[(2 * j + resolucion_angulo) / 2] = (primeratazadecambio[j] -
        primeratazadecambio[j + resolucion_angulo]) * escalad2 / resolucion_angulo;
  }

  for (j = anguloMin + resolucion_angulo; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {
    Serial.print('S');
    Serial.print(j);
    Serial.print(' ');
    Serial.println(segundatazadecambio[j]);
  }
  if (!objetivoencontrado) {
    for (j = anguloMin + resolucion_angulo; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {

      if (segundatazadecambio[j] < -sensibilidad) {
        if (punto1 == false && punto2 == false && punto3 == false && punto4 == false)
          punto1 = true;
        else if (punto1 == true && punto2 == true && punto3 == false && punto4 == false)
          punto2 = false;
        else if (punto1 == true && punto2 == true && punto3 == true && punto4 == false)
          punto4 = true;
      }
      else if (segundatazadecambio[j] > sensibilidad) {
        if (punto1 == true && punto2 == false && punto3 == false && punto4 == false) {
          punto2 = true;
          angulo1 = j;
        } else if (punto1 == true && punto2 == true && punto3 == false && punto4 == false) {
          punto3 = true;
          angulo2 = j;
        } else if (punto1 == true && punto2 == true && punto3 == true && punto4 == false) {
          punto1 = false;
          punto2 = false;
          punto3 = false;
        }
      } else {
        if (punto1 = true && punto2 == true && punto3 == false && punto4 == false) {
          punto5 = true;
        }

      }
    }
    if (punto1 && punto2 && punto3 && punto4 && punto5) {
      objetivoencontrado = true;
      angulobjetivo = ((angulo1 + angulo2) / 2);
      Serial.println("cuatro punto encontrado");
    }
  }

  if (!objetivoencontrado) {
    punto1 = false;
    punto2 = false;
    punto3 = false;
    punto4 = false;

    for (j = anguloMin + resolucion_angulo; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {
      if (segundatazadecambio[j] < sensibilidad && segundatazadecambio[j] > (-sensibilidad)) {
        angulobjetivo = j;
        if (segundatazadecambio[j + 1 * resolucion_angulo] > sensibilidad&&(j + 1 * resolucion_angulo)<=anguloMax) {
          if (segundatazadecambio[j + 2 * resolucion_angulo] < (-sensibilidad)&&(j + 1 * resolucion_angulo)<=anguloMax) {
            if (segundatazadecambio[j + 3 * resolucion_angulo] < sensibilidad && segundatazadecambio[j] > (-sensibilidad)&&(j + 1 * resolucion_angulo)<=anguloMax) {
              objetivoencontrado = true;
              
            }
          }
        }
      }
    }
  }

  if (!objetivoencontrado) {
    punto1 = false;
    punto2 = false;
    punto3 = false;
    punto4 = false;

    for (j = anguloMin + resolucion_angulo; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {
      if (segundatazadecambio[j] < sensibilidad && segundatazadecambio[j] > (-sensibilidad)) {
        if (segundatazadecambio[j + 1 * resolucion_angulo] < (-sensibilidad)&&(j + 1 * resolucion_angulo)<=anguloMax) {
          if (segundatazadecambio[j + 2 * resolucion_angulo] > sensibilidad&&(j + 2 * resolucion_angulo)<=anguloMax) {
            if (segundatazadecambio[j + 3 * resolucion_angulo] < sensibilidad && segundatazadecambio[j] > (-sensibilidad)&&(j + 3 * resolucion_angulo)<=anguloMax) {
              objetivoencontrado = true;
              angulobjetivo = j + 3 * resolucion_angulo;
            }
          }
        }
      }
    }
  }

//  if (!objetivoencontrado) {
//    punto1 = false;
//    punto2 = false;
//    punto3 = false;
//    punto4 = false;
//
//    for (j = anguloMin + resolucion_angulo; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {
//      if (segundatazadecambio[j] > sensibilidad && segundatazadecambio[j] < (sensibilidad + rangosensibilidad)) {
//        if (punto1 == false && punto2 == false) {
//          punto1 = true;
//          angulo1 = j;
//        }
//        else if (punto1 == true && punto2 == false) {
//          punto2 = true;
//          angulo2 = j;
//        }
//        else if (punto1 == true && punto2 == true) {
//          if (segundatazadecambio[j] > segundatazadecambio[angulo1] && segundatazadecambio[angulo2] > segundatazadecambio[angulo1]) {
//            angulo1 = angulo2;
//            angulo2 = j;
//          } else if (segundatazadecambio[j] > segundatazadecambio[angulo2] && segundatazadecambio[angulo1] > segundatazadecambio[angulo2]) {
//            angulo2 = j;
//          }
//        }
//      } else if (segundatazadecambio[j] > (-sensibilidad) && punto1 && !punto2) {
//        punto3 = true;
//      }
//    }
//    if (punto1 == true && punto2 == true && punto3) {
//      objetivoencontrado = true;
//      angulobjetivo = (angulo1 + angulo2) / 2;
//      Serial.println("dos punto encontrado");
//    }
//  }





//  if (!objetivoencontrado) {
//    int nummayor = 0;
//    for (j = anguloMin + resolucion_angulo; j <= anguloMax - resolucion_angulo; j = j + resolucion_angulo) {
//      if (nummayor < segundatazadecambio[j]) {
//        nummayor = segundatazadecambio[j];
//        angulo1 = j;
//      }
//    }
//    if (nummayor > sensibilidad && nummayor < (sensibilidad + rangosensibilidad)) {
//      objetivoencontrado = true;
//      angulobjetivo = angulo1;
//      Serial.print("un punto encontrado");
//      //          Serial.print(sensibilidad);
//      //          Serial.println(nummayor);
//    }
//  }

  if (objetivoencontrado) {
    Serial.print('O');
    Serial.println(angulobjetivo);
  }
}






