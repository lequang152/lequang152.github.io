#include "CommonFunction.h"
#include "Text.h"


bool SDL_CommonFunc::CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect)
{
	if (x >= rect.x && x <= rect.x + rect.w &&
		y >= rect.y && y <= rect.y + rect.h)
	{
		return true;
	}
	return false;
}



int SDL_CommonFunc::ShowMenu(SDL_Surface* des, TTF_Font* font)
{
	g_menu = LoadSurface("image//Game_Start.png");
	if (g_menu == NULL) 
	{
		return 1;
	}

	const int MenuItemNum = 2;
	SDL_Rect pos_arr[MenuItemNum];
	pos_arr[0].x = 920;
	pos_arr[0].y = 400;

	pos_arr[1].x = 920;
	pos_arr[1].y = 450;

	Text text_menu[MenuItemNum];

	text_menu[0].SetText("PLAY GAME");
	text_menu[0].SetColor(Text::RED_TEXT);
	text_menu[0].SetRect(pos_arr[0].x, pos_arr[0].y);

	text_menu[1].SetText("EXIT");
	text_menu[1].SetColor(Text::RED_TEXT);
	text_menu[1].SetRect(pos_arr[1].x, pos_arr[1].y);

	bool select[MenuItemNum] = { 0, 0 };
	int xm = 0;
	int ym = 0;

	SDL_Event m_event;
	
		while (true)
	{
		SDL_CommonFunc::ApplySurface(g_menu, des, 0, 0);
		for (int i = 0; i < MenuItemNum; i++)
		{
			text_menu[i].CreateGameText(font, des);
		}
			
		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
				return 1;
			case SDL_MOUSEMOTION:
			{
				xm = m_event.motion.x;
				ym = m_event.motion.y;

				for (int i = 0; i < MenuItemNum; i++)
				{
					if (CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
					{
						if (select[i] == false)
						{
							select[i] = 1;
							text_menu[i].SetColor(Text::BLACK_TEXT);
						}
					}

					else
					{
						if (select[i] == true)
						{
							select[i] = 0;
							text_menu[i].SetColor(Text::RED_TEXT);
						}
					}					}
				}
					break;
				case SDL_MOUSEBUTTONDOWN:
				{
					xm = m_event.button.x;
					ym = m_event.button.y;
					for (int i = 0; i < MenuItemNum; i++)
					{
						if (CheckFocusWithRect(xm, ym, text_menu[i].GetRect()))
						{
							return i;
						}
					}
				}
					break;
				case SDL_KEYDOWN:
					if (m_event.key.keysym.sym = SDLK_ESCAPE)
					{
						return 1;
					}
				default:
					break;
				}
			}
		SDL_Flip(des);
	}
	return 1;
}




SDL_Surface* SDL_CommonFunc::LoadSurface(std::string file_path)
{
	SDL_Surface* load_surface = NULL;
	SDL_Surface* optimize_image = NULL;

	load_surface = IMG_Load(file_path.c_str());
	if (load_surface != NULL){
		optimize_image = SDL_DisplayFormat(load_surface);
		SDL_FreeSurface(load_surface);

		if (optimize_image != NULL)
		{
			UINT32 color_key = SDL_MapRGB(optimize_image->format, 0, 0xFF, 0xFF);
			SDL_SetColorKey(optimize_image, SDL_SRCCOLORKEY, color_key);
		}
	}
	return optimize_image;
}


SDL_Rect SDL_CommonFunc::ApplySurface(SDL_Surface* src, SDL_Surface* des, int x, int y)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, NULL, des, &offset);
	return offset;
}


void SDL_CommonFunc::ApplySurfaceClip(SDL_Surface* src, SDL_Surface* des, SDL_Rect* clip, int x, int y)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(src, clip, des, &offset);
}


bool SDL_CommonFunc::CheckCollision(const SDL_Rect& object1, const SDL_Rect& object2)
{
	int left_a = object1.x;
	int right_a = object1.x + object1.w - 8;
	int top_a = object1.y;
	int bottom_a = object1.y + object1.h;

	int left_b = object2.x;
	int right_b = object2.x + object2.w;
	int top_b = object2.y;
	int bottom_b = object2.y + object2.h;

	// Case 1: size object 1 < size object 2
	if (left_a > left_b && left_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (left_a > left_b && left_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (top_a > top_b && top_a < bottom_b)
		{
			return true;
		}
	}

	if (right_a > left_b && right_a < right_b)
	{
		if (bottom_a > top_b && bottom_a < bottom_b)
		{
			return true;
		}
	}

	// Case 2: size object 1 < size object 2
	if (left_b > left_a && left_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (left_b > left_a && left_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (top_b > top_a && top_b < bottom_a)
		{
			return true;
		}
	}

	if (right_b > left_a && right_b < right_a)
	{
		if (bottom_b > top_a && bottom_b < bottom_a)
		{
			return true;
		}
	}

	// Case 3: size object 1 = size object 2
	if (top_a == top_b && right_a == right_b && bottom_a == bottom_b)
	{
		return true;
	}

	return false;
}

void SDL_CommonFunc::CleanUp()
{
	SDL_FreeSurface(g_screen);
	SDL_FreeSurface(g_bkground);
}