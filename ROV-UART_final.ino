// 2019.8.5 成功
// 手柄翻转键控制：翻转、纵推、前进
#include<math.h>
#include<Servo.h>
Servo motorzm;//纵向中间电机
Servo motorzl;//纵向左电机
Servo motorzr;//纵向右电机
Servo motorhl;//横向左电机
Servo motorhr;//横向右电机
Servo servoCAM;//摄像头云台舵机
#define PINmotor1 9   //纵向
#define PINmotor2 10
#define PINmotor3 11
#define PINmotor4 5  //横向
#define PINmotor5 6
#define Median 90
#define PINservo 3 //摄像头云台舵机
#define PinLight 12 //灯

String data = "";//String data = "\0\0\0\0\0";//
int i = 0;
int ot = 0;
byte num[3] = { 0,0,0 };
int x = 0; int y = 0; int z = 0;
int l = 0; int r = 0;
int rou = 0;//手柄摇杆转动时到中心的距离

void setup()
{
  motorzm.attach(PINmotor1);
  motorzl.attach(PINmotor2);
  motorzr.attach(PINmotor3);
  motorhl.attach(PINmotor4);
  motorhr.attach(PINmotor5);
  servoCAM.attach(PINservo);

  motorzm.write(Median); // 启动信号：中位
  motorzl.write(Median);
  motorzr.write(Median);
  motorhl.write(Median);
  motorhr.write(Median);
  delay(100);
  servoCAM.write(80); delay(200); servoCAM.write(90); delay(200); servoCAM.write(75);//舵机测试
  
  Serial.begin(57600);//Serial.begin(57600, SERIAL_8N1); //数据位8，无奇偶校验，停止位1
  pinMode(PinLight , OUTPUT); digitalWrite(PinLight , 1); delay(500); digitalWrite(PinLight , 0); //引脚初始化；灯测试
  delay(100);
}

void loop()
{
  i = 0;
  //————接收定长数据包（用字符串来存）————
  if (Serial.available() > 0)
  {
    while (i++ < 5)
    {
      data += char(Serial.read());
      delayMicroseconds(240);//delay(0.1); //必须延时，否则程序运行速率超过串口发送速率，就会接收不全
    }
    num[0] = data[2]; num[1] = data[3]; num[2] = data[4];

    switch ( data[0] )
    {
      case 'o': ot = (int)num[0]-128;
        if(ot!=0) { motorzm.write(90); motorzr.write(90-ot/2); motorzl.write(90+ot/2); }
        //Serial.print("ot"); Serial.println(ot); // Serial.print(num[1]); Serial.println(num[2]);//
        break;
      case 'm': if(ot == 0) { x = (int)num[0] - 128; y = (int)num[1] - 128; z = (int)num[2] - 128;
			rou = (int)sqrt( x*x + y*y ); rou = (rou>100)? 100 : rou;
		  motorzm.write(90+z/2); motorzl.write(90-z/2); motorzr.write(90-z/2); //motorhl.write(90+y/2); motorhr.write(90+y/2);
          if(x<=0&&y>=0)      { r = 90 - rou/2; l = 90 - rou/2 + x; }
          else if(x<=0&&y<=0) { l = 90 + rou/2; r = 90 + rou/2 - x; }
          else if(x>=0&&y>=0) { l = 90 - rou/2; r = 90 - rou/2 - x; }
          else                { r = 90 + rou/2; l = 90 + rou/2 + x; }
          motorhl.write(l);  motorhr.write(r);//（左旋、右旋）不能满油门
        }
        //Serial.print(rou); Serial.print(" "); Serial.print(l); Serial.print(" "); Serial.println(r); //Serial.print("m "); Serial.print(x); Serial.print(y); Serial.println(z);
        break;
      case 'l': digitalWrite(PinLight , num[0]);
        //Serial.print("li:"); Serial.println(num[0]);
        break;
      case 's':
        if(num[0] == 0) { motorhl.write(Median); motorhr.write(Median);
          motorzm.write(Median); motorzl.write(Median); motorzr.write(Median); }
        else if(num[0] == 1){;}
        else {;}
        //Serial.print("st:"); Serial.println(num[0]);
        break;
      case 'c': servoCAM.write( num[0] );
        //Serial.print("ca:"); Serial.println(num[0]);
        break;
      default: break;
    }
	data = "";
  }
}
