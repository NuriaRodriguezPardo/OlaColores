#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "tu_ssid";
const char* password = "tu_contraseña";

const int numRows = 3; // Número de filas en la matriz de LEDs
const int numCols = 4; // Número de columnas en la matriz de LEDs
const int ledPins[numRows][numCols] = {
  {2, 3, 4, 5},   // Ejemplo de pines para la matriz de LEDs
  {6, 7, 8, 9},
  {10, 11, 12, 13}
}; // Pines de los LEDs en la matriz

const int buttonPin = 14; // Pin del botón

WebServer server(80);

bool direccion = true; // true: derecha, false: izquierda

void handleRoot() {
  String paginaWeb = "<!DOCTYPE html>\
    <html lang=\"en\">\
    <head>\
      <meta charset=\"UTF-8\">\
      <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\
      <title>Simulación de LEDs</title>\
      <style>\
        .led {\
          width: 50px;\
          height: 50px;\
          border-radius: 50%;\
          display: inline-block;\
          margin: 10px;\
        }\
      </style>\
    </head>\
    <body>\
      <!-- Agregar divs para representar la matriz de LEDs -->";
      
  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      paginaWeb += "<div class=\"led\" id=\"led" + String(i * numCols + j) + "\"></div>";
    }
    paginaWeb += "<br>";
  }
  
  paginaWeb += "<script>\
        var leds = document.querySelectorAll('.led');\
\
        function encenderLED(numero) {\
          // Aquí enviarías una solicitud HTTP o WebSocket al Arduino para encender el LED correspondiente\
          leds[numero - 1].style.backgroundColor = 'red'; // Ejemplo: cambiar color del LED a rojo\
          fetch('/led?numero=' + numero + '&estado=encender'); // Enviar solicitud HTTP para encender el LED\
        }\
\
        function apagarLED(numero) {\
          // Aquí enviarías una solicitud HTTP o WebSocket al Arduino para apagar el LED correspondiente\
          leds[numero - 1].style.backgroundColor = 'gray'; // Ejemplo: cambiar color del LED a gris\
          fetch('/led?numero=' + numero + '&estado=apagar'); // Enviar solicitud HTTP para apagar el LED\
        }\
\
        function cambiarDireccion() {\
          // Aquí enviarías una solicitud HTTP o WebSocket al Arduino para cambiar la dirección de la ola\
          fetch('/direction')\
            .then(response => response.text())\
            .then(data => console.log('Dirección cambiada:', data))\
            .catch(error => console.error('Error al cambiar la dirección:', error));\
        }\
      </script>\
    </body>\
    </html>";
  server.send(200, "text/html", paginaWeb);
}

void handleLED() {
  int numero = server.arg("numero").toInt();
  String estado = server.arg("estado");

  if (estado == "encender") {
    digitalWrite(ledPins[(numero - 1) / numCols][(numero - 1) % numCols], HIGH); // Encender el LED correspondiente
  } else if (estado == "apagar") {
    digitalWrite(ledPins[(numero - 1) / numCols][(numero - 1) % numCols], LOW); // Apagar el LED correspondiente
  }
  server.send(200, "text/plain", "OK");
}

void handleDirection() {
  // Cambiar la dirección de la ola
  direccion = !direccion;
  server.send(200, "text/plain", direccion ? "Derecha" : "Izquierda");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  pinMode(buttonPin, INPUT_PULLUP); // Configurar el pin del botón como entrada con pull-up

  for (int i = 0; i < numRows; i++) {
    for (int j = 0; j < numCols; j++) {
      pinMode(ledPins[i][j], OUTPUT); // Configurar los pines de los LEDs como salidas
    }
  }

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }

  Serial.println("Conectado al WiFi");

  server.on("/", handleRoot);
  server.on("/led", handleLED);
  server.on("/direction", handleDirection);
  server.begin();
  Serial.println("Servidor iniciado");
}

