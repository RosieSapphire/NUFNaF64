#ifndef _ENGINE_SCENE_H_
#define _ENGINE_SCENE_H_

enum
{
	SCENE_NONE = -1,
	SCENE_TITLE,
	SCENE_GAME,
	SCENE_DEATH,
	SCENE_CNT,
};

extern volatile int scene_index;

#endif /* _ENGINE_SCENE_H_ */
