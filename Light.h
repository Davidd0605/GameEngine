#ifndef LIGHT_H
#define LIGHT_H

#include "src/GameEngineCore/Components/Component.h"
#include "LightTypes.h"
#include <glm/glm.hpp>
class Light : public Component
{
	private: 
		LightType lightType;
		glm::vec3 lightColor;
		float intensity;

	public:

		void start() override;
		void update() override;
		void fixedUpdate() override;
		void end() override;

		glm::vec3 getColor();
		float getIntensity();
		LightType getType();

		Light(LightType lightType, int intensity = 1, glm::vec3 lgihtColor = glm::vec3(1));
	
};

#endif

