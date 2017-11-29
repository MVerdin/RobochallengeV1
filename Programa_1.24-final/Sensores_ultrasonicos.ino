//Funciones usadas para la ubicacion del robot dentro de la arena con la ayuda de los sensores ultrasonicos laterales y el sensor trasero

//Mide las distancias de los tres sensores
void medirdistancias() {
  distanciaderecha = sonarderecha.convert_cm(sonarderecha.ping_median(iteracionesUS));
  distanciaderecha = distanciaderecha == 0 ? distanciamaximaUS : distanciaderecha;
  distanciaizquierda = sonarizquierda.convert_cm(sonarizquierda.ping_median(iteracionesUS));
  distanciaizquierda = distanciaizquierda == 0 ? distanciamaximaUS : distanciaizquierda;
  distanciaatras = sonaratras.convert_cm(sonaratras.ping_median(iteracionesUS));
  distanciaatras = distanciaatras == 0 ? distanciamaximaUS : distanciaatras;
}

//Con la ayuda de las mediciones obtenidoas el robot gira hasta que esta en direccion al centro de la arena
boolean giraralcentro() {
  int distanciaderechaant, distanciaizquierdaant, distanciaatrasant;
  boolean objetivodetectado = false;
  medirdistancias();
  distanciaderechaant = distanciaderecha;
  distanciaizquierdaant = distanciaizquierda;
  distanciaatrasant = distanciaatras;


  while ((distanciaderecha - distanciaizquierda) > tolerancia || (distanciaderecha - distanciaizquierda) < (-tolerancia) || distanciaatras > (anchoarena / 2) - 20) {
    if  (distanciaderecha > distanciaizquierda) {
      girar(paso, derecha);
    }
    else {
      girar(paso, izquierda);
    }
    medirdistancias();
    if (distanciaderecha < (distanciaderechaant - sensibilidadUS)) {
      objetivodetectado = true;
      direccionobjetivoUS = 1;
    } else if (distanciaizquierda < (distanciaizquierdaant - sensibilidadUS)) {
      objetivodetectado = true;
      direccionobjetivoUS = 2;
    } else if (distanciaatras < (distanciaatrasant - sensibilidadUS)) {
      objetivodetectado = true;
      direccionobjetivoUS = 3;
    } else {
      objetivodetectado = false;
      direccionobjetivoUS = 0;
    }
    //    if (objetivodetectado) {
    //      break;
    //    }
    distanciaderechaant = distanciaderecha;
    distanciaizquierdaant = distanciaizquierda;
    distanciaatrasant = distanciaatras;
  }
  return objetivodetectado;
}





