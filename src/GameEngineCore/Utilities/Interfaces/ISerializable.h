#ifndef ISERIALIZABLE_H
#define ISERIALIZABLE_H

#include <string>
#include <json/json.h>

class ISerializable
{
public:
	virtual std::string serialize() = 0;

private:
	std::string jsonData; // Placeholder for serialized data, can be replaced with actual JSON library or format
};


#endif // ISERIALIZABLE_H