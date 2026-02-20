#ifndef SCENE_H
#define SCENE_H
	
#include <string>

class Scene {

	public:
		//array of objects in the scene
		//systems in the scene


		Scene(std::string name);

		//Must be overriden
		void virtual update() = 0;
		void virtual start() = 0;
		void virtual end() = 0;

		std::string name;
};

#endif // !SCENE_H
