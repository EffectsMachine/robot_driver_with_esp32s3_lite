#include <lygion_devs.h>

NodeClass node;

void setup()
{
  Serial1.begin(1000000, SERIAL_8N1, 5, 4);//esp32-s3
  //Serial1.begin(1000000);//mega2560
  node.pSerial = &Serial1;
  delay(1000);
  node.unLockEprom(1);//打开EPROM保存功能
  node.writeByte(1, LY_NODE_ID, 2);//ID
  node.LockEprom(2);//关闭EPROM保存功能
}

void loop()
{

}
