#ifndef _COMMAND_H
#define _COMMAND_H

#include "datapack.h"

// command definitions
// control the motor
#define CMD_CAR				0x10
#define CMD_CAR_MOVE		0x11
#define CMD_CAR_STOP		0x12
#define CMD_CAR_SWING		0x13
#define CMD_CAR_TURNDIR		0x15
#define CMD_CAR_SPEEDUP		0x16
#define CMD_CAR_SLOWDOWN	0x17
#define CMD_CAR_STEP		0x18
#define CMD_MOTOR1_SET		0x19
#define CMD_MOTOR2_SET		0x1A

// control the speeding sensor
#define CMD_SPEED			0x20
#define CMD_SPEED_GET		0x21

// control the servo
#define CMD_SERVO			0x30
#define CMD_SERVO_OPEN		0x31
#define CMD_SERVO_CLOSE		0x32
#define CMD_SERVO_LIFT		0x33
#define CMD_SERVO_LAND		0x34

// control hc-sr04
#define CMD_HCSR04			0x40
#define CMD_HCSR04_START	0x41
#define CMD_HCSR04_END		0x42
#define CMD_HCSR04_DIST		0x43
#define CMD_HCSR04_INTERVAL	0x44

// control pins directly
#define CMD_PIN				0xE0
#define CMD_PIN_MODE		0xE1
#define CMD_PIN_DIGITREAD	0xE2
#define CMD_PIN_DIGITWRITE	0xE3
#define CMD_PIN_ANALOGREAD	0xE4
#define CMD_PIN_ANALOGWRITE	0xE5
#define CMD_PIN_PULSEIN		0xE6

// others
#define CMD_GENERAL			0x00
#define CMD_TEST			0x01
#define CMD_ERRORCODE		0x0F
#define CMD_CLEARERROR		0x02
#define CMD_ECHO			0x03

// return code
#define RET_SUCCESS			0x00
#define RET_UNKNOWNCODE		0x01
#define RET_INVALIDDATA		0x02
#define RET_INVALIDCODE		0x03
#define RET_INVALIDPARAM	0x04

// interface for module
class ICommandExecutor
{
public:
	virtual DataPack Action(unsigned int cmdCode, unsigned int cmdReserved, unsigned int param1, unsigned int param2) = 0;
};


#define TYPE_INT	0x00
#define TYPE_UINT	0x01
#define TYPE_FLOAT	0x02
#define TYPE_DOUBLE	0x03
#define TYPE_CHAR	0x04
#define TYPE_UCHAR	0x05
#define TYPE_STRING	0x06
#define TYPE_CUSTOM	0xFF

struct ResultData
{
	unsigned char _retCode;
	unsigned char _typeId;
	union
	{
		int _int;
		unsigned int _uint;
		float _float;
		double _double;
		char _char;
		unsigned char _uchar;
		const char* _string;
		const void* _custom;
	} _retData;

	ResultData(unsigned char retCode, unsigned char typeId)
		: _retCode(retCode), _typeId(typeId)
	{
		_retData._int = 0;
	}
};

class Command
{
private:
	ICommandExecutor *_commandExecutors[15];
	static const int _maxExecutors = 15;

public:
	// pack a string in a success datapack
	static DataPack Success(const char* message = 0);

	// pack a basic type value (int, char, double etc...) in a success datapack
	static DataPack Success(unsigned int typeId, const void* obj);

	// pack a custom type object in a success datapack
	// don't use it to transfer nested pointers, like there is pointer inside the object. this function will not deep copy those pointers
	static DataPack Success(const void* obj, unsigned int length);

	// pack a fail datapack with error code and error message
	static DataPack Fail(unsigned int errorcode = 1, const char* message = 0);
	
	// pack a command datapack
	static DataPack ToDataPack(unsigned int cmdCode, unsigned int param1, unsigned int param2);

	// get result data from the datapack
	static const ResultData* GetResult(const DataPack *data);

	static unsigned int GetExtraDataLength(const DataPack *data);

	static const char* ErrorMessage(unsigned int errorCode);

	Command();

	// register a command executor to the dictionary, with required first cmd code (cmdCodeStart). The start code must be multiple of 16 in dec
	bool RegisterExecutor(ICommandExecutor *commandExecutor, unsigned int cmdCodeStart);
	DataPack BeginCommand(const DataPack *data) const;
	
private:
	static DataPack ToDataPack(unsigned char retCode, unsigned char typeId, const void* obj, unsigned int extraLength);
};
#endif
