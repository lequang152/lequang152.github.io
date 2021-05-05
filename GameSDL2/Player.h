#ifndef PLAYER_H
#define PLAYER_H
#include "BaseObject.h"
#include<vector>

class Player : public BaseObject
{
public:
	Player();
	~Player();
	void SetNumber(const int& num) { number_ = num; }
	void Position(const int& xpos);
	void Render(SDL_Surface* des);  
	void Init();
	void Decrease();

private:
	int number_;
	std::vector<int> pos_list_;
};
#endif