#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include"BaseObject.h"
#include "CommonFunction.h"

const int EXPLOSION_WIDTH = 168;
const int EXPLOSION_HEIGHT = 145;

class Explosion : public BaseObject
{
public:
	Explosion();
	~Explosion();

	void set_clip();
	void set_frame(const int &fr){ frame_ = fr ;} 
	void ShowEx(SDL_Surface* des);
private:
	int frame_;

	SDL_Rect clip_[5];

};
#endif
