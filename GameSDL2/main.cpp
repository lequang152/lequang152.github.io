#include "CommonFunction.h"
#include "MainObject.h"
#include "Threat.h"
#include "Explosion.h"
#include "Text.h"
#include "Player.h"

TTF_Font* g_font_text = NULL;
TTF_Font* g_font_menu = NULL;	

bool Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	g_screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);
	if (g_screen == NULL)
	{
		return false;
	}

	//AudioforGame
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		return false;

	g_sound_bullet[0] = Mix_LoadWAV("sound//Gun+Silencer.wav");
	g_sound_bullet[1] = Mix_LoadWAV("sound//Fireball+3.wav");
	g_sound_exp[0] = Mix_LoadWAV("sound//Explosion+1.wav");
	g_sound_exp[1] = Mix_LoadWAV("sound//Explosion+3.wav");

	if (g_sound_exp[0] == NULL || g_sound_exp[1] == NULL || g_sound_bullet[0] == NULL || g_sound_bullet[1] == NULL)
	{
		return false;
	}

	//TrueTypeFont
	if (TTF_Init() == -1)
	{
		return false;
	}

	g_font_text = TTF_OpenFont("font//1111.ttf", 20);
	g_font_menu = TTF_OpenFont("font//1111.ttf", 37);
	if (g_font_text == NULL || g_font_menu == NULL)
	{
		return false;
	}


	return true;
}


