#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include "framework.h"
#include "art_module.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
	int result, width = 0, height = 0;
	char request[10], art[256], bnum[10];
	SOCKET clientSocket = socketSetup();
	SDL_Init(SDL_INIT_VIDEO);

	while (1) {
		result = connectSocket(clientSocket);
		if (result != 0) {
			perror("Error connecting socket.\n");
			perror("Trying again...\n");
			Sleep(3000);
			continue;
		}
		break;
	}

	result = recv(clientSocket, bnum, sizeof(bnum), 0);

	int inum = atoi(bnum);
	switch (inum) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 6:
	case 8:
	case 9:
	case 10:
	case 11:
		width = 600;
		height = 720;
		break;
	default:
		width = 800;
		height = 520;
		break;
	}
	char art_path[100];
	for (int i = 0; i < 16; i++) {
		if (inum == i) {
			sprintf(art_path, "..\\art_module\\art_resources\\art%d.bmp", i);
			break;
		}
	}

	SDL_Window* window = SDL_CreateWindow("", 30, 40, width, height, SDL_WINDOW_BORDERLESS);

	SDL_Surface* image = SDL_LoadBMP(art_path);

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = 0;
	destRect.w = width;
	destRect.h = height;

	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, texture, NULL, &destRect);

	SDL_RenderPresent(renderer);

	SDL_Event event;
	int quit = 0;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = 1;
			}
		}
	}

	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}