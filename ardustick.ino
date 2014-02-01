/**
* Ardustick v0.1
* Mathias Djärv (2014)
* http://google.com/+MathiasDjärv
*
* Arduino sketch to control Nexa-compatible devices through serial commands
* See README.md for more details.
*
* This sketch requires the NexaCtrl library which can be found at:
* https://github.com/calle-gunnarsson/NexaCtrl
*
*/

#include <NexaCtrl.h>


// Pins used
#define TX_PIN 2
#define RX_PIN 3 // Not used
#define LED 13 // LED indicating a command is being sent

// Number of times to repeat commands, higher numbers improve reliability but
// makes the command take longer to complete, I found 4 iterations to be a
// good balance and takes about 15ms
#define REPEATS 4


// The Nexa object used to control the relays
NexaCtrl nexaCtrl(TX_PIN, RX_PIN);

// Variables used to read and "tokenize" serial commands
static const int serialCommandBufferSize = 1024;
char serialCommandBuffer[serialCommandBufferSize];
byte serialCommandIndex = 0;
bool serialCommandComplete = false;

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(57600);
}


void loop()
{
  // Read serial command
  getSerialCommand();      
}


// Set a device to on or off
void setReceiver(unsigned long controller, int device, int on) {

  // Start building the response string
  Serial.print("{\"success\":true,\"controller\":\"");
  Serial.print(controller);
  Serial.print("\",\"device\":\"");
  Serial.print(device);
  Serial.print("\",\"sent\":\"");

  // Turn on LED to indicate transmission
  digitalWrite(LED, HIGH);

  // Send command multiple times in a row to improve reliability
  if(on == 0) {
    for(int i = 0; i < REPEATS; i++) {
      nexaCtrl.DeviceOff(controller, device);
    }

    Serial.println("off\"}");
  } else {
    for(int i = 0; i < REPEATS; i++) {
      nexaCtrl.DeviceOn(controller, device);
    }

    Serial.println("on\"}");
  }

  // Flush the buffer and turn off the LED
  Serial.flush();
  digitalWrite(LED, LOW);
}


// Read a command from serial input
void getSerialCommand() {
  // Read until '\n' before processing
  while (Serial.available() > 0) {
    if(serialCommandIndex < (serialCommandBufferSize-1)) {
      char inChar = Serial.read();
      if(inChar == '\n') {
        serialCommandComplete = true;
        break;
      }
      serialCommandBuffer[serialCommandIndex] = inChar;
      serialCommandIndex++;
      serialCommandBuffer[serialCommandIndex] = '\0';
    }
  }

  // '\n' found, time to parse it
  if(serialCommandComplete) {
    // Separate the first part of the comma-separated command
    char *result = NULL;
    result = strtok(serialCommandBuffer, "," );
    
    // Process it
    if(processCommand(result) == false) {
      Serial.println("{\"success\":false,\"message\":\"Command not understood, available commands are: SET,remote,device,on and GET_REMOTE_ID\"}");
      Serial.flush();
    }

    // Clear buffer
    serialCommandBuffer[0] = '\0';
    serialCommandIndex = 0;
    serialCommandComplete = false;
  }
}


// Process a the command buffer
bool processCommand(String cmd) {
  int resultIndex = 0;

  if(cmd == "SET") {
    unsigned long controller = -1;
    int device = -1;
    int on = -1;

    char *result = NULL;
    result = strtok( NULL, "," );

    // Iterate the tokenized string and sort the values into appropriate variables
    while( result != NULL ) {
      if(resultIndex == 0)
        controller = atol(result);
      if(resultIndex == 1)
        device = atoi(result);
      if(resultIndex == 2)
        on = atoi(result);

      resultIndex++;
      result = strtok( NULL, "," );
    }

    // Sanity check for the values
    if(controller <= 0 || device == -1 || on == -1) {
      return false;
    }

    setReceiver(controller, device, on);

    return true;
  }

  return false;
}

