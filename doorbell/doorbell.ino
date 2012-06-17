#include <SPI.h>
#include <Ethernet.h>

/**** Configuration ***/
// MAC address of the Shield
byte mac[] = {  0x90, 0xA2, 0xDA, 0x00, 0x09, 0xBF };
// IP address of the server
IPAddress server(192,168,1,8);                          
/**** Configuration ***/

#define HTTP_USER_AGENT "User-Agent: Doorbell"
#define HTTP_GET_DOORBELL "GET /doorbell HTTP/1.1"
#define HTTP_CONNECTION_CLOSE "Connection: close"

const int buttonPin = 2;     
const int ledPin =  13; 

EthernetClient client;

int buttonState = 0;         // variable for reading the pushbutton status
int prevButtonState = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(buttonPin, INPUT); 
  pinMode(ledPin, OUTPUT); 
  
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());  
}

void loop() {
  buttonState = digitalRead(buttonPin);
  
  if (buttonState != prevButtonState) {
    if (buttonState == HIGH) {     
      // turn LED on:    
      sendNotification();
      digitalWrite(ledPin, HIGH);  
    } 
    else {
      // turn LED off:
      digitalWrite(ledPin, LOW); 
    }
  }
  
 if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }  
  
  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  prevButtonState = buttonState;
}

void sendNotification() {
  // if you get a connection, report back via serial:
  if (client.connect(server, 3000)) {
    Serial.println("connected");
    // Make a HTTP request:
    client.println(HTTP_GET_DOORBELL);
    client.println(HTTP_USER_AGENT);
    client.println(HTTP_CONNECTION_CLOSE);
    client.println();
    client.stop();
  } 
  else {
    // kf you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}
