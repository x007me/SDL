#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>


	const int SCREEN_WIDTH = 640 ;
	const int SCREEN_HEIGHT = 480 ; 
	const int SCREEN_BPP = 32 ;

	

	//the surfaces
	SDL_Surface *background = NULL;
	SDL_Surface *screen = NULL;
	SDL_Surface *upMessage = NULL;
	SDL_Surface *downMessage = NULL;
	SDL_Surface *leftMessage = NULL;
	SDL_Surface *rightMessage = NULL;
	SDL_Surface *message = NULL;

	SDL_Event event;

	TTF_Font *font = NULL;
	SDL_Color textColor = { 0, 0, 0};

	
	SDL_Surface *load_image (std::string filename)
	{
		SDL_Surface* loadedImage = NULL;

		SDL_Surface* optimizedImage = NULL;

		loadedImage = IMG_Load(filename.c_str());

		if(loadedImage!= NULL)
		{
			optimizedImage = SDL_DisplayFormat(loadedImage);

			SDL_FreeSurface(loadedImage);
		
			if( optimizedImage != NULL )
			{
				//Color key surface
				SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
			}
		}
	
		return optimizedImage;
	}

	void apply_surface (int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL)
	{
		SDL_Rect offset;

		offset.x = x;
		offset.y = y;
				
		SDL_BlitSurface( source, clip, destination, &offset);
	}

	bool init()
	{
		if(SDL_Init (SDL_INIT_EVERYTHING) == -1)
		{
			return 1;
		}

		screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE);

		if (screen == NULL)
		{
			return false;
		}

		if (TTF_Init() == -1)
		{
			return false;
		}

		SDL_WM_SetCaption("Press an Arrow Key", NULL);

		return true;
	}

	bool load_files()
	{
		background = load_image( "bg.bmp" );

		font = TTF_OpenFont("BRADHITC.ttf", 72);

		if(background == NULL)
		{
			return false;
		}

		if(font == NULL)
		{
			return false;
		}

	
		return true;

	}

	void clean_up()
	{
		SDL_FreeSurface (background);
		SDL_FreeSurface (upMessage);
		SDL_FreeSurface (downMessage);
		SDL_FreeSurface (leftMessage);
		SDL_FreeSurface (rightMessage);
	
		TTF_CloseFont(font);
		TTF_Quit();
		SDL_Quit();
	}

int main (int argc, char* args[])
{
	bool quit = false;

	if (init() == false)
	{
		return 1;
	}

	if (load_files () ==false)
	{
		return 1;
	
	}

	upMessage = TTF_RenderText_Solid( font, "Up was pressed.", textColor );
    downMessage = TTF_RenderText_Solid( font, "Down was pressed.", textColor );
    leftMessage = TTF_RenderText_Solid( font, "Left was pressed", textColor );
    rightMessage = TTF_RenderText_Solid( font, "Right was pressed", textColor );

	apply_surface(0,0, background, screen);

	while(quit == false)
	{
		while(SDL_PollEvent (&event))
		{
			if(event.type == SDL_KEYDOWN)
			{
				switch( event.key.keysym.sym)
				{
					case SDLK_UP: message = upMessage; break;
					case SDLK_DOWN: message = downMessage; break;
					case SDLK_LEFT: message = leftMessage; break;
					case SDLK_RIGHT: message = rightMessage; break;
				}
				
			}

			else if(event.type == SDL_QUIT)
			{
				quit = true;
			}
		}

		if( message != NULL )
        {
            //Apply the background to the screen
            apply_surface( 0, 0, background, screen );

            //Apply the message centered on the screen
            apply_surface( ( SCREEN_WIDTH - message->w ) / 2, ( SCREEN_HEIGHT - message->h ) / 2, message, screen );

            //Null the surface pointer
            message = NULL;
        }

	

		if (SDL_Flip(screen) == -1)
		{
			return 1;
		}
	}

	clean_up();

	return 0;

}
