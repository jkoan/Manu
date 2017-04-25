/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>

//Screen dimension constants
int SCREEN_WIDTH=1000;
int SCREEN_HEIGHT=480;

int HEAD_SIZE_HEIGHT=50;

int HEAD_SIZE_WIDTH=SCREEN_WIDTH;

//Starts up SDL and creates window
bool init();
void PrintKeyInfo( SDL_KeyboardEvent *key );
//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
int alien_x=0, alien_y=0;
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}


		if(TTF_Init()==-1) {
			printf("TTF_Init: %s\n", TTF_GetError());
			exit(2);
		}

		//Create window
		gWindow = SDL_CreateWindow( "Manu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN|SDL_WINDOW_FULLSCREEN_DESKTOP );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			SDL_DisplayMode current;
			if (SDL_GetNumVideoDisplays() != 1) {
				printf("Only work on 1 Display\n" );
			}
			int should_be_zero =SDL_GetCurrentDisplayMode(0, &current);
			//current.w, current.h,
			SCREEN_WIDTH=current.w;
			SCREEN_HEIGHT=current.h;
			HEAD_SIZE_WIDTH=current.w;
			HEAD_SIZE_HEIGHT=current.h*0.05;

			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_ShowCursor(SDL_DISABLE);
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Nothing to load
	return success;
}

