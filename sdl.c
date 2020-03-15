#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


void sdl_display(const char *input_file, const int width, const int height)
{
	int ret = -1;
	SDL_Window *sdl_window = NULL;
	SDL_Renderer *sdl_renderer = NULL;
	SDL_Texture * sdl_texture = NULL;
	FILE *fp;
	char *yuv_data = NULL;
	int duration = 40; /*根据fps计算，一秒25帧，每帧持续40ms*/
	int done = 1;

	if ((sdl_window = SDL_CreateWindow("display yuv", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE)) == NULL)
	{
		fprintf(stderr, "SDL_CreateWindow failed.\n");
		goto ret1;	
	}

	if ((sdl_renderer = SDL_CreateRenderer(sdl_window, -1, 0)) == NULL)
	{
		fprintf(stderr, "SDL_CreateRenderer failed.\n");
		goto ret1;
	}

	if ((sdl_texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_IYUV, 
					SDL_TEXTUREACCESS_STREAMING, width, height)) == NULL)
	{
		fprintf(stderr, "SDL_CreateTexture failed.\n");
		goto ret1;
	}

	if ((fp = fopen(input_file, "rb")) == NULL)
	{
		fprintf(stderr, "fopen failed.\n");
		goto ret1;
	}

	if ((yuv_data = (char *)malloc(width * height * 3 / 2 + 1)) == NULL)
	{
		fprintf(stderr, "malloc for read input_file failed.\n");
		goto ret2;
	}
	memset(yuv_data, 0, width * height * 3 / 2 + 1);

	while (done == 1)
	{
		SDL_Event event;

		while (SDL_PollEvent(&event) > 0)
		{
			if (event.type == SDL_QUIT)
			{
				printf("display end.\n");
				done = 0;
			}
		}
	
		if (fread(yuv_data, 1, width * height * 3 / 2, fp) > 0)
		{
			SDL_UpdateTexture(sdl_texture, NULL, yuv_data, width);
		}

		SDL_RenderClear(sdl_renderer);
		SDL_RenderCopy(sdl_renderer, sdl_texture, NULL, NULL);
		SDL_RenderPresent(sdl_renderer);
		SDL_Delay(duration);
	}

	SDL_Quit();


ret3:
	free(yuv_data);
ret2:
	fclose(fp);
ret1:
	return;
}


int main(int argc, const char *argv[])
{
	if (argc < 4)
	{
		fprintf(stderr, "Usage:<input file> <width> <height>\n");
		return -1;
	}

	sdl_display(argv[1], atoi(argv[2]), atoi(argv[3]));

	return 0;
}
