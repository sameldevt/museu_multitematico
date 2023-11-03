#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define MAP_MENU ""
#define MAP_SANTOS_DUMONT ""
#define MAP_ART_WEEK ""
#define MAP_TECNOLOGY_SECURITY ""
#define MAP_OLIMPIC_GAMES ""

void loadScreen(char screen_path[100]) {
	system("cls");
	FILE* fp = fopen(screen_path, "r");

	char buffer[256];

	while (fgets(buffer, sizeof(buffer), fp) != NULL) {
		printf("%s", buffer);
	}

	fclose(fp);
}

int main() {
	int key;

	while (1) {
		while (_kbhit() == NULL) {
			loadScreen(MAP_MENU);
			key = _getch();

			switch (key) {
			case 49:
				loadScreen(MAP_SANTOS_DUMONT);
				key = _getch();
				if (key == 81) {
					break;
				}
			case 50:
				loadScreen(MAP_ART_WEEK);
				key = _getch();
				if (key == 81) {
					break;
				}
			case 51:
				loadScreen(MAP_TECNOLOGY_SECURITY);
				key = _getch();
				if (key == 81) {
					break;
				}
			case 52:
				loadScreen(MAP_OLIMPIC_GAMES);
				key = _getch();
				if (key == 81) {
					break;
				}
			default:
				continue;
			}
		}
		Sleep(500);
		continue;
	}
}