void close()
{
	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event event;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &event ) != 0 )
				{
					//User requests quit
					switch( event.type ){
            /* Look for a keypress */
						case SDL_QUIT:
								quit = 1;
								break;
            case SDL_KEYDOWN:
                /* Check the SDLKey values and move change the coords */
                switch( event.key.keysym.sym ){
                    case SDLK_LEFT:
                        alien_x -= 1;

												if (alien_x < 0) {
													alien_x=2;
													alien_y-=1;
													if (alien_y < 0) {
														alien_y =2;
													}
												}
                        break;
                    case SDLK_RIGHT:
                        alien_x += 1;
												if (alien_x >= 3) {
													alien_x = 0;
													alien_y +=1;
													if (alien_y >= 3) {
														alien_y = 0;
													}
												}
                        break;
                    case SDLK_UP:
                        alien_y -= 1;
												if (alien_y < 0) {
													alien_y = 2;
												}
                        break;
                    case SDLK_DOWN:
                        alien_y += 1;
												if (alien_y >= 3) {
													alien_y = 0;
												}
                        break;
										case SDLK_RETURN:
												SDL_DisplayMode mode;
												printf("%d\n",mode.w );
												char rename[100];

												snprintf ( rename, 100, "./run/%d/%d/start.sh", alien_x, alien_y );


												close();
												if (system(rename)) {
													printf("failed\n");
													exit(1);
												}
												init();
												break;
										case SDLK_q:
												quit = 1;
												break;
                    default:
                        break;
                }
            }
				}

				int MAX_SELECT_AREA_HEIGHT = SCREEN_HEIGHT - HEAD_SIZE_HEIGHT-20;
				int MAX_SELECT_AREA_WIDTH  = SCREEN_WIDTH-20;

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render head
				SDL_Rect fillRecthead = { 0 , 0, HEAD_SIZE_WIDTH, HEAD_SIZE_HEIGHT };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x0F, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRecthead );

				if (alien_x == 0 && alien_y == 0) {
					SDL_Rect fillRect11_bg = {  15 , HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH/3 -10, MAX_SELECT_AREA_HEIGHT / 3-10};
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect11_bg );
				}

				//Render red filled quad
				SDL_Rect fillRect11 = {  20 , HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH/3 -20, MAX_SELECT_AREA_HEIGHT / 3-20};
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect11 );

				if (alien_x == 1 && alien_y == 0) {
					SDL_Rect fillRect12 = { MAX_SELECT_AREA_WIDTH / 3+15 , HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH / 3-10, MAX_SELECT_AREA_HEIGHT / 3-10 };
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect12 );
				}

				//Render red filled quad
				SDL_Rect fillRect12 = { MAX_SELECT_AREA_WIDTH / 3+20 , HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH / 3-20, MAX_SELECT_AREA_HEIGHT / 3-20 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect12 );

				if (alien_x == 2 && alien_y == 0) {
					SDL_Rect fillRect13 = { MAX_SELECT_AREA_WIDTH / 3*2+15 , HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH / 3-10, MAX_SELECT_AREA_HEIGHT / 3-10 };
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect13 );
				}

				//Render red filled quad
				SDL_Rect fillRect13 = { MAX_SELECT_AREA_WIDTH / 3*2+20 , HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH / 3-20, MAX_SELECT_AREA_HEIGHT / 3-20 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect13 );


				if (alien_x == 0 && alien_y == 1) {
					SDL_Rect fillRect21 = {  15 , MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH/3 -10, MAX_SELECT_AREA_HEIGHT / 3-10};
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect21 );
				}
				SDL_Rect fillRect21 = {  20 , MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH/3 -20, MAX_SELECT_AREA_HEIGHT / 3-20};
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect21 );


				if (alien_x == 1 && alien_y == 1) {
					SDL_Rect fillRect22 = { MAX_SELECT_AREA_WIDTH / 3+15 , MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH/3 -10, MAX_SELECT_AREA_HEIGHT / 3-10 };
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect22 );
				}
				//Render red filled quad
				SDL_Rect fillRect22 = { MAX_SELECT_AREA_WIDTH / 3+20 , MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH/3 -20, MAX_SELECT_AREA_HEIGHT / 3-20 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect22 );

				if (alien_x == 2 && alien_y == 1) {
					SDL_Rect fillRect23 = { MAX_SELECT_AREA_WIDTH / 3*2+15 , MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH / 3-10, MAX_SELECT_AREA_HEIGHT / 3-10 };
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect23 );
				}
				//Render red filled quad
				SDL_Rect fillRect23 = { MAX_SELECT_AREA_WIDTH / 3*2+20 , MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH / 3-20, MAX_SELECT_AREA_HEIGHT / 3-20 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect23 );

				if (alien_x == 0 && alien_y == 2) {
					SDL_Rect fillRect31 = {  15 , 2*MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH/3 -10, MAX_SELECT_AREA_HEIGHT / 3-10};
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect31 );
				}

				SDL_Rect fillRect31 = {  20 , 2*MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH/3 -20, MAX_SELECT_AREA_HEIGHT / 3-20};
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect31 );

				if (alien_x == 1 && alien_y == 2) {
					SDL_Rect fillRect32 = { MAX_SELECT_AREA_WIDTH / 3+15 , 2*MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH/3 -10, MAX_SELECT_AREA_HEIGHT / 3-10 };
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect32 );
				}

				//Render red filled quad
				SDL_Rect fillRect32 = { MAX_SELECT_AREA_WIDTH / 3+20 , 2*MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH/3 -20, MAX_SELECT_AREA_HEIGHT / 3-20 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect32 );

				if (alien_x == 2 && alien_y == 2) {
					SDL_Rect fillRect33 = { MAX_SELECT_AREA_WIDTH / 3*2+15 , 2*MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+15, MAX_SELECT_AREA_WIDTH/3 -10, MAX_SELECT_AREA_HEIGHT / 3-10 };
					SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
					SDL_RenderFillRect( gRenderer, &fillRect33 );
				}
				//Render red filled quad
				SDL_Rect fillRect33 = { MAX_SELECT_AREA_WIDTH / 3*2+20 , 2*MAX_SELECT_AREA_HEIGHT /3+HEAD_SIZE_HEIGHT+20, MAX_SELECT_AREA_WIDTH/3 -20, MAX_SELECT_AREA_HEIGHT / 3-20 };
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xF0, 0x00, 0xFF );
				SDL_RenderFillRect( gRenderer, &fillRect33 );


				//Draw blue horizontal line
				/*SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );
				SDL_RenderDrawLine( gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2 );

				//Draw vertical line of yellow dots
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
				for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
				{
					SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
				}*/

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
