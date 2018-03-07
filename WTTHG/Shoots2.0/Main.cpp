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
	SDL_Window * window = nullptr;
	SDL_Renderer* renderer = nullptr;;
	SDL_Event event;
	SDL_Texture * characterTx = nullptr;;
	SDL_Texture * backgroundTx = nullptr;;
	SDL_Rect p1Rect;
	SDL_Rect p2Rect;
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
						break;
					case SDLK_LEFT:
						pressingLeft = true;
						break;
					case SDLK_DOWN:
						pressingDown = true;
						break;
					case SDLK_RIGHT:
						pressingRight = true;
						break;
					case SDLK_w:
						pressingW = true;
						break;
					case SDLK_a:
						pressingA = true;
						break;
					case SDLK_s:
						pressingS = true;
						break;
					case SDLK_d:
						pressingD = true;
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
