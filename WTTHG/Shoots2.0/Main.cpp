#include "SDL\include\SDL.h"
#include "SDL\include\SDL_image.h"
#include "SDL\include\SDL_mixer.h"

#pragma comment (lib,"SDL/x86/SDL2.lib")
#pragma comment (lib,"SDL/x86/SDL2main.lib")
#pragma comment (lib,"SDL/x86/SDL2_image.lib")
#pragma comment (lib, "SDL/x86/SDL2_mixer.lib");

#define screenWitdh 1280
#define screenHeight 720
//All image sizes are ajustaded automatically except for the background image

int main(int argc, char* argv[])
{
	//Declaration
	bool playing;
	bool pressingUp, pressingLeft, pressingDown, pressingRight, pressingW, pressingA, pressingS, pressingD;
	int i = 0;
	int characterWitdh = 0, characterHeight = 0;
	int projectileWitdh = 0, projectileHeight = 0;
	int characterSpeed;
	int projectileSpeed;
	int pushR1Pos = 0;//1 = up, 2 = left, 3 = down, 4 = right
	int pushR2Pos = 0;
	int pushR1Counter = 0;
	SDL_Window * window = nullptr;
	SDL_Renderer* renderer = nullptr;;
	SDL_Event event;
	SDL_Texture * characterTx = nullptr;;
	SDL_Texture * backgroundTx = nullptr;;
	SDL_Rect p1Rect;
	SDL_Rect p2Rect;
	SDL_Rect pushR1;
	SDL_Rect pushR2;
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
	projectileSpeed = 10;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("MyAwesomeGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWitdh, screenHeight, SDL_WINDOW_OPENGL);
	if (window == NULL){return 1;}
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	IMG_Init(IMG_INIT_PNG);
	characterTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character.png"));
	backgroundTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Background.png"));
	SDL_QueryTexture(characterTx, nullptr, nullptr, &characterWitdh, &characterHeight);
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
							switch(pushR1Pos)
							{
							case 1:
								p2Rect.y -= 50;
								break;
							case 2:
								p2Rect.x -= 50;
								break;
							case 3:
								p2Rect.y += 50;
								break;
							case 4:
								p2Rect.x += 50;
								break;
								//!POSSIBLE IMPROVEMENT: Amb una funcio podria passar la direccio simplement i ja em posaria cap on va
							}
						}
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
		//Player 1
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
		//Player 2
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
		//Push rectangle 1 position
		switch(pushR1Pos)
		{
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
		//Push rectangle 2 position
		switch (pushR2Pos)
		{
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
		//Push rectangle 1 action
		//if(pushR1Counter < 10)
		//{

		//}
		//Push rectangle 2 action

		//Render
		SDL_RenderCopy(renderer, backgroundTx, NULL, NULL);

		if((p1Rect.x + p1Rect.w) > objectiveR.x && p1Rect.x < (objectiveR.x + objectiveR.w) && (p1Rect.y + p1Rect.h) > objectiveR.y && p1Rect.y < (objectiveR.y + objectiveR.h))
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		}
		else if((p2Rect.x + p2Rect.w) > objectiveR.x && p2Rect.x < (objectiveR.x + objectiveR.w) && (p2Rect.y + p2Rect.h) > objectiveR.y && p2Rect.y < (objectiveR.y + objectiveR.h))
		{
			SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		}

		SDL_SetRenderDrawColor(renderer, 102, 0, 153, 255);
		SDL_RenderFillRect(renderer, &pushR1);

		SDL_SetRenderDrawColor(renderer, 102, 0, 153, 255);
		SDL_RenderFillRect(renderer, &pushR2);

		SDL_RenderFillRect(renderer, &objectiveR);

		SDL_RenderCopy(renderer, characterTx, NULL, &p1Rect);
		SDL_RenderCopy(renderer, characterTx, NULL, &p2Rect);

		SDL_RenderPresent(renderer);
	}

	//Quit
	Mix_FreeMusic(music);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyTexture(characterTx);
	SDL_DestroyTexture(backgroundTx);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
