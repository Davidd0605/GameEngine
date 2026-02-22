#ifndef SCENE_H
#define SCENE_H
	
#include <string>

/// <summary>
/// Abstract class for the scene.
/// </summary>
class Scene {

	public:
		Scene(std::string name);

		//Must be overriden
		void virtual update() = 0;
		void virtual start() = 0;
		void virtual end() = 0;

		std::string name;
};

#endif // !SCENE_H
