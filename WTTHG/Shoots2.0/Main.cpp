#include "SDL\include\SDL.h"
#include "SDL\include\SDL_image.h"
#include "SDL\include\SDL_mixer.h"
#include <time.h>//To calculate the random positions
#include <iostream>//To calculate the random positionss

#pragma comment (lib,"SDL/x86/SDL2.lib")
#pragma comment (lib,"SDL/x86/SDL2main.lib")
#pragma comment (lib,"SDL/x86/SDL2_image.lib")
#pragma comment (lib,"SDL/x86/SDL2_mixer.lib")

#define screenWitdh 1280
#define screenHeight 720
#define pushDist 26
#define pushDistMultiplier 5
//All image sizes are ajustaded automatically except for the background image

//Declaration
bool playing;
bool pressingUp, pressingLeft, pressingDown, pressingRight, pressingW, pressingA, pressingS, pressingD;
bool pushP2 = false, pushP1 = false;
bool timeFinished = false;
bool p1IsIn = false, p2IsIn = false;
bool start = false;
int characterWitdh = 0, characterHeight = 0;
int characterSpeed;
int pushR1Pos = 0;//1 = up, 2 = left, 3 = down, 4 = right
int pushR2Pos = 0;
int pushR1Counter = 0, pushR2Counter = 0;
SDL_Window * window = nullptr;
SDL_Renderer* renderer = nullptr;;
SDL_Event event;
SDL_Rect p1Rect;
SDL_Rect p2Rect;
SDL_Rect pushR1;
SDL_Rect pushR2;
SDL_Rect p1Screen;
SDL_Rect p2Screen;
SDL_Rect objectiveR;
SDL_Texture * p1Tx = nullptr;
SDL_Texture * p2Tx = nullptr;
SDL_Texture * backgroundTx = nullptr;
SDL_Texture * winTx = nullptr;
SDL_Texture * loseTx = nullptr;
SDL_Texture * objectiveTx = nullptr;
SDL_Texture * push1Tx = nullptr;
SDL_Texture * push2Tx = nullptr;
SDL_Texture * initialTx = nullptr;
Mix_Music * backgroundMusic = nullptr;
Mix_Music * winMusic = nullptr;

void Inicialization()
{
	playing = true;
	pressingUp = false;
	pressingLeft = false;
	pressingDown = false;
	pressingRight = false;
	pressingW = false;
	pressingA = false;
	pressingS = false;
	pressingD = false;
	characterSpeed = 6;

	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Let the hunger games begin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWitdh, screenHeight, 0);
	//Window error (sortir del joc)
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

	IMG_Init(IMG_INIT_PNG);
	p1Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character1.png"));
	p2Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character2.png"));
	backgroundTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Background.png"));
	winTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Win.png"));
	loseTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Lose.png"));
	objectiveTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Objective.png"));
	push1Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Push1.png"));
	push2Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Push2.png"));
	initialTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Initial.png"));
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
	pushR1.x = (p1Rect.x + p1Rect.h / 2) - (pushR1.h / 2);
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
	p1Screen.w = screenWitdh / 2;
	p1Screen.h = screenHeight;
	p2Screen.x = screenWitdh / 2;
	p2Screen.y = 0;
	p2Screen.w = screenWitdh / 2;
	p2Screen.h = screenHeight;
	srand(time(NULL));

	Mix_Init(MIX_INIT_OGG);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	backgroundMusic = Mix_LoadMUS("Assets/BackgroundMusic.ogg");
	winMusic = Mix_LoadMUS("Assets/WinMusic.ogg");
	Mix_PlayMusic(backgroundMusic, -1);
}

void InitialScreen()
{
	//Missatge: press enter to continue
	while (start == false)
	{
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_RETURN:
					start = true;
					break;
				}
				break;
			case SDL_QUIT:
				playing = false;
				break;
			}
		}
		SDL_RenderCopy(renderer, initialTx, NULL, NULL);
		SDL_RenderPresent(renderer);
	}
}

