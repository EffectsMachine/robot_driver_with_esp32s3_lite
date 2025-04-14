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
  Serial.println("m1~m4 sync speed 1000");
  gqdmd.SpeedCtl(1, 1000, 500, 600, 0, 0x10);//bit4=1 sycn;bit0=0 no ack
  gqdmd.SpeedCtl(2, 1000, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(3, 1000, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(4, 1000, 500, 600, 0, 0x10);
  gqdmd.StatusCtl(0, 2048);
  delay(6000);

  Serial.println("m1~m4 sync speed 0");
  gqdmd.SpeedCtl(1, 0, 500, 600, 0, 0x10);//bit4=1 sycn;bit0=0 no ack
  gqdmd.SpeedCtl(2, 0, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(3, 0, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(4, 0, 500, 600, 0, 0x10);
  gqdmd.StatusCtl(0, 2048);
  delay(2000);

  Serial.println("m1~m4 sync speed -1000");
  gqdmd.SpeedCtl(1, -1000, 500, 600, 0, 0x10);//bit4=1 sycn;bit0=0 no ack
  gqdmd.SpeedCtl(2, -1000, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(3, -1000, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(4, -1000, 500, 600, 0, 0x10);
  gqdmd.StatusCtl(0, 2048);
  delay(6000);

  Serial.println("m1~m4 sync speed 0");
  gqdmd.SpeedCtl(1, 0, 500, 600, 0, 0x10);//bit4=1 sycn;bit0=0 no ack
  gqdmd.SpeedCtl(2, 0, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(3, 0, 500, 600, 0, 0x10);
  gqdmd.SpeedCtl(4, 0, 500, 600, 0, 0x10);
  gqdmd.StatusCtl(0, 2048);
  delay(2000);
}
