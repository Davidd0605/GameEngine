#ifndef SCENE_H
#define SCENE_H
	

class Scene {

	public:
		//array of objects in the scene
		//systems in the scene
		void virtual update() = 0;
		void virtual start() = 0;
		void virtual end() = 0;
};

#endif // !SCENE_H
