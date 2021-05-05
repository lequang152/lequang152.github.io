#include "Player.h"

Player::Player()
{

}

Player::~Player()
{

}

void Player::Position(const int& pos)
{
	pos_list_.push_back(pos);
}

void Player::Render(SDL_Surface* des)
{
	if (number_ == pos_list_.size())
	{
		for (int i = 0; i < pos_list_.size(); i++)
		{
			rect_.x = pos_list_[i];
			Show(des);
		}
	}
}

void Player::Init()
{
	LoadSur("image//heart.png");
	number_ = 3;
	if (pos_list_.size() > 0)
	{
		pos_list_.clear();
	}

	Position(20);
	Position(60);
	Position(100);
}

void Player::Decrease()
{
	number_--;
	pos_list_.pop_back();
}