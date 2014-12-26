#include "datapack.h"

DataPack::DataPack(unsigned long length)
{
	this->_data = new char[length + 1];
	this->_data[0] = length;
}

DataPack::DataPack(const void *data, unsigned long length)
{
	this->_data = new char[length + 1];
	this->_data[0] = length;
	memcpy(this->_data + 1, data, length);
}

#ifdef _ARDUINO_H
DataPack::DataPack(Stream & stream)
{
	// the first byte is the length of the data
	int length = stream.read();
#else
DataPack::DataPack(istream & stream)
{
	// the first byte is the length of the data
	int length = stream.get();
#endif
	if (length < 0)
	{
		length = 0;
	}

	this->_data = new char[length + 1];
	this->_data[0] = length;
#ifdef _ARDUINO_H
	stream.readBytes(this->_data + 1, length);
#else
	stream.read(this->_data + 1, length);
#endif
}

DataPack::DataPack(const DataPack & dp)
{
	unsigned int length = dp.OutputLength();
	this->_data = new char[length];
	memcpy(this->_data, dp._data, length);
}

DataPack & DataPack::operator=(const DataPack & dp)
{
	if (&dp != this)
	{
		this->Release();
		unsigned int length = dp.OutputLength();
		this->_data = new char[length];
		memcpy(this->_data, dp._data, length);
	}
}

DataPack::~DataPack()
{
	this->Release();
}

unsigned long DataPack::InputLength() const
{
	// first cast to unsigned char to limit the final value between 0 ~ 255
	return (unsigned char)this->_data[0];
}

void * DataPack::InputObject()
{
	if (this->_data[0] > 0)
	{
		return this->_data + 1;
	}

	return 0;
}

const void * DataPack::InputObject() const
{
	if (this->_data[0] > 0)
	{
		return this->_data + 1;
	}

	return 0;
}

unsigned long DataPack::OutputLength() const
{
	return (unsigned char)this->_data[0] + 1;
}

const void * DataPack::OutputObject() const
{
	return this->_data;
}

void DataPack::Release()
{
	if (this->_data != 0)
	{
		delete[] this->_data;
		this->_data = 0;
	}
}
