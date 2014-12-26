#include <App.h>
#include <Command.h>
#include <datapack.h>
#include <hc-sr04.h>
#include <L298.h>
#include <Speed.h>

L298 car(5, 6, 3, 4, 10, 11);
Speed spd(8, 7);
HcSr04 hcsr04(12, 2);

Command command;

void setup()
{  
  Serial.begin(9600);
  car.Setup();
  spd.Setup();
  hcsr04.Setup();
  hcsr04.SetMonitorCallback(returnDistance);
  App.SerialEvent = serialInput;
  command.RegisterExecutor(&car, CMD_CAR);
  command.RegisterExecutor(&spd, CMD_SPEED);
  command.RegisterExecutor(&hcsr04, CMD_HCSR04);
}

void loop()
{
  App.Run();
}

void serialEvent()
{
  while (Serial.available())
  {
    DataPack dp(Serial);
    DataPack ret = command.BeginCommand(&dp);
    const ResultData* result = Command::GetResult(&ret);
    printResult(result);
  }
}

void printResult(const ResultData* data)
{
  switch (data->_typeId)
  {
    case TYPE_INT:
    Serial.println(data->_retData._int);
    break;
    case TYPE_UINT:
    Serial.println(data->_retData._uint);
    break;
    case TYPE_FLOAT:
    Serial.println(data->_retData._float);
    break;
    case TYPE_DOUBLE:
    Serial.println(data->_retData._double);
    break;
    case TYPE_CHAR:
    Serial.println(data->_retData._char);
    break;
    case TYPE_UCHAR:
    Serial.println(data->_retData._uchar);
    break;
    case TYPE_STRING:
    Serial.println(data->_retData._string);
    break;
    case TYPE_CUSTOM:
    Serial.println("Custom data");
    default:
    Serial.println("Unknown data type");
    break;
  }    
}

void returnDistance(HcSr04 *obj)
{
  double dist = obj->Distance(HcSr04::Centimeter);
  Serial.print(dist);
  Serial.println("CM");
}
