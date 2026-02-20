#ifndef COMPONENT_H
#define COMPONENT_H
class Component
{
public:
	virtual ~Component() {};
	void virtual start() = 0;
	void virtual update() = 0;
	void virtual end() = 0;
};

#endif // !COMPONENT_H

