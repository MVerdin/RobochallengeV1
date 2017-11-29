//Funciones para el control de los motores

//Girar el robot un angulo en sentido: derecho=1, izquierdo=0
void girar (int angulo, boolean sentido) {
  int pulsos;
  int contador = 0;
  long temp = millis();
  pulsos = angulo * 12.77777777;
  if (sentido)
    girarderecha();
  else
    girarizquierda();

  while (contador < pulsos && (millis() < (temp + 3000))) {
    if (digitalRead(pinencoderder)) {
      temp = millis();
      contador++;
      while (digitalRead(pinencoderder)) {}
    }

  }
  detenerse();
}

//Avanzar el robot una distancia en cm en direccion: adelante=1, atras=0
void avanzarcm(int distancia, boolean direccion) {
  int pulsos = 0;
  int contador = 0;
  long temp = millis();
  pulsos = distancia * 42.4413;
  if (direccion)
    avanzar();
  else
    retroceder();

  while (contador < pulsos && (millis() < (temp + 2000))) {
    if (digitalRead(pinencoderder)) {
      temp = millis();
      contador++;
      while (digitalRead(pinencoderder)) {}
    }
  }
  detenerse();
}

//configura los puentes H para avanzar
void avanzar() {
  Serial1.println("avanzando");
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, HIGH);
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, HIGH);
  avanzando = false;
  retrocediendo = true;
  girandoderecha = false;
  girandoizquierda = false;
}
//configura los puentes H para retroceder
void retroceder() {
  //  Serial1.println("retrocediendo");
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2a, HIGH);
  digitalWrite(motor2b, LOW);
  avanzando = true;
  retrocediendo = false;
  girandoderecha = false;
  girandoizquierda = false;
}
//configura los puentes H para girar a la derecha
void girarderecha() {
  //  Serial1.println("girando a la derecha");
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, HIGH);
  digitalWrite(motor2a, HIGH);
  digitalWrite(motor2b, LOW);
  avanzando = false;
  retrocediendo = false;
  girandoderecha = false;
  girandoizquierda = true;
}
//configura los puentes H para girar a la izquierda
void girarizquierda() {
  //  Serial1.println("girando a la izquierda");
  digitalWrite(motor1a, HIGH);
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, HIGH);
  avanzando = false;
  retrocediendo = false;
  girandoderecha = true;
  girandoizquierda = false;
}
//configura los puentes H para frenar los motores
void detenerse() {
  digitalWrite(motor1a, LOW);
  digitalWrite(motor1b, LOW);
  digitalWrite(motor2a, LOW);
  digitalWrite(motor2b, LOW);
  avanzando = false;
  retrocediendo = false;
  girandoderecha = false;
  girandoizquierda = false;
  //  Serial1.println("detenido");
}
