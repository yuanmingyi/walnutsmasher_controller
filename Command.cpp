#include "Command.h"

struct CommandData
{
	unsigned char _cmdCode;
	unsigned char _cmdReserved;
	unsigned char _param1;
	unsigned char _param2;
	CommandData(unsigned char cmdCode, unsigned char cmdReserved, unsigned char param1, unsigned char param2)
		: _cmdCode(cmdCode), _cmdReserved(cmdReserved), _param1(param1), _param2(param2)
	{
	}
};

DataPack Command::Success(const char * message)
{
	if (message == 0)
	{
		message = ErrorMessage(RET_SUCCESS);
	}

	return ToDataPack(RET_SUCCESS, TYPE_STRING, message, strlen(message) + 1);
}

DataPack Command::Success(unsigned int typeId, const void * obj)
{
	// use Success(const void* obj, unsigned int length) to compose a custom type success datapack
	if (typeId == TYPE_STRING)
	{
		return Success((const char*)obj);
	}
	else
	{
		return ToDataPack(RET_SUCCESS, typeId, obj, 0);
	}
}

DataPack Command::Success(const void* obj, unsigned int length)
{
	// don't use it to transfer nested pointers, like there is pointer inside the object. this function will not deep copy those pointers
	return ToDataPack(RET_SUCCESS, TYPE_CUSTOM, obj, length);
}

DataPack Command::Fail(unsigned int errorcode, const char * message)
{
	if (message == 0)
	{
		message = ErrorMessage(errorcode);
	}

	return ToDataPack(errorcode, TYPE_STRING, message, strlen(message) + 1);
}

DataPack Command::ToDataPack(unsigned int cmdCode, unsigned int param1, unsigned int param2)
{
	CommandData data(cmdCode, 0, param1, param2);
	return DataPack(&data, sizeof(data));
}

const ResultData * Command::GetResult(const DataPack * data)
{
	return (const ResultData*)data->InputObject();
}

unsigned int Command::GetExtraDataLength(const DataPack * data)
{
	return data->InputLength() - sizeof(ResultData);
}

const char * Command::ErrorMessage(unsigned int errorCode)
{
	const char *msg = 0;
	switch (errorCode)
	{
	case RET_SUCCESS:
		msg = "No error";
		break;

	case RET_INVALIDCODE:
		msg = "Invalid command code";
		break;

	case RET_INVALIDDATA:
		msg = "Invalid command data";
		break;

	case RET_INVALIDPARAM:
		msg = "Invalid command parameters";
		break;

	case RET_UNKNOWNCODE:
		msg = "Unknown command group";
		break;

	default:
		msg = "Unknown error code";
		break;
	}

	return msg;
}

Command::Command()
{
	for (int i = 0; i < Command::_maxExecutors; i++)
	{
		this->_commandExecutors[i] = 0;
	}
}

bool Command::RegisterExecutor(ICommandExecutor * commandExecutor, unsigned int cmdCodeStart)
{
	unsigned int index = (cmdCodeStart >> 4);
	if (index < Command::_maxExecutors)
	{
		if (_commandExecutors[index] == 0)
		{
			_commandExecutors[index] = commandExecutor;
			return true;
		}
	}

	return false;
}

DataPack Command::BeginCommand(const DataPack *data) const
{
	// Assert.IsNotNull(data); Assert.AreEqual(4, data->Length);
	if (data->InputLength() != sizeof(CommandData))
	{
		return Command::Fail(RET_INVALIDDATA);
	}

	const CommandData *cmdData = (const CommandData*)(data->InputObject());

	unsigned int cmdCode = cmdData->_cmdCode;
	unsigned int cmdReserved = cmdData->_cmdReserved;
	unsigned int param1 = cmdData->_param1;
	unsigned int param2 = cmdData->_param2;
	unsigned int index = cmdCode >> 4;

	if (index >= this->_maxExecutors || this->_commandExecutors[index] == 0)
	{
		return Command::Fail(RET_UNKNOWNCODE);
	}

	return this->_commandExecutors[index]->Action(cmdCode, cmdReserved, param1, param2);
}

DataPack Command::ToDataPack(unsigned char retCode, unsigned char typeId, const void * obj, unsigned int extraLength)
{
	if (typeId != TYPE_STRING || typeId != TYPE_CUSTOM)
	{
		extraLength = 0;
	}

	DataPack dp(sizeof(ResultData) + extraLength);
	ResultData *p = (ResultData*)dp.InputObject();
	char *custom = (char*)dp.InputObject() + sizeof(ResultData);
	p->_retCode = retCode;
	p->_typeId = typeId;

	if (extraLength > 0)
	{
		p->_retData._string = custom;
		memcpy(custom, obj, extraLength);
	}
	else
	{
		p->_retData._double = *(const double*)obj;
	}

	return dp;
}
