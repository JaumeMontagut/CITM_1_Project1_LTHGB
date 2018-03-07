#include "SDL\include\SDL.h"
#include "SDL\include\SDL_image.h"
#include "SDL\include\SDL_mixer.h"
#include <time.h>//To calculate the random positions

#pragma comment (lib,"SDL/x86/SDL2.lib")
#pragma comment (lib,"SDL/x86/SDL2main.lib")
#pragma comment (lib,"SDL/x86/SDL2_image.lib")
#pragma comment (lib, "SDL/x86/SDL2_mixer.lib")

#define screenWitdh 1280
#define screenHeight 720
#define pushDist 10
#define pushDistMultiplier 5
//All image sizes are ajustaded automatically except for the background image

int main(int argc, char* argv[])
{
	//Declaration
	bool playing;
	bool pressingUp, pressingLeft, pressingDown, pressingRight, pressingW, pressingA, pressingS, pressingD;
	bool pushP2 = false, pushP1 = false;
	bool timeFinished = false;
	bool p1IsIn = false, p2IsIn = false;
	int characterWitdh = 0, characterHeight = 0;
	int characterSpeed;
	int pushR1Pos = 0;//1 = up, 2 = left, 3 = down, 4 = right
	int pushR2Pos = 0;
	int pushR1Counter = 0, pushR2Counter = 0;
	SDL_Window * window = nullptr;
	SDL_Renderer* renderer = nullptr;;
	SDL_Event event;
	SDL_Texture * p1Tx = nullptr;
	SDL_Texture * p2Tx = nullptr;
	SDL_Texture * backgroundTx = nullptr;
	SDL_Texture * winTx = nullptr;
	SDL_Texture * loseTx = nullptr;
	SDL_Rect p1Rect;
	SDL_Rect p2Rect;
	SDL_Rect pushR1;
	SDL_Rect pushR2;
	SDL_Rect p1Screen;
	SDL_Rect p2Screen;
	Mix_Music * music = nullptr;
	SDL_Rect objectiveR;

	//Inicialization (inicialitzacio bona (valors que farem servir), no aquella que ha de servir de valor buit per a les variables que encara no hem calculat)
	playing = true;
	pressingUp = false;
	pressingLeft = false;
	pressingDown = false;
	pressingRight = false;
	pressingW = false;
	pressingA = false;
	pressingS = false;
	pressingD = false;
	characterSpeed = 5;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("MyAwesomeGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWitdh, screenHeight, SDL_WINDOW_OPENGL);
	if (window == NULL){return 1;}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	IMG_Init(IMG_INIT_PNG);
	p1Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character1.png"));
	p2Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character2.png"));
	backgroundTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Background.png"));
	winTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Win.png"));
	loseTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Lose.png"));
	SDL_QueryTexture(p1Tx, nullptr, nullptr, &characterWitdh, &characterHeight);//Els dos personatges haurien de tenir la mateixa mida, de manera que no caldria repetir aixo dos cops
	p1Rect.x = screenWitdh / 2 - characterWitdh / 2;
	p1Rect.y = screenHeight / 2 - characterHeight / 2;
	p1Rect.w = characterWitdh;
	p1Rect.h = characterHeight;
	p2Rect.x = screenWitdh / 2 - characterWitdh / 2;
	p2Rect.y = screenHeight / 2 - characterHeight / 2;
	p2Rect.w = characterWitdh;
	p2Rect.h = characterHeight;
	pushR1.w = 50;
	pushR1.h = 50;
	pushR1.x = (p1Rect.x + p1Rect.h/2) - (pushR1.h/2);
	pushR1.y = (p1Rect.y + p1Rect.w / 2) - (pushR1.w / 2);
	pushR2.w = 50;
	pushR2.h = 50;
	pushR2.x = (p2Rect.x + p2Rect.h / 2) - (pushR2.h / 2);
	pushR2.y = (p2Rect.y + p2Rect.w / 2) - (pushR2.w / 2);
	objectiveR.x = 100;
	objectiveR.y = 100;
	objectiveR.w = 75;
	objectiveR.h = 75;
	p1Screen.x = 0;
	p1Screen.y = 0;
	p1Screen.w = screenWitdh/2;
	p1Screen.h = screenHeight;
	p2Screen.x = screenWitdh / 2;
	p2Screen.y = 0;
	p2Screen.w = screenWitdh / 2;
	p2Screen.h = screenHeight;

	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	music = Mix_LoadMUS("Assets/music.ogg");
	Mix_PlayMusic(music, -1);


	while (playing == true)
	{
		//Input
		while (SDL_PollEvent(&event) != 0)
		{
			switch(event.type)
			{
				case SDL_QUIT:
					playing = false;
					break;
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym)
					{
					case SDLK_UP:
						pressingUp = true;
						pushR2Pos = 1;
						break;
					case SDLK_LEFT:
						pressingLeft = true;
						pushR2Pos = 2;
						break;
					case SDLK_DOWN:
						pressingDown = true;
						pushR2Pos = 3;
						break;
					case SDLK_RIGHT:
						pressingRight = true;
						pushR2Pos = 4;
						break;
					case SDLK_w:
						pressingW = true;
						pushR1Pos = 1;
						break;
					case SDLK_a:
						pressingA = true;
						pushR1Pos = 2;
						break;
					case SDLK_s:
						pressingS = true;
						pushR1Pos = 3;
						break;
					case SDLK_d:
						pressingD = true;
						pushR1Pos = 4;
						break;
					case SDLK_g:
						//SFX
						if((p2Rect.x + p2Rect.w) > pushR1.x && p2Rect.x < (pushR1.x + pushR1.w) && (p2Rect.y + p2Rect.h) > pushR1.y && p2Rect.y < (pushR1.y + pushR1.h))
						{
							pushP2 = true;
						}
						break;
					case SDLK_0:
					case SDLK_KP_0:
						//SFX
						if ((p1Rect.x + p1Rect.w) > pushR2.x && p1Rect.x < (pushR2.x + pushR2.w) && (p1Rect.y + p1Rect.h) > pushR2.y && p1Rect.y < (pushR2.y + pushR2.h))
						{
							pushP1 = true;
						}
						break;
					case SDLK_KP_ENTER:
						if(timeFinished == false)
						{
							timeFinished = true;
						}
						else if(timeFinished = true)
						{
							timeFinished = false;
						}
						
						break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym)
					{
					case SDLK_UP:
						pressingUp = false;
						break;
					case SDLK_LEFT:
						pressingLeft = false;
						break;
					case SDLK_DOWN:
						pressingDown = false;
						break;
					case SDLK_RIGHT:
						pressingRight = false;
						break;
					case SDLK_w:
						pressingW = false;
						break;
					case SDLK_a:
						pressingA = false;
						break;
					case SDLK_s:
						pressingS = false;
						break;
					case SDLK_d:
						pressingD = false;
						break;
					}
					break;
			}
		}

		//Logic
		//- Player 1
		if (pressingW == true && p1Rect.y > 0)
		{
			p1Rect.y -= characterSpeed;
		}
		if (pressingA == true && p1Rect.x > 0)
		{
			p1Rect.x -= characterSpeed;
		}
		if (pressingS == true && p1Rect.y < screenHeight - characterHeight)
		{
			p1Rect.y += characterSpeed;
		}
		if (pressingD == true && p1Rect.x < screenWitdh - characterWitdh)
		{
			p1Rect.x += characterSpeed;
		}
		//- Player 2
		if (pressingUp == true && p2Rect.y > 0)
		{
			p2Rect.y -= characterSpeed;
		}
		if (pressingLeft == true && p2Rect.x > 0)
		{
			p2Rect.x -= characterSpeed;
		}
		if (pressingDown == true && p2Rect.y < screenHeight - characterHeight)
		{
			p2Rect.y += characterSpeed;
		}
		if (pressingRight == true && p2Rect.x < screenWitdh - characterWitdh)
		{
			p2Rect.x += characterSpeed;
		}
		//- Push rectangle 1 position
		switch(pushR1Pos)
		{
		case 0:
			pushR1.x = (p1Rect.x + p1Rect.h / 2) - (pushR1.h / 2);
			pushR1.y = (p1Rect.y + p1Rect.w / 2) - (pushR1.w / 2);
			break;
		case 1:
			pushR1.y = p1Rect.y - pushR1.h;
			pushR1.x = p1Rect.x;
			break;
		case 2:
			pushR1.x = p1Rect.x - pushR1.w;
			pushR1.y = p1Rect.y;
			break;
		case 3:
			pushR1.y = p1Rect.y + p1Rect.h;
			pushR1.x = p1Rect.x;
			break;
		case 4:
			pushR1.x = p1Rect.x + p1Rect.w;
			pushR1.y = p1Rect.y;
			break;
		}
		//- Push rectangle 2 position
		switch (pushR2Pos)
		{
		case 0:
			pushR2.x = (p2Rect.x + p2Rect.h / 2) - (pushR2.h / 2);
			pushR2.y = (p2Rect.y + p2Rect.w / 2) - (pushR2.w / 2);
			break;
		case 1:
			pushR2.y = p2Rect.y - pushR2.h;
			pushR2.x = p2Rect.x;
			break;
		case 2:
			pushR2.x = p2Rect.x - pushR2.w;
			pushR2.y = p2Rect.y;
			break;
		case 3:
			pushR2.y = p2Rect.y + p2Rect.h;
			pushR2.x = p2Rect.x;
			break;
		case 4:
			pushR2.x = p2Rect.x + p2Rect.w;
			pushR2.y = p2Rect.y;
			break;
		}
		//- Push rectangle 1 action
		if(pushP2 == true)
		{
			switch (pushR1Pos)
			{
			case 1:
				p2Rect.y -= pushDist;
				break;
			case 2:
				p2Rect.x -= pushDist;
				break;
			case 3:
				p2Rect.y += pushDist;
				break;
			case 4:
				p2Rect.x += pushDist;
				break;
			}
			if (pushR1Counter == pushDistMultiplier)
			{
				pushP2 = false;
				pushR1Counter = 0;
			}
			pushR1Counter++;
		}
		//- Push rectangle 2 action
		if (pushP1 == true)
		{
			switch (pushR2Pos)
			{
			case 1:
				p1Rect.y -= pushDist;
				break;
			case 2:
				p1Rect.x -= pushDist;
				break;
			case 3:
				p1Rect.y += pushDist;
				break;
			case 4:
				p1Rect.x += pushDist;
				break;
			}
			if (pushR2Counter == pushDistMultiplier)
			{
				pushP1 = false;
				pushR2Counter = 0;
			}
			pushR2Counter++;
		}

		//Render
		//- Time finish
		if (timeFinished == true)
		{
			p1IsIn = 0;
			p2IsIn = 0;
			//Player 1 is on the objective
			if ((p1Rect.x + p1Rect.w) > objectiveR.x && p1Rect.x < (objectiveR.x + objectiveR.w) && (p1Rect.y + p1Rect.h) > objectiveR.y && p1Rect.y < (objectiveR.y + objectiveR.h))
			{
				p1IsIn = true;
			}
			if((p2Rect.x + p2Rect.w) > objectiveR.x && p2Rect.x < (objectiveR.x + objectiveR.w) && (p2Rect.y + p2Rect.h) > objectiveR.y && p2Rect.y < (objectiveR.y + objectiveR.h))
			{
				p2IsIn = true;
			}

			if (p1IsIn == true && p2IsIn == true)
			{
				//Continue normally
			}
			else if (p1IsIn == true && p2IsIn == false)
			{
				SDL_RenderCopy(renderer, winTx, NULL, &p1Screen);
				SDL_RenderCopy(renderer, loseTx, NULL, &p2Screen);
				//Pausar joc
			}
			else if (p1IsIn == false && p2IsIn == true)
			{
				SDL_RenderCopy(renderer, loseTx, NULL, &p1Screen);
				SDL_RenderCopy(renderer, winTx, NULL, &p2Screen);
				//Pausar joc
			}
			else if (p1IsIn == false && p2IsIn == false)
			{
				SDL_RenderCopy(renderer, loseTx, NULL, &p1Screen);
				SDL_RenderCopy(renderer, loseTx, NULL, &p2Screen);
				//Pausar joc
			}
			//cambiar la posicio del rectangle
			//timeFinished = false;
		}
		else
		{
			//- Normal sprites
			SDL_RenderCopy(renderer, backgroundTx, NULL, NULL);

			if (timeFinished == true)
			{
				SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
			}
			else
			{
				SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
			}
			SDL_RenderFillRect(renderer, &objectiveR);

			SDL_SetRenderDrawColor(renderer, 102, 0, 153, 255);
			SDL_RenderFillRect(renderer, &pushR1);

			SDL_SetRenderDrawColor(renderer, 102, 0, 153, 255);
			SDL_RenderFillRect(renderer, &pushR2);

			SDL_RenderCopy(renderer, p1Tx, NULL, &p1Rect);
			SDL_RenderCopy(renderer, p2Tx, NULL, &p2Rect);
		}

		SDL_RenderPresent(renderer);
	}

	//Quit
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyTexture(p1Tx);
	SDL_DestroyTexture(p2Tx);
	SDL_DestroyTexture(backgroundTx);
	SDL_DestroyTexture(winTx);
	SDL_DestroyTexture(loseTx);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
