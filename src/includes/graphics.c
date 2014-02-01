/**
 * @Author
 * @Name: Robin Jakobsson / Sarah Larsson
 * @Email:    Rjak@kth.se /
 * @id:       910724-2696 /
 * @Grupp: 2
 */

#include "graphics.h"

SDL_Event event;

int init_GL()
{
    //Enable Alpha blending
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_NEVER);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.1f);

    //Set clear color
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    //set viewport
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    //Set projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1 );
    //Initialize modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    //If there was any errors
    if( glGetError() != GL_NO_ERROR )
    {
        return 0;
    }

    return 1;
}

int initGraphics()
{
    //init SDL
    if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
    {
        return 0;
    }

    SDL_InitSubSystem(SDL_INIT_AUDIO);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    //set icon
    Uint32          colorkey;
    SDL_Surface     *icon;

    icon = SDL_LoadBMP("textures/icon.bmp");
    printf("After load icon\n");

    colorkey = SDL_MapRGB(icon->format, 255, 255, 255);
    SDL_SetColorKey(icon, SDL_SRCCOLORKEY, colorkey);
    SDL_WM_SetIcon(icon,NULL);

    //Set video mode
    if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_OPENGL ) == NULL )
    {
        return 0;
    }

    //init GL
    if( init_GL() == 0 )
    {
        return 0;
    }
    SDL_WM_SetCaption( "Amazing Tiny Adventures", NULL );

    return 1;
}

void clean_up()
{
    glFinish();
    clean_up_menu();
    clean_up_world();
    clean_up_editor();
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
    Mix_CloseAudio();
    SDL_Quit();
    printf("The program exited correctly");
}

void options(char ip[])
{
    int quit = 0;
    int menu_choice = 0;
    //fps
    struct timer_t2 fps;

    init_options();
    init_font();

    //wait for user to continue or exit
    while( quit != 1 )
    {
        //Start the frame timer
        timer_start(&fps);
        //While there are events to handle
        while( SDL_PollEvent( &event ) )
        {

            if( event.type == SDL_QUIT )
            {
                //exit
                clean_up();
                exit(0);
            }
            if( event.type == SDL_KEYDOWN )
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        quit = 1; break;
                }
            }

            mouse_over_options(event.button.x, event.button.y);

            if( event.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        press_options(event.button.x, event.button.y);
                        break;
                }
            }


            if( event.type == SDL_MOUSEBUTTONUP)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        menu_choice = press_options(event.button.x, event.button.y);
                        break;
                }
            }

            switch(menu_choice)
            {
                case 1:
                    handle_input(event,ip);
                    printf("String in graphics.c (%s)\n",ip);
                    break;
                case 2:
                    quit = 1;
                    printf("F�rs�ker bryta ut ur options\n");
                    break;
                default:
                    break;
            }
        }
        apply_letter(ip);
        apply_texture();

        startDraw();
        //draw menu
        drawOptions(SCREEN_WIDTH, SCREEN_HEIGHT);
        drawBack();
        endDraw();

        if( timer_get_ticks(&fps) < 1000 / FRAMES_PER_SECOND )
        {
            //delay the as much time as we need to get desired frames per second
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer_get_ticks(&fps) );
        }
    }
}