int main(int arc, char* argv[])
{
	int bkg_x = 0;

	bool is_quit = false;
	if (Init() == false)
		return 0;

	g_bkground = SDL_CommonFunc::LoadSurface("image//g_bkg.png");
	if (g_bkground == NULL)
	{
		return 0;
	}

	Text mark_game;
	mark_game.SetColor(Text::BLACK_TEXT);

	//Player
	Player player;
	player.Init();


	//MainObject
	MainObject main_object;
	main_object.SetRect(100, 200);
	bool ret = main_object.LoadSur("image//plane3yellow.png");
	if (!ret)
	{
		return 0;
	}

	//Explosion
	Explosion exp_main;
	Explosion exp_threat;

	ret = exp_threat.LoadSur("image//Untitled.png");
	exp_threat.set_clip();
	if (ret == false)
	{
		return 0;
	}

	ret = exp_main.LoadSur("image//Untitled.png");
	exp_main.set_clip();
	if (ret == false)
	{
		return 0;
	}



	//ThreatsObject
	ThreatObject* p_threats = new ThreatObject[NUM_THREATS];
	for (int t = 0; t < NUM_THREATS; t++)
	{
		ThreatObject* p_threat = (p_threats + t);	
		if (p_threat)
		{
			ret = p_threat->LoadSur("image//plane2red.png");
			if (ret == false)
			{
				return 0;
			}

			int rand_y = rand() % 400;
			if (rand_y > SCREEN_HEIGHT - 200)
			{
				rand_y = SCREEN_HEIGHT*0.3;
			}


			p_threat->SetRect(SCREEN_WIDTH + t * 400, rand_y);
			p_threat->set_x_val(3);

			AmoObject* p_amo = new AmoObject();
			p_threat->InitAmo(p_amo);
		}
	}

	unsigned int mark_value = 0;
	unsigned int die_number = 0;

	//ShowMenu
	int ret_menu = SDL_CommonFunc::ShowMenu(g_screen, g_font_menu);
	if (ret_menu == 1)
	{
		is_quit = true;
	}


	while (!is_quit)
	{
		while (SDL_PollEvent(&g_even))
		{
			if (g_even.type == SDL_QUIT)
			{
				is_quit = true;
				break;
			}
			main_object.HandleInputAction(g_even, g_sound_bullet);
		}
		
		//BackgroundMove
		bkg_x -= 2;
		SDL_CommonFunc::ApplySurface(g_bkground, g_screen, bkg_x, 0);
		SDL_CommonFunc::ApplySurface(g_bkground, g_screen, bkg_x + SCREEN_WIDTH, 0);
		if (bkg_x <= -SCREEN_WIDTH)
		{
			bkg_x = 0;
		}

		//ShowPlayer
		player.Render(g_screen);

		main_object.HandleMove();
		main_object.Show(g_screen);
		main_object.MakeAmo(g_screen);
		

		for (int m = 0; m < NUM_THREATS; m++)
		{
			ThreatObject* p_threat = (p_threats + m);
			if (p_threat)
			{
				p_threat->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);
				p_threat->MakeAmo(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				
				//ThreatOutScene


				//Ex main -> threat
				bool is_col = SDL_CommonFunc::CheckCollision(main_object.GetRect(), p_threat->GetRect());
				if (is_col)
	 			{
					for (int ex = 0; ex < 5; ex++)
					{
						int x_pos = main_object.GetRect().x + main_object.GetRect().w*0.5 - EXPLOSION_WIDTH*0.5;
						int y_pos = main_object.GetRect().y + main_object.GetRect().h*0.5 - EXPLOSION_HEIGHT*0.5;

						exp_main.set_frame(ex);
						exp_main.SetRect(x_pos, y_pos);
						exp_main.ShowEx(g_screen);
						
						SDL_Delay(100);
						
						if (SDL_Flip(g_screen) == -1) return 0;
					}

					Mix_PlayChannel(-1, g_sound_exp[1], 0);
					
					die_number++;

					if (die_number <= 2)
					{
						SDL_Delay(1000);
						main_object.SetRect(100, 200);
						player.Decrease();
						player.Render(g_screen);
						if (SDL_Flip(g_screen) == -1)
						{
							delete[] p_threats;
							SDL_CommonFunc::CleanUp();
							SDL_Quit();
							return 0;
						}
					}

					else
					{
						if (MessageBox(NULL, "GAME OVER!", "Info", MB_OK) == IDOK)
						{
							delete[] p_threats;
							SDL_CommonFunc::CleanUp();
							SDL_Quit();
							return 0;
						}
					}

					
				}

				//Ex amo -> threat
				std::vector<AmoObject*> amo_list = main_object.GetAmoList();
				for (int am = 0; am < amo_list.size(); am++)
				{
					AmoObject* p_amo = amo_list[am];
					if (p_amo != NULL)
					{
						bool ret_col = SDL_CommonFunc::CheckCollision(p_amo->GetRect(), p_threat->GetRect());
						if (ret_col)
						{
							mark_value++;
							p_threat->Reset(SCREEN_WIDTH + m * 400);
							main_object.RemoveAmo(am);
							Mix_PlayChannel(-1, g_sound_exp[0], 0);
						}
					}
				}

				//Ex amo -> main
				std::vector<AmoObject*> amo_list_ = p_threat->GetAmoList();
				for (int am = 0; am < amo_list_.size(); am++)
				{
					AmoObject* p_amo = amo_list_[am];
					if (p_amo != NULL)
					{
						bool ret_col1 = SDL_CommonFunc::CheckCollision(p_amo->GetRect(), main_object.GetRect());
						if (ret_col1)
						{
							for (int ex1 = 0; ex1 < 5; ex1++)
							{
								int x_pos = main_object.GetRect().x + main_object.GetRect().w*0.5 - EXPLOSION_WIDTH*0.5;
								int y_pos = main_object.GetRect().y + main_object.GetRect().h*0.5 - EXPLOSION_HEIGHT*0.5;

								exp_main.set_frame(ex1);
								exp_main.SetRect(x_pos, y_pos);
								exp_main.ShowEx(g_screen);

								SDL_Delay(50);

								if (SDL_Flip(g_screen) == -1) return 0;
							}

							Mix_PlayChannel(-1, g_sound_exp[1], 0);
							die_number++;
							
							if (die_number <= 2)
							{
								SDL_Delay(1000);
								main_object.SetRect(100, 200);
								player.Decrease();
								player.Render(g_screen);
								if (SDL_Flip(g_screen) == -1)
								{
									delete[] p_threats;
									SDL_CommonFunc::CleanUp();
									SDL_Quit();
									return 0;
								}
							}
							
							else
							{
								if (MessageBox(NULL, "GAME OVER!", "Info", MB_OK) == IDOK)
								{
									delete[] p_threats;
									SDL_CommonFunc::CleanUp();
									SDL_Quit();
									return 0;
								}
							}
						}

					}
				}

			}	
		}
		
		//ShowMark
		std::string val_str_mark = std::to_string(mark_value);
		std::string strMark("Scored: ");
		strMark += val_str_mark;

		mark_game.SetText(strMark);
		mark_game.SetRect(520, 10);
		mark_game.CreateGameText(g_font_text, g_screen);
		
		if (SDL_Flip(g_screen) == -1)
			return 0;

	}

	delete[] p_threats;

	SDL_CommonFunc::CleanUp();
	SDL_Quit();
	return 1;
}