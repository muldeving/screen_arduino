// Example 5 - Receive with start- and end-markers combined with parsing

#include "U8glib.h"
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

const byte numChars = 100;
char receivedChars[numChars];
char tempChars[numChars];        // temporary array for use when parsing

      // variables to hold the parsed data
String s1, s2, s3, s4, s5, s6, s7, cpu, ram, rx
, tx, net, cput, d1, d2, d3, d4;
int i1, i2, i3, i4, i5, tssd, m, nbm, cssd = 30;

boolean newData = false;

//============

void setup() {
    Serial.begin(9600);
    Serial.println("This demo expects 3 pieces of data - text, an integer and a floating point value");
    Serial.println("Enter data in this style <HelloWorld, 12, 24.7>  ");
    Serial.println();
}

void draw(void) {
  cpu = "CPU: " + s3;
  ram = "RAM: " + s4;
  rx = "RX: " + s5;
  tx = "TX: " + s6;
  net = "NET: " + s7;
  cput = "CPU: " + String(i1) + "C";
  d1 = "SSD1: " + String(i2) + "C";
  d2 = "SSD2: " + String(i3) + "C";
  d3 = "SSD3: " + String(i4) + "C";
  d4 = "SSD4: " + String(i5) + "C";  
  u8g.setFont(u8g_font_5x8);
  u8g.setPrintPos(0, 8); 
  u8g.print(s1);
  u8g.setPrintPos(85, 8); 
  u8g.print(s2);
  u8g.setFont(u8g_font_5x8);
  u8g.setPrintPos(0, 20); 
  u8g.print(cpu);
  u8g.setPrintPos(0, 30); 
  u8g.print(ram);
  u8g.setPrintPos(0, 40); 
  u8g.print(rx);
  u8g.setPrintPos(0, 50); 
  u8g.print(tx);
  u8g.setPrintPos(0, 60); 
  u8g.print(net);
  
  u8g.setPrintPos(75, 20); 
  u8g.print(cput);
  u8g.setPrintPos(75, 30); 
  u8g.print(d1);
  u8g.setPrintPos(75, 40); 
  u8g.print(d2);
  u8g.setPrintPos(75, 50); 
  u8g.print(d3);
  u8g.setPrintPos(75, 60); 
  u8g.print(d4);
}

//============

void loop() {
    recvWithStartEndMarkers();
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        showParsedData();
        if(i2 > 0){
          m = m + i2;
          nbm ++;
        }
        if(i3 > 0){
          m = m + i3;
          nbm ++;
        }
        if(i4 > 0){
          m = m + i4;
          nbm ++;
        }
        if(i5 > 0){
          m = m + i5;
          nbm ++;
        }
        tssd = m/nbm;
        nbm = 0;
        m = 0;
        Serial.println(tssd);
        if(tssd > cssd){
          digitalWrite(13, HIGH);
        }
        else{
          digitalWrite(13, LOW);
        }
        newData = false;
    }
}

//============

void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    s1 = strtokIndx; // copy it to messageFromPC
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    s2 = strtokIndx; // copy it to messageFromPC
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    s3 = strtokIndx; // copy it to messageFromPC
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    s4 = strtokIndx; // copy it to messageFromPC
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    s5 = strtokIndx; // copy it to messageFromPC
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    s6 = strtokIndx; // copy it to messageFromPC
    strtokIndx = strtok(NULL,",");      // get the first part - the string
    s7 = strtokIndx; // copy it to messageFromPC
 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    i1 = atoi(strtokIndx);     // convert this part to an integer 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    i2 = atoi(strtokIndx);     // convert this part to an integer 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    i3 = atoi(strtokIndx);     // convert this part to an integer 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    i4 = atoi(strtokIndx);     // convert this part to an integer 
    strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
    i5 = atoi(strtokIndx);     // convert this part to an integer

}

//============

void showParsedData() {
    Serial.print("S1 ");
    Serial.println(s1);
    Serial.print("S2 ");
    Serial.println(s2);
    Serial.print("S3 ");
    Serial.println(s3);
    Serial.print("S4 ");
    Serial.println(s4);
    Serial.print("S5 ");
    Serial.println(s5);
    Serial.print("S6 ");
    Serial.println(s6);
    Serial.print("S7 ");
    Serial.println(s7);
    Serial.print("I1 ");
    Serial.println(i1);
    Serial.print("I2 ");
    Serial.println(i2);
    Serial.print("I3 ");
    Serial.println(i3);
    Serial.print("I4 ");
    Serial.println(i4);
    Serial.print("I5 ");
    Serial.println(i5);
u8g.firstPage();  
  do {
    draw();
  } while( u8g.nextPage() );
}
