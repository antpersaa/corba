#ifndef navigation_h
#define navigation_h

#include "WProgram.h"
#include "config.h"
#include "engine.h"
#include "PololuQTRSensors.h"

#define LINE_LOST -1
#define EDGE_LEFT -2
#define EDGE_RIGHT -3

void calibrate(PololuQTRSensorsRC* qtr){
  motor(MotorA,150);
  motor(MotorB,-150);

  int i;
  for (i = 0; i < 125; i++){
    qtr->calibrate(QTR_EMITTERS_ON);
    delay(20);
  }

  motor(MotorA,0);
  motor(MotorB,0);
}

bool lineLost(unsigned int* vals){
  bool lost = true;
  
  if (TRACKING_WHITE == 0){
    for(int i=0;i<NUM_OF_SENSORS - 1;i++){
      if (vals[i] > WHITE_TRASHOLD){
	lost =false;
      }
    }
  }else{
    for(int i=0;i<NUM_OF_SENSORS - 1;i++){
      if (vals[i] < BLACK_TRASHOLD){
	lost =false;
      }
    }
  }
  return lost;
}

int checkEdge(unsigned int* vals){ 
  if (TRACKING_WHITE == 0){
    if ((vals[NUM_OF_SENSORS - 1] > WHITE_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 2] > WHITE_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 3] > WHITE_TRASHOLD) &&
	(vals[0] < WHITE_TRASHOLD) &&
	(vals[1] < WHITE_TRASHOLD) &&
	(vals[2] < WHITE_TRASHOLD)){
      return EDGE_LEFT;
    }
    if ((vals[0] > WHITE_TRASHOLD) &&
	(vals[1] > WHITE_TRASHOLD) &&
	(vals[2] > WHITE_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 1] < WHITE_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 2] < WHITE_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 3] < WHITE_TRASHOLD)){
      return EDGE_RIGHT;
    }
  }else{
    if ((vals[NUM_OF_SENSORS - 1] < BLACK_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 2] < BLACK_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 3] < BLACK_TRASHOLD) &&
	(vals[0] > BLACK_TRASHOLD) &&
	(vals[1] > BLACK_TRASHOLD) &&
	(vals[2] > BLACK_TRASHOLD)){
      return EDGE_LEFT;
    }
    if ((vals[0] < BLACK_TRASHOLD) &&
	(vals[1] < BLACK_TRASHOLD) &&
	(vals[2] < BLACK_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 1] > BLACK_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 2] > BLACK_TRASHOLD) &&
	(vals[NUM_OF_SENSORS - 3] > BLACK_TRASHOLD)){
      return EDGE_RIGHT;
    }
  }
  return 0;
}

int readLine(PololuQTRSensorsRC* qtr){
  unsigned int val[NUM_OF_SENSORS];
  qtr->readCalibrated(val);
  int line = qtr->readLine(val,QTR_EMITTERS_ON,TRACKING_WHITE);

#ifdef DEBUG
  Serial.print(line);
  Serial.print(" R< ");
  Serial.print(val[0]); Serial.print(" ");
  Serial.print(val[1]); Serial.print(" ");
  Serial.print(val[2]); Serial.print(" ");
  Serial.print(val[3]); Serial.print(" ");
  Serial.print(val[4]); Serial.print(" ");
  Serial.print(val[5]); Serial.print(" ");
  Serial.print(val[6]); Serial.print(" ");
  Serial.print(val[7]); Serial.print(" >L ");

  if (lineLost(val) == true)
    Serial.print("\n");
#endif

  if (lineLost(val) == true)
    return LINE_LOST;
  else if (checkEdge(val) != 0)
    return checkEdge(val);
  else
    return line;
}

#endif
