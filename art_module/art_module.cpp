#include "WinsockSetup.h"
#include <stdio.h>
#include <stdlib.h>
#include "framework.h"
#include "art_module.h"
#include <SDL.h>

int main(int argc, char* argv[]) {
	SDL_Event event;
	int result, width = 0, height = 0, quit = 0;
	char request[10], art[256], art_id[10], char art_path[100];
	SOCKET clientSocket = socketSetup();
	SDL_Init(SDL_INIT_VIDEO);
	
	// Loop para conexão do soquete com o módulo de questionário.
	while (1) {
		result = connectSocket(clientSocket);
		if (result != 0) {
			perror("Trying again...\n");
			Sleep(3000);
			continue;
		}
		break;
	}

	/*
	 *	Recebe uma mensagem do módulo de questionário.essa mensagem representa
	 *	o ID da arte que será carregada posteriormente
	 *
	 *  @param clientSocket		soquete do cliente para enviar o tipo de entrada.
	 *  @param art_id			string que representa o ID da arte.
	 *  @param sizeof(art_id)	tamanho da string.
	 *  @param 0				flag que modifica o comportamento da função "recv()".
	 */	
	result = recv(clientSocket, art_id, sizeof(art_id), 0);

	/*
	 * Verifica o ID recebido acima com o propósito de ajustar a resolução
	 * da janela que será criada posteriormente.
	 */

	// O ID "art_id" é convertido para inteiro usando a função "atoi()".
	switch (atoi(art_id)) {
	case 0:
	case 1:
	case 2:
	case 4:
	case 6:
	case 8:
	case 9:
	case 10:
	case 11:
		// Se o ID estiver em uma das condições acima, define a largura e altura da janela.
		width = 600;	// Largura da janela.
		height = 720;	// Altura da janela.
		break;

	// Se o ID não estiver em nenhum dos casos acima, utiliza a resolução padrão.
	default:
		width = 800;	// Largura da janela.
		height = 520;	// Altura da janela.
		break;
	}	

	/*
	 * Define o caminho da arte a ser carregada de acordo com o ID recebido pelo
	 * módulo de questionário e atribui o mesmo à variável "art_path".
	 */
	for (int i = 0; i < 16; i++) {
		if (atoi(art_id) == i) {
			sprintf(art_path, "..\\art_module\\art_resources\\art%d.bmp", i);
			break;
		}
	}

	/* 
	 *	Cria uma janela.
	 * 
	 *  @param ""						representa o título da janela, no caso vazio.
	 *  @param 30						coordenada "x" para posicionamento da janela.
	 *  @param 40						coordenada "y" para posicionamento da janela.
	 *  @param width					largura da janela.
	 *  @param height					altura da janela.
	 *  @param SDL_WINDOW_BORDERLESS	janela sem borda.
	 */	
	SDL_Window* window = SDL_CreateWindow("", 30, 40, width, height, SDL_WINDOW_BORDERLESS);

	/*
	 * Carrega a imagem associada ao caminho em "art_path" na janela criada anteriormente.
	 *
	 * @param art_path	caminho da imagem.
	 */
	SDL_Surface* image = SDL_LoadBMP(art_path);

	/*
	 * Cria um objeto SDL_Renderer que será utilizado para renderizar gráficos na janela criada.
	 *
	 * @param window					janela em que o renderizador irá desenhar.
	 * @param -1						especifíca o driver gráfico a ser usado.
	 * @param SDL_RENDERER_ACCELERATED	flag que controlam o comportamento do renderizador.
	 */
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	/*
	 * Cria uma textura SDL_Texture a partir de uma superfície SDL_Surface. Esta textura será 
	 * utilizada para renderizar a imagem associada ao renderizador.
	 *
	 * @param renderer	ponteiro para o objeto "SDL_Renderer".
	 * @param image		ponteiro para a superfície "SDL_Surface".
	 */
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);

	SDL_Rect destRect;
	destRect.x = 0;
	destRect.y = 0;
	destRect.w = width;
	destRect.h = height;

	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_RenderPresent(renderer);

	// Loop que aguarda o evento de fechamento da janela.
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