void editor(world * gameWorld)
{
    int quit = 0;
    int pre_property = 0;
    int object_data = 1;
    int left_ctrl=0;
    int moving = 0;
    int old_mouse_x;
    int old_mouse_y;

    init_editor(gameWorld);

    object_data = mapLoad(gameWorld, MAP_FILE);
    //fps
    struct timer_t2 fps;

    //wait for user to continue or exit
    while( quit != 1 )
    {
        //Start the frame timer
        timer_start(&fps);
        //While there are events to handle
        while( SDL_PollEvent( &event ) )
        {

            if( event.type == SDL_QUIT )
            {
                mapSave(gameWorld, MAP_FILE);
                //exit
                clean_up();
                exit(0);
            }

            if(event.type == SDL_KEYDOWN)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_LCTRL://is left ctrl being pressed?
                        left_ctrl = 1; break;
                    default:
                        break;
                }
            }
            //set property, handle keyevents
            if(event.type == SDL_KEYUP)
            {
                switch(event.key.keysym.sym)
                {
                    case SDLK_1:
                        pre_property = 1; break;

                    case SDLK_2:
                        pre_property = 2; break;

                    case SDLK_3:
                        pre_property = 3; break;

                    case SDLK_4:
                        pre_property = 4; break;

                    case SDLK_5:
                        pre_property = 5; break;

                    case SDLK_6:
                        pre_property = 6; break;

                    case SDLK_7:
                        pre_property = 7; break;

                    case SDLK_8:
                        pre_property = 8; break;

                    case SDLK_s:
                        pre_property = 's'; break;

                    case SDLK_e:
                        pre_property = 'e'; break;

                    case SDLK_LCTRL:
                        left_ctrl = 0; break;
                    case SDLK_z:
                        if(left_ctrl == 1)
                        {
                            if(object_data>=2)
                            {
                                editor_undo(object_data,gameWorld);
                                object_data--;
                            }
                            break;
                        }
                        else
                        {
                            break;
                        }
                    //save the map before quiting
                    case SDLK_ESCAPE:
                        mapSave(gameWorld, MAP_FILE);
                        quit = 1; break;
                    default:
                        break;
                }
                break;
            }
            if(moving == 1)
            {
                editor_update_move_object(event.button.x, event.button.y, &old_mouse_x, &old_mouse_y,gameWorld);
            }
            if( event.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_RIGHT:
                        moving = 1;
                        editor_start_move_object(event.button.x, event.button.y, &old_mouse_x, &old_mouse_y);
                        break;
                }
            }
            if( event.type == SDL_MOUSEBUTTONUP)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        editor_create_object(event.button.x, event.button.y, pre_property, &object_data,gameWorld);
                        break;
                    case SDL_BUTTON_RIGHT:
                        moving = 0;
                        break;
                }
            }
        }
        //draw editor
        editor_show(SCREEN_WIDTH, SCREEN_HEIGHT,gameWorld);

        if( timer_get_ticks(&fps) < 1000 / FRAMES_PER_SECOND )
        {
            //delay the as much time as we need to get desired frames per second
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer_get_ticks(&fps) );
        }
    }
}

void graphicsMenu(world * gameWorld, char *server_ip)
{
    int in_menu = 1;
    int mouse_pressed = 0;
    int menu_choice = 0;

    //fps
    struct timer_t2 fps;

    printf("Before trying to load the menu in load manu\n");
    init_draw();
    //load menu
    if ( load_menu("menu/main_menu_background.png",
              "menu/play_inactiv.png",
              "menu/options_inactiv.png",
              "menu/editor_inactiv.png",
              "menu/exit_inactiv.png",
              "menu/back_inactiv.png",
              SCREEN_WIDTH,
              SCREEN_HEIGHT)
        == -1)
    {
        clean_up();
        printf("couldn't read texture, quitting\n");
        exit(-3);
    }
    load_level();

    printf("After load menu\n");

    //wait for user to continue or exit
    while( in_menu != 0 )
    {
        //Start the frame timer
        timer_start(&fps);
        //While there are events to handle
        while( SDL_PollEvent( &event ) )
        {

            if( event.type == SDL_QUIT )
            {
                //exit
                clean_up();
                exit(0);
            }

            if (mouse_pressed != 1)
            {
                mouse_over_menu(event.button.x, event.button.y);
            }

            if( event.type == SDL_MOUSEBUTTONDOWN)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        mouse_pressed = mouse_down_menu(event.button.x, event.button.y);
                        break;
                }
            }

            //is mousebuttom being pressed
            if( event.type == SDL_MOUSEBUTTONUP)
            {
                switch(event.button.button)
                {
                    case SDL_BUTTON_LEFT:
                        menu_choice = press_menu(event.button.x, event.button.y);
                        break;
                }
            }
        }
        //draw menu
        drawMenu(SCREEN_WIDTH, SCREEN_HEIGHT);

        switch(menu_choice)
        {
            case 1:
                printf("quitting menu, trying to play\n");
                in_menu = 0;
                drawLoadScr(SCREEN_WIDTH, SCREEN_HEIGHT);
                break;
            case 2:
                menu_choice = 0;
                options(server_ip);
                break;
            case 3:
                menu_choice = 0;
                editor(gameWorld);
                break;
            case 4:
                clean_up();
                exit(0);
                break;
            default:
                break;
        }

        if( timer_get_ticks(&fps) < 1000 / FRAMES_PER_SECOND )
        {
            //delay the as much time as we need to get desired frames per second
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - timer_get_ticks(&fps) );
        }
    }
}