void Game()
{
	//Input
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
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
				if ((p2Rect.x + p2Rect.w) > pushR1.x && p2Rect.x < (pushR1.x + pushR1.w) && (p2Rect.y + p2Rect.h) > pushR1.y && p2Rect.y < (pushR1.y + pushR1.h))
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
			case SDLK_RETURN:
				//Quan incorporem temps haurem de canviar aixo
				timeFinished = true;
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
	if (pressingW == true)
	{
		p1Rect.y -= characterSpeed;
	}
	if (pressingA == true)
	{
		p1Rect.x -= characterSpeed;
	}
	if (pressingS == true)
	{
		p1Rect.y += characterSpeed;
	}
	if (pressingD == true)
	{
		p1Rect.x += characterSpeed;
	}
	//- Player 2
	if (pressingUp == true)
	{
		p2Rect.y -= characterSpeed;
	}
	if (pressingLeft == true)
	{
		p2Rect.x -= characterSpeed;
	}
	if (pressingDown == true)
	{
		p2Rect.y += characterSpeed;
	}
	if (pressingRight == true)
	{
		p2Rect.x += characterSpeed;
	}
	//- Push rectangle 1 position
	switch (pushR1Pos)
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
	if (pushP2 == true)
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
	////Player 1 limits
	//if (p1Rect.y < 0)
	//{
	//	p1Rect.y = 0;
	//}
	//if (p1Rect.x < 0)
	//{
	//	p1Rect.x = characterSpeed;
	//}
	//if (p1Rect.y > screenHeight - characterHeight)
	//{
	//	p1Rect.y = screenHeight - characterHeight;
	//}
	//if (p1Rect.x > screenWitdh - characterWitdh)
	//{
	//	p1Rect.x = screenWitdh - characterWitdh;
	//}
	////Player 2 limits
	//if (p2Rect.y < 0)
	//{
	//	p2Rect.y = 0;
	//}
	//if (p2Rect.x < 0)
	//{
	//	p2Rect.x = characterSpeed;
	//}
	//if (p2Rect.y > screenHeight - characterHeight)
	//{
	//	p2Rect.y = screenHeight - characterHeight;
	//}
	//if (p2Rect.x > screenWitdh - characterWitdh)
	//{
	//	p2Rect.x = screenWitdh - characterWitdh;
	//}

	//Render
	//- Normal sprites
	//--Background
	SDL_RenderCopy(renderer, backgroundTx, NULL, NULL);
	//--Objective
	SDL_RenderCopy(renderer, objectiveTx, NULL, &objectiveR);
	//--Push indicator 1
	SDL_RenderCopy(renderer, push1Tx, NULL, &pushR1);
	//--Push indicator 2
	SDL_RenderCopy(renderer, push2Tx, NULL, &pushR2);
	//--Character 1
	SDL_RenderCopy(renderer, p1Tx, NULL, &p1Rect);
	//--Character 2
	SDL_RenderCopy(renderer, p2Tx, NULL, &p2Rect);

	SDL_RenderPresent(renderer);
}

void WinLoseScreen()
{
	//Input
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_RETURN:
				//- We change the position of the objective
				objectiveR.x = rand() % (screenWitdh - objectiveR.w);
				objectiveR.y = rand() % (screenHeight - objectiveR.h);
				timeFinished = false;
				break;
			case SDLK_ESCAPE:
				playing = false;
			}
			break;
		case SDL_QUIT:
			playing = false;
			break;
		}
	}

	//Rendering
	//-Player 1 is on the objective
	if ((p1Rect.x + p1Rect.w) > objectiveR.x && p1Rect.x < (objectiveR.x + objectiveR.w) && (p1Rect.y + p1Rect.h) > objectiveR.y && p1Rect.y < (objectiveR.y + objectiveR.h))
	{
		p1IsIn = true;
	}
	else
	{
		p1IsIn = false;
	}
	//-Player 2 is on the objective
	if ((p2Rect.x + p2Rect.w) > objectiveR.x && p2Rect.x < (objectiveR.x + objectiveR.w) && (p2Rect.y + p2Rect.h) > objectiveR.y && p2Rect.y < (objectiveR.y + objectiveR.h))
	{
		p2IsIn = true;
	}
	else
	{
		p2IsIn = false;
	}
	//--Print
	if (p1IsIn == true && p2IsIn == true)
	{
		objectiveR.x = rand() % (screenWitdh - objectiveR.w);
		objectiveR.y = rand() % (screenHeight - objectiveR.h);
		timeFinished = false;
		//PauseBackgroundMusic
		//Mix_PlayMusic(backgroundMusic, -1);//Cambiar canal si no funciona
		//Continue normally
	}
	else if (p1IsIn == true && p2IsIn == false)
	{
		SDL_RenderCopy(renderer, winTx, NULL, &p1Screen);
		SDL_RenderCopy(renderer, loseTx, NULL, &p2Screen);
	}
	else if (p1IsIn == false && p2IsIn == true)
	{
		SDL_RenderCopy(renderer, loseTx, NULL, &p1Screen);
		SDL_RenderCopy(renderer, winTx, NULL, &p2Screen);
	}
	else if (p1IsIn == false && p2IsIn == false)
	{
		SDL_RenderCopy(renderer, loseTx, NULL, &p1Screen);
		SDL_RenderCopy(renderer, loseTx, NULL, &p2Screen);
	}
	SDL_RenderPresent(renderer);
}

void Quit()
{
	//Quit
	Mix_FreeMusic(backgroundMusic);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_DestroyTexture(p1Tx);
	SDL_DestroyTexture(p2Tx);
	SDL_DestroyTexture(backgroundTx);
	SDL_DestroyTexture(winTx);
	SDL_DestroyTexture(loseTx);
	SDL_DestroyTexture(objectiveTx);
	SDL_DestroyTexture(push1Tx);
	SDL_DestroyTexture(push2Tx);
	SDL_DestroyTexture(initialTx);
	IMG_Quit();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	Inicialization();
	InitialScreen();
	while (playing == true)
	{
		if (timeFinished == false)
		{
			Game();
		}
		else
		{
			WinLoseScreen();
		}
	}
	return 0;
}
