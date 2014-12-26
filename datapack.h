#ifndef _DATAPACK_H
#define _DATAPACK_H

#include "header.h"

class DataPack
{
public:
    DataPack(unsigned long length = 0);
    DataPack(const void *data, unsigned long length);

#ifdef _ARDUINO_H
	DataPack(Stream &stream);
#else
	DataPack(istream &stream);
#endif

	DataPack(const DataPack& dp);
	DataPack& operator=(const DataPack& dp);

	~DataPack();

	// return the length of the original object
	unsigned long InputLength() const;
	// return the address of the input object
	void *InputObject();
	// return the read-only address of the input object
	const void *InputObject() const;

	// return the length of the output object
	unsigned long OutputLength() const;
	// return the read-only address of the output object
	const void *OutputObject() const;

private:
	// address of output data
    char *_data;

	void Release();
};

#endif