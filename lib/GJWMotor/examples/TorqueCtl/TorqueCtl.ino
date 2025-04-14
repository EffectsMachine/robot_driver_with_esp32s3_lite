#include <GJWMotor.h>

GQDMD gqdmd;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(1000000, SERIAL_8N1, 5, 4);
  gqdmd.begin(&Serial1);// baud-rate at 1000000
  gqdmd.setTxEnd_T32(1000000);// baud-rate at 1000000
  gqdmd.setTimeOut(100); // if there is no answer in 100 ms, roll over
  delay(1000);
}

void loop()
{
  gqdmd.TorqueCtl(1, 100, 0);
  if(!gqdmd.getLastError()){
    Serial.println(gqdmd.motorTurn);
    Serial.println(gqdmd.motorAngle);
    Serial.println(gqdmd.motorSpeed);
    Serial.println(gqdmd.motorTorque);
    Serial.println(gqdmd.motorStatus);
  }
  delay(6000);

  gqdmd.TorqueCtl(1, 0, 0);
  if(!gqdmd.getLastError()){
    Serial.println(gqdmd.motorTurn);
    Serial.println(gqdmd.motorAngle);
    Serial.println(gqdmd.motorSpeed);
    Serial.println(gqdmd.motorTorque);
    Serial.println(gqdmd.motorStatus);
  }
  delay(2000);

  gqdmd.TorqueCtl(1, -100, 0);
  if(!gqdmd.getLastError()){
    Serial.println(gqdmd.motorTurn);
    Serial.println(gqdmd.motorAngle);
    Serial.println(gqdmd.motorSpeed);
    Serial.println(gqdmd.motorTorque);
    Serial.println(gqdmd.motorStatus);
  }
  delay(6000);

  gqdmd.TorqueCtl(1, 0, 0);
  if(!gqdmd.getLastError()){
    Serial.println(gqdmd.motorTurn);
    Serial.println(gqdmd.motorAngle);
    Serial.println(gqdmd.motorSpeed);
    Serial.println(gqdmd.motorTorque);
    Serial.println(gqdmd.motorStatus);
  }
  delay(2000);
}
