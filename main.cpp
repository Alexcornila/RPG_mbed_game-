
// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
int go_up ();
int go_left ();
int go_down ();
int go_right ();
int action_button ();
void menu_button ();
void context_button1 ();
void context_button2 ();
void draw_game (int init);
void init_main_map ();
int main ();
void npc_interact();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct
{
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    int has_spear;
    int omni; // If player is omnipotent
    int health;
    int gamestate;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 3
#define MENU_BUTTON 1
#define OMNI_BUTTON 2
#define CONTEXT_BUTTON 4
#define GO_LEFT 5
#define GO_RIGHT 6
#define GO_UP 7
#define GO_DOWN 8
int get_action(GameInputs inputs)
{
    if (!inputs.b1) {
        // pc.printf("MENU\r\n"); // menu button takes priority in inputs
        return MENU_BUTTON;
    } else if (!inputs.b3) { // other buttons follow, and accelerometer input is last
        // pc.printf("ACTION\r\n");
        return ACTION_BUTTON;
    } else if (!inputs.b2) {
        // pc.printf("OMNI TOGGLE\r\n");
        return OMNI_BUTTON;
    } else if (!inputs.b4) {
        // pc.printf("C2\r\n");
        return CONTEXT_BUTTON;
    } else if (inputs.ax > 0.30) {
        // pc.printf("GO RIGHT\r\n");
        return GO_RIGHT;
    } else if (inputs.ax < -0.30) {
        // pc.printf("GO LEFT\r\n");
        return GO_LEFT;
    } else if (inputs.ay > 0.30) {
        // pc.printf("GO UP\r\n");
        return GO_UP;
    } else if (inputs.ay < -0.30) {
        // pc.printf("GO DOWN\r\n");
        return GO_DOWN;
    } else {
        return NO_ACTION;
    }

}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define GAME_WIN 3
int update_game(int action)
{
    if (Player.health == 0) return GAME_OVER;
    if (Player.gamestate > 4) return GAME_WIN;
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;



    if ((Player.x ==22) && (Player.y == 0))
       {
           speech("Going into", "second map");
           map_erase(Player.x, Player.y);
           fflush(pc);
           set_active_map(1);
           map2();
           //px py
           Player.x =30;
           Player.y =30;
           //set_active_map(1);
           return FULL_DRAW;
       }



       if ((Player.x ==49) && (Player.y == 13))
       {
           speech("Going into", "first map");
           map_erase(Player.x, Player.y);
           fflush(pc);
           set_active_map(0);
           init_main_map();
           //px py
           Player.x =4;
           Player.y =4;
           //set_active_map(1);
           return FULL_DRAW;
       }




    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    switch(action)
    {
        case GO_UP:
            return go_up();
        case GO_LEFT:
            return go_left();
        case GO_DOWN:
            return go_down();
        case GO_RIGHT:
            return go_right();
        case ACTION_BUTTON:
            return action_button();
        case MENU_BUTTON:
            menu_button();
            break;
        case OMNI_BUTTON:
            Player.omni = !Player.omni;
            break;
        case CONTEXT_BUTTON:
            context_button2();
            break;
        default:        break;
    }
    return NO_RESULT;
}

/**
 * Helper function for if the player goes up. Returns the action to take
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define GAME_WIN 3
int go_up()
{
    MapItem* north = get_north(Player.x, Player.y);
    if (Player.omni || !north || north -> walkable == true) {
     Player.py = Player.y;
     Player.y--;
        if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1) {
            int health = --Player.health;
            draw_upper_status(health, Player.x, Player.y);
        }
        if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py)) {
            return FULL_DRAW;
        }
    }
    return NO_RESULT;
}

/**
 * Helper function for if the player goes left.
 */
int go_left()
{
    MapItem* west = get_west(Player.x, Player.y);
    if (Player.omni || !west || west -> walkable == true) {
     Player.px = Player.x;
     Player.x--;
        if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1) {
            int health = --Player.health;
            draw_upper_status(health, Player.x, Player.y);
        }
        if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py)) {
            return FULL_DRAW;
        }
    }
    return NO_RESULT;
}

/**
 * Helper function for if the player goes down.
 */
int go_down()
{
    MapItem* south = get_south(Player.x, Player.y);
    if (Player.omni || !south || south -> walkable == true) {
     Player.py = Player.y;
     Player.y++;
        if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1) {
            int health = --Player.health;
            draw_upper_status(health, Player.x, Player.y);
        }
        if (get_here(Player.x, Player.y) || get_here(Player.px, Player.py)) {
            return FULL_DRAW;
        }
    }
    return NO_RESULT;
}

/**
 * Helper function for if the player goes right.
 */
int go_right()
{
    MapItem* east = get_east(Player.x, Player.y);
    if (Player.omni || !east || east -> walkable == true) {
     Player.px = Player.x;
     Player.x++;
        if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1) {
            int health = --Player.health;
            draw_upper_status(health, Player.x, Player.y);
        }
        if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py)) {
            return FULL_DRAW;
        }
    }
    return NO_RESULT;
}

/**
 * Helper function for if the player uses the action button.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define GAME_WIN 3
int action_button()
{

    //speaker.write(0.0f);
    MapItem* east = get_east(Player.x, Player.y);
    MapItem* west = get_west(Player.x, Player.y);
    MapItem* north = get_east(Player.x, Player.y);
    MapItem* south = get_east(Player.x, Player.y);

    if (east -> type == KEY)
    {
        Player.has_key = true;
        map_erase(Player.x + 1, Player.y);
    } else if (west -> type == KEY)
    {
        Player.has_key = true;
        map_erase(Player.x - 1, Player.y);
    } else if (north -> type == KEY)
    {
        Player.has_key = true;
        map_erase(Player.x, Player.y - 1);
    } else if (south -> type == KEY)
    {
        Player.has_key = true;
        map_erase(Player.x, Player.y + 1);
    } else if (east -> type == NPC || west -> type == NPC || north -> type == NPC || south -> type == NPC)
    {
        npc_interact();
        return FULL_DRAW;
    } else if (east -> type == DOOR && Player.has_key)
    {
        speech("You unlocked the door.", NULL);
        map_erase(Player.x + 1, Player.y);
    } else if (west -> type == DOOR && Player.has_key)
    {
        speech("You unlocked the door.", NULL);
        map_erase(Player.x - 1, Player.y);
    } else if (north -> type == DOOR && Player.has_key)
    {
        speech("You unlocked the door.", NULL);
        map_erase(Player.x, Player.y - 1);
    } else if (south -> type == DOOR && Player.has_key)
    {
        speech("You unlocked the door.", NULL);
        map_erase(Player.x, Player.y + 1);
    } else if (east -> type == LIFE)
    {
            if (Player.health < 3)
                {
                    Player.health++;
                    map_erase(Player.x + 1, Player.y);
                }
    } else if (west -> type == LIFE)
    {
            if (Player.health < 3)
                {
                    Player.health++;
                    map_erase(Player.x - 1, Player.y);
                }
    } else if (north -> type == LIFE)
    {
            if (Player.health < 3)
                {
                    Player.health++;
                    map_erase(Player.x - 1, Player.y);
                }
    } else if (south -> type == LIFE)
    {
            if (Player.health < 3)
                {
                    Player.health++;
                    map_erase(Player.x - 1, Player.y);
                }
    } else if (east -> type == SPEAR)
    {
            speech("asteroid found!", NULL);
            Player.has_spear = true;
            map_erase(Player.x + 1, Player.y);
    } else if (west -> type == SPEAR)
    {
            speech("asteroid found!", NULL);
            Player.has_spear = true;
            map_erase(Player.x - 1, Player.y);
    } else if (north -> type == SPEAR)
    {
            speech("asteroid found!", NULL);
            Player.has_spear = true;
            map_erase(Player.x, Player.y - 1);
    } else if (south -> type == SPEAR)
    {
            speech("asteroid found!", NULL);
            Player.has_spear = true;
            map_erase(Player.x, Player.y + 1);
    } else if (east -> type == TURRET || west -> type == TURRET || north -> type == TURRET || south -> type == TURRET)
    {
        Player.gamestate = 5;
        return GAME_WIN;
    } else {
        return NO_RESULT;
    }
    return FULL_DRAW;
}

/**
 * Helper function for if the player uses the menu button.
 */
void menu_button()
{
}

/**
 * Helper function for if the player uses the second context button.
 */
void context_button2()
{
}

/**
 * Helper function to interact with the NPC.
 */
void npc_interact()
{
    if (Player.gamestate == 0) {
        speech("Hello Optimus!", "Can you help me get out of here.");
        Player.gamestate++;
    } else {
        if (!Player.has_spear && (Player.gamestate == 1 || Player.gamestate == 3))
        {
            speech("You need to find the key", "Watch for trooper!");
        } else if (Player.gamestate == 1) {
            speech("Thank you for helping me", "I need to find the asteroid");
            Player.has_key = true;
            Player.gamestate++;
            Player.has_spear = false;
        } else if (Player.gamestate == 2) {
            speech("Can I count on you?", "This asteroid has some magical powers");
            Player.gamestate++;
        } else if (Player.gamestate == 3) {
            speech("Thank you for helping me!", "You saved the planet!");
             if (Player.health < 3)
                {
                    Player.health++;
                }
            Player.gamestate++;
        }
    }
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status(Player.health, Player.x, Player.y);
    draw_lower_status();
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{


    pc.printf("big string done!\r\n");
    for (int x = 0; x < map_width(); x++)
    {
        for (int y = 0; y < map_height(); y++)
        {
            int XY_KEY = x + y * map_width();
            if (startmap[XY_KEY] == 'W') add_wall(x, y, HORIZONTAL, 1);
            // else if (startmap[XY_KEY] == 'P') add_plant(x, y);
            // else if (startmap[XY_KEY] == '^') add_spikes(x, y);
            // //else if (startmap[XY_KEY] == '~') add_water(x, y);
            // //else if (startmap[XY_KEY] == 'S') add_skeleton(x, y);
            // else if (startmap[XY_KEY] == 'H') add_door(x, y);
            // else if (startmap[XY_KEY] == 'K') add_key(x, y);
            // //else if (startmap[XY_KEY] == 'C') add_chest(x, y);
            // //else if (startmap[XY_KEY] == 'N') add_NPC(x, y);
            // else if (startmap[XY_KEY] == '+') add_heart(x, y);
            // else if (startmap[XY_KEY] == '|') add_spear(x, y);
            // //else continue;
        //  }
      //  }

                 add_wall(10, 1, VERTICAL, 6);
                 add_door(10,16);
                 add_wall(10, 1, VERTICAL, 6);
                 add_wall(10,8, VERTICAL, 6);
                 add_door(10,7);
                 add_wall(10,9,HORIZONTAL, 5);
                 add_wall(10,14,VERTICAL, 3);
                 add_wall(10,17,HORIZONTAL, 4);
                 add_wall(14,17,VERTICAL, 17);
                 add_wall(15,9, HORIZONTAL, 3);
                 add_wall(18,9,VERTICAL,6);
                 add_wall(24,9,HORIZONTAL, -14);
                 add_wall(14,33, VERTICAL,6);
                 add_wall(14,40, VERTICAL,9);
                 add_wall(10,14,HORIZONTAL,5);
                 add_trooper(17,11);
                 add_NPC(11,4);
                 add_wall(13,1, VERTICAL,4);

                 add_turret(23,5);
                 add_trooper(10,29);
                 add_trooper(6,9);
                 add_trooper(46,40);
                 add_wall(26,18,HORIZONTAL,15);
                 add_door(16,21);
                 add_door(22,0);


        }
    }


    print_map();
}





void map2()
{

     Map* map2 = set_active_map(1);
     for(int i = map_width() + 3; i < map_area(); i += 39)
     {
         //add_plant2(i % map_width(), i / map_width());
         add_plant(i % map_width(), i / map_width());
     }

     add_wall(0, 0, HORIZONTAL, map_width());
     add_wall(0, map_height()-1, HORIZONTAL, map_width());
     add_wall(0, 0,  VERTICAL,   map_height());
     add_wall(map_width()-1,  0, VERTICAL,   map_height());
     add_wall(map_width()/2,  0, VERTICAL,    map_height());
     add_wall(10, 1, VERTICAL, 6);
     add_door(10,16);
     add_wall(10, 1, VERTICAL, 6);
     add_wall(10,8, VERTICAL, 6);
     add_door(10,7);
     add_wall(10,9,HORIZONTAL, 5);
     add_wall(10,14,VERTICAL, 3);
     add_wall(10,17,HORIZONTAL, 4);
     add_wall(14,17,VERTICAL, 17);
     add_wall(15,9, HORIZONTAL, 3);
     add_wall(18,9,VERTICAL,6);
     add_wall(24,9,HORIZONTAL, -14);
     add_wall(14,33, VERTICAL,6);
     add_wall(14,40, VERTICAL,9);
     add_wall(10,14,HORIZONTAL,5);
     add_door(49,13);
     //draw_nothing(49, 13);

     add_wall(30,30,VERTICAL,-4);
     add_wall(30,26,HORIZONTAL,5);
     add_wall(35,26,VERTICAL,6);
     add_wall(35,20,HORIZONTAL,3);
     add_wall(38,20,VERTICAL,17);
     add_wall(38,37,HORIZONTAL,2);
     add_wall(40,37,VERTICAL,-17);
     add_wall(40,20,HORIZONTAL,4);
     add_wall(44,20,VERTICAL,4);
     add_wall(44,24,HORIZONTAL,4);
     add_wall(40,18,VERTICAL,7);
     add_wall(40,25,HORIZONTAL,-8);
     add_wall(32,25,VERTICAL,-4);
      add_wall(32,21,HORIZONTAL,-4);
      add_wall(28,21,VERTICAL,7);
      add_wall(28,28,HORIZONTAL,10);
      add_wall(38,28,VERTICAL,3);
      add_wall(38,31,HORIZONTAL,4);
     print_map();

}





/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");

    // Initialize the maps
    maps_init();
    init_main_map();
    //uLCD.printf("\nHello World!\nrajshiddapur");
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;

    // Splash/start screen
    uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
    uLCD.color(WHITE);
    uLCD.locate(6, 2);
    uLCD.printf("Intro");
    uLCD.locate(1, 6);
    uLCD.printf("The Transformers");
    uLCD.locate(2, 10);
    uLCD.printf("Press button        to start");
    GameInputs in = read_inputs();
    while (in.b1) {
        in = read_inputs();
        continue;
    }
    uLCD.cls();

    // Initial drawing
    draw_game(true);
    //speech("test a longer string", "test2");

    // Initial player data
    Player.omni = false;
    Player.health = 3;
    Player.has_key = false;
    Player.has_spear = false;
    Player.gamestate = 0;

    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();

        // Actually do the game update:

        // 1. Read inputs
        GameInputs values = read_inputs();
        // 2. Determine action (get_action)
        int action = get_action(values);
        // 3. Update game (update_game)
        int updated = update_game(action);
        // 3b. Check for game over / game win conditions
        if (updated == 1) {
            draw_gameover();
            break;
        } else if (updated == 3) {
            draw_gamewin();
            break;
        }
        // 4. Draw frame (draw_game)
        draw_game(updated);
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}









//
// // Project includes
// #include "globals.h"
// #include "hardware.h"
// #include "map.h"
// #include "graphics.h"
// #include "Speaker.h"
// #include "speech.h"
//
// // Functions in this file
// int get_action (GameInputs inputs);
// int update_game (int action);
// int go_up ();
// int go_left ();
// int go_down ();
// int go_right ();
// int action_button ();
// void menu_button ();
// void f_button1 ();
// void f_button2 ();
// void draw_game (int init);
// void init_main_map ();
// int main ();
// void npc_interact();
void map2();
//
// /**
//  * The main game state. Must include Player locations and previous locations for
//  * drawing to work properly. Other items can be added as needed.
//  */
// struct
// {
//     int x,y;    // Current locations
//     int px, py; // Previous locations
//     int has_key;
//     int has_gun;
//     int omni;
//     int health;
//     int state;
// } Player;
//
// /**
//  * Given the game inputs, determine what kind of update needs to happen.
//  * Possbile return input are defined below.
//  */
// #define NO_ACTION 0
// #define ACTION_BUTTON 3
// #define MENU_BUTTON 1
// #define OMNI_BUTTON 2
// #define C_BUTTON 4
// #define GO_LEFT 5
// #define GO_RIGHT 6
// #define GO_UP 7
// #define GO_DOWN 8
// int get_action(GameInputs inputs)
// {
//     if (!inputs.b1)
//     {
//
//         return MENU_BUTTON;
//     } else if (!inputs.b3)
//     { // other buttons follow, and accelerometer input is last
//
//         return ACTION_BUTTON;
//     } else if (!inputs.b2)
//     {
//
//         return OMNI_BUTTON;
//     } else if (!inputs.b4)
//     {
//
//         return C_BUTTON;
//     } else if (inputs.ax > 0.3)
//     {
//
//         return GO_RIGHT;
//     } else if (inputs.ax < -0.3)
//     {
//
//         return GO_LEFT;
//     } else if (inputs.ay > 0.3)
//     {
//
//         return GO_UP;
//     } else if (inputs.ay < -0.3)
//     {
//
//         return GO_DOWN;
//     } else
//     {
//         return NO_ACTION;
//     }
//
// }
//
// /**
//  * Update the game state based on the user action. For example, if the user
//  * requests GO_UP, then this function should determine if that is possible by
//  * consulting the map, and update the Player position accordingly.
//  *
//  * Return input are defined below. FULL_DRAW indicates that for this frame,
//  * draw_game should not optimize drawing and should draw every tile, even if
//  * the player has not moved.
//  */
// #define NO_RESULT 0
// #define GAME_OVER 1
// #define FULL_DRAW 2
// #define GAME_WIN 3
//
// int update_game(int action)
// {
//     if (Player.health == 0) return GAME_OVER;
//     if (Player.state > 4) return GAME_WIN;
//     // Save player previous location before updating
//     Player.px = Player.x;
//     Player.py = Player.y;
//     ////
//
//     if ((Player.x ==22) && (Player.y == 0))
//     {
//         speech("Going into", "second map");
//         map_erase(Player.x, Player.y);
//         fflush(pc);
//         set_active_map(1);
//         map2();
//         //px py
//         Player.x =30;
//         Player.y =30;
//         //set_active_map(1);
//         return FULL_DRAW;
//     }
//
//
//
//     if ((Player.x ==49) && (Player.y == 13))
//     {
//         speech("Going into", "first map");
//         map_erase(Player.x, Player.y);
//         fflush(pc);
//         set_active_map(0);
//         init_main_map();
//         //px py
//         Player.x =4;
//         Player.y =4;
//         //set_active_map(1);
//         return FULL_DRAW;
//     }
//
//
//
//
//
//     // if ((Player.x ==15) && (Player.y == 13))
//     // {
//     //    speech("Hi there", "Optimus");
//     //
//     // }
//
//
//
//     // Do different things based on the each action.
//     // You can define functions like "go_up()" that get called for each case.
//     switch(action)
//     {
//         case GO_UP:
//             return go_up();
//         case GO_LEFT:
//             return go_left();
//         case GO_DOWN:
//             return go_down();
//         case GO_RIGHT:
//             return go_right();
//         case ACTION_BUTTON:
//             return action_button();
//         case MENU_BUTTON:
//             menu_button();
//             break;
//         case OMNI_BUTTON:
//             Player.omni = !Player.omni;
//             break;
//         case C_BUTTON:
//             f_button2();
//             break;
//         default:
//          break;
//     }
//     return NO_RESULT;
// }
//
//
//
// /**
//  * Helper function for if the player goes up. Returns the action to take
//  */
// #define NO_RESULT 0
// #define GAME_OVER 1
// #define FULL_DRAW 2
// #define GAME_WIN 3
//
//
// int go_up()
// {
//     MapItem* north = get_north(Player.x, Player.y);
//     if (Player.omni || !north || north -> walkable == true)
//     {
//      Player.py = Player.y;
//      Player.y--;
//         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
//         {
//             int health = --Player.health;
//             draw_upper_status(health, Player.x, Player.y);
//         }
//         if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py))
//         {
//             return FULL_DRAW;
//         }
//     }
//     return NO_RESULT;
// }
//
// /**
//  * Helper function for if the player goes left.
//  */
// int go_left()
// {
//     MapItem* west = get_west(Player.x, Player.y);
//     if (Player.omni || !west || west -> walkable == true)
//      {
//      Player.px = Player.x;
//      Player.x--;
//         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
//         {
//             int health = --Player.health;
//             draw_upper_status(health, Player.x, Player.y);
//         }
//         if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py))
//         {
//             return FULL_DRAW;
//         }
//     }
//     return NO_RESULT;
// }
//
// /**
//  * Helper function for if the player goes down.
//  */
// int go_down()
// {
//     MapItem* south = get_south(Player.x, Player.y);
//     if (Player.omni || !south || south -> walkable == true)
//     {
//      Player.py = Player.y;
//      Player.y++;
//         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
//         {
//             int health = --Player.health;
//             draw_upper_status(health, Player.x, Player.y);
//         }
//         if (get_here(Player.x, Player.y) || get_here(Player.px, Player.py))
//         {
//             return FULL_DRAW;
//         }
//     }
//     return NO_RESULT;
// }
//
// /**
//  * Helper function for if the player goes right.
//  */
// int go_right()
// {
//     MapItem* east = get_east(Player.x, Player.y);
//     if (Player.omni || !east || east -> walkable == true)
//     {
//      Player.px = Player.x;
//      Player.x++;
//         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
//         {
//             int health = --Player.health;
//             draw_upper_status(health, Player.x, Player.y);
//         }
//         if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py))
//         {
//             return FULL_DRAW;
//         }
//     }
//     return NO_RESULT;
// }
//
// /**
//  * Helper function for if the player uses the action button.
//  */
// #define NO_RESULT 0
// #define GAME_OVER 1
// #define FULL_DRAW 2
// #define GAME_WIN  3
//
//
// int action_button()
// {
//
//     MapItem* east = get_east(Player.x, Player.y);
//     MapItem* west = get_west(Player.x, Player.y);
//     MapItem* north = get_north(Player.x, Player.y);
//     MapItem* south = get_south(Player.x, Player.y);
//
//      if (east -> type == KEY)
//     {
//         Player.has_key = true;
//         map_erase(Player.x + 1, Player.y);
//     }
//     else if (west -> type == KEY)
//     {
//         Player.has_key = true;
//         map_erase(Player.x - 1, Player.y);
//     }
//     else if (north -> type == KEY)
//     {
//         Player.has_key = true;
//         map_erase(Player.x, Player.y - 1);
//     }
//     else if (south -> type == KEY)
//     {
//         Player.has_key = true;
//         map_erase(Player.x, Player.y + 1);
//     }
//     else if (east -> type == NPC || west -> type == NPC || north -> type == NPC || south -> type == NPC)
//     {   speech("Its good to", "see you Optimus!");
//
//         npc_interact();
//         return FULL_DRAW;
//     }
//     else if (east -> type == DOOR && Player.has_key)
//     {
//         speech("Door is open.", "Nice!");
//         map_erase(Player.x + 1, Player.y);
//     }
//     else if (west -> type == DOOR && Player.has_key)
//     {
//         speech("Door is open.","A");
//         map_erase(Player.x - 1, Player.y);
//     }
//     else if (north -> type == DOOR && Player.has_key)
//     {
//         speech("Door is open.","B");
//         map_erase(Player.x, Player.y - 1);
//     }
//     else if (south -> type == DOOR && Player.has_key)
//     {
//         speech("Door","C");
//         map_erase(Player.x, Player.y + 1);
//     }
//     else if (east -> type == LIFE)
//     {
//             if (Player.health < 3)
//                 {
//                     Player.health++;
//                     map_erase(Player.x + 1, Player.y);
//                 }
//     }
//     else if (west -> type == LIFE)
//     {
//             if (Player.health < 3)
//                 {
//                     Player.health++;
//                     map_erase(Player.x - 1, Player.y);
//                 }
//     }
//     else if (north -> type == LIFE)
//     {
//             if (Player.health < 3)
//                 {
//                     Player.health++;
//                     map_erase(Player.x - 1, Player.y);
//                 }
//     }
//     else if (south -> type == LIFE)
//     {
//             if (Player.health < 3)
//                 {
//                     Player.health++;
//                     map_erase(Player.x - 1, Player.y);
//                 }
//     }
//     else if (east -> type == GUN)
//     {
//             speech("gun found!","D");
//             Player.has_gun = true;
//             map_erase(Player.x + 1, Player.y);
//     }
//     else if (west -> type == GUN)
//     {
//             speech("gun found!","E");
//             Player.has_gun = true;
//             map_erase(Player.x - 1, Player.y);
//     }
//     else if (north -> type == GUN)
//     {
//             speech("ohhh great!","F");
//             Player.has_gun = true;
//             map_erase(Player.x, Player.y - 1);
//     }
//     else if (south -> type == GUN)
//     {
//             speech("gun found!","G");
//             Player.has_gun = true;
//             map_erase(Player.x, Player.y + 1);
//     }
//     else if (east -> type == TURRET || west -> type == TURRET || north -> type == TURRET || south -> type == TURRET)
//     {
//         Player.state = 5;
//         return GAME_WIN;
//     }
//     else
//     {
//         return NO_RESULT;
//     }
//     return FULL_DRAW;
// }
//
// /**
//  * Helper function for if the player uses the menu button.
//  */
// void menu_button()
// {
// }
//
// /**
//  * Helper function for if the player uses the second button.
//  */
// void f_button2()
// {
//   //use to fire gun
// }
//
// /**
//  * Helper function to interact with the NPC.
//  */
//  //state machine to det next state
// void npc_interact()
// {
//
//  // const char* speak1 = "You will need a gun!";
//  // const char* speak2 = "Look around for one";
//  // const char* speak3 = "found it";
//  // const char* speak4 = "Lets kill some troopers";
//     if (Player.state == 0)
//     {
//         speech("A","Nice!");
//         Player.state++;
//     } else
//     {
//         if (!Player.has_gun && (Player.state == 1 || Player.state == 3))
//         {
//             speech("B","Nice2");
//         } else if (Player.state == 1)
//         {
//             speech("B", "A");
//             Player.has_key = true;
//             Player.state++;
//             Player.has_gun = false;
//         } else if (Player.state == 2)
//         {
//             speech("ABC","B");
//             Player.state++;
//         } else if (Player.state == 3)
//         {
//             speech("FGH", "F");
//              if (Player.health < 3)
//                 {
//                     Player.health++;
//                 }
//             Player.state++;
//         }
//     }
// }
//
//
// /**
//  * Entry point for frame drawing. This should be called once per iteration of
//  * the game loop. This draws all tiles on the screen, followed by the status
//  * bars. Unless init is nonzero, this function will optimize drawing by only
//  * drawing tiles that have changed from the previous frame.
//  */
//
// void draw_game(int init)
// {
//     // Draw game border first
//     if(init) draw_border();
//     // Iterate over all visible map tiles
//     for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
//     {
//         for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
//         {
//             // Here, we have a given (i,j)
//
//             // Compute the current map (x,y) of this tile
//             int x = i + Player.x;
//             int y = j + Player.y;
//
//             // Compute the previous map (px, py) of this tile
//             int px = i + Player.px;
//             int py = j + Player.py;
//
//             // Compute u,v coordinates for drawing
//             int u = (i+5)*11 + 3;
//             int v = (j+4)*11 + 15;
//
//             // Figure out what to draw
//             DrawFunc draw = NULL;
//             if (init && i == 0 && j == 0) // Only draw the player on init
//             {
//                 draw_player(u, v, Player.has_key);
//                 continue;
//             }
//             else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
//             {
//                 MapItem* curr_item = get_here(x, y);
//                 MapItem* prev_item = get_here(px, py);
//                 if (init || curr_item != prev_item) // Only draw if they're different
//                 {
//                     if (curr_item) // There's something here! Draw it
//                     {
//                         draw = curr_item->draw;
//                     }
//                     else // There used to be something, but now there isn't
//                     {
//                         draw = draw_nothing;
//                     }
//                 }
//             }
//             else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
//             {
//                 draw = draw_wall;
//             }
//
//             // Actually draw the tile
//             if (draw)
//             draw(u, v);
//         }
//     }
//
//     // Draw status bars
//     draw_upper_status(Player.health, Player.x, Player.y);
//     draw_lower_status();
// }
//
//
// void init_main_map()
// {
//
//      Map* map = set_active_map(0);
//      for(int i = map_width() + 3; i < map_area(); i += 39)
//      {
//          add_plant(i % map_width(), i / map_width());
//      }
//
//
//
//
//
//      add_wall(0, 0, HORIZONTAL, map_width());
//      add_wall(0, map_height()-1, HORIZONTAL, map_width());
//      add_wall(0, 0,  VERTICAL,   map_height());
//      add_wall(map_width()-1,  0, VERTICAL,   map_height());
//      add_wall(map_width()/2,  0, VERTICAL,    map_height());
//      add_wall(10, 1, VERTICAL, 6);
//      add_door(10,16);
//      add_wall(10, 1, VERTICAL, 6);
//      add_wall(10,8, VERTICAL, 6);
//      add_door(10,7);
//      add_wall(10,9,HORIZONTAL, 5);
//      add_wall(10,14,VERTICAL, 3);
//      add_wall(10,17,HORIZONTAL, 4);
//      add_wall(14,17,VERTICAL, 17);
//      add_wall(15,9, HORIZONTAL, 3);
//      add_wall(18,9,VERTICAL,6);
//      add_wall(24,9,HORIZONTAL, -14);
//      add_wall(14,33, VERTICAL,6);
//      add_wall(14,40, VERTICAL,9);
//      add_wall(10,14,HORIZONTAL,5);
//      add_trooper(17,11);
//      add_NPC(15,12);
//      add_turret(23,5);
//      add_trooper(10,29);
//      add_trooper(5,4);
//      add_trooper(46,40);
//      add_wall(26,18,HORIZONTAL,15);
//      add_door(16,21);
//      add_door(22,0);
//      //draw_nothing(22,0);
//
//
//      //  const char* startmap = "K N";
//      //
//      //
//      //  for (int x = 0; x < map_width(); x++)
//      //  {
//      //    for (int y = 0; y < map_height(); y++)
//      //    {
//      //        add_plant(i % map_width(), i / map_width());
//      //        int XY_KEY = x + y * map_width();
//      //        if (startmap[XY_KEY] == 'K') add_key(9,8);
//      //        else if (startmap[XY_KEY] == 'N') add_NPC(11,10);
//      //        else continue;
//      //      }
//      // }
//
//      //add_key(4,20);W
//
//
//
//      //print_map();
//
// }
//
//
// void map2()
// {
//
//      Map* map2 = set_active_map(1);
//      for(int i = map_width() + 3; i < map_area(); i += 39)
//      {
//          add_plant2(i % map_width(), i / map_width());
//      }
//
//      add_wall(0, 0, HORIZONTAL, map_width());
//      add_wall(0, map_height()-1, HORIZONTAL, map_width());
//      add_wall(0, 0,  VERTICAL,   map_height());
//      add_wall(map_width()-1,  0, VERTICAL,   map_height());
//      add_wall(map_width()/2,  0, VERTICAL,    map_height());
//      add_wall(10, 1, VERTICAL, 6);
//      add_door(10,16);
//      add_wall(10, 1, VERTICAL, 6);
//      add_wall(10,8, VERTICAL, 6);
//      add_door(10,7);
//      add_wall(10,9,HORIZONTAL, 5);
//      add_wall(10,14,VERTICAL, 3);
//      add_wall(10,17,HORIZONTAL, 4);
//      add_wall(14,17,VERTICAL, 17);
//      add_wall(15,9, HORIZONTAL, 3);
//      add_wall(18,9,VERTICAL,6);
//      add_wall(24,9,HORIZONTAL, -14);
//      add_wall(14,33, VERTICAL,6);
//      add_wall(14,40, VERTICAL,9);
//      add_wall(10,14,HORIZONTAL,5);
//      add_door(49,13);
//      //draw_nothing(49, 13);
//
//
//      add_wall(30,30,VERTICAL,-4);
//      add_wall(30,26,HORIZONTAL,5);
//      add_wall(35,26,VERTICAL,6);
//      add_wall(35,20,HORIZONTAL,3);
//      add_wall(38,20,VERTICAL,17);
//      add_wall(38,37,HORIZONTAL,2);
//      add_wall(40,37,VERTICAL,-17);
//      add_wall(40,20,HORIZONTAL,4);
//      add_wall(44,20,VERTICAL,4);
//      add_wall(44,24,HORIZONTAL,4);
//      add_wall(40,18,VERTICAL,7);
//      add_wall(40,25,HORIZONTAL,-8);
//      add_wall(32,25,VERTICAL,-4);
//       add_wall(32,21,HORIZONTAL,-4);
//       add_wall(28,21,VERTICAL,7);
//       add_wall(28,28,HORIZONTAL,10);
//       add_wall(38,28,VERTICAL,3);
//       add_wall(38,31,HORIZONTAL,4);
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//      print_map();
//
// }
//
//
//
//
// /**
//  * Program entry point! This is where it all begins.
//  * This function orTURRETrates all the parts of the game. Most of your
//  * implementation should be elsewhere - this holds the game loop, and should
//  * read like a road map for the rest of the code.
//  */
//
// int main()
// {
//     GameInputs input;
//     int action;
//     int updated;
//
//     // First things first: initialize hardware
//     ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
//
//     // Initialize the maps
//     maps_init();
//     init_main_map();
//     //mapq();
//     set_active_map(0);
//     Player.x = Player.y = 5;
//
//     // Splash/start screen
//     uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
//     uLCD.color(WHITE);
//     uLCD.cls();
//     uLCD.locate(3, 3);
//     uLCD.text_width(2);
//     uLCD.text_height(2);
//     uLCD.printf("Intro");
//     wait(2);
//     uLCD.cls();
//     uLCD.locate(2, 2);
//     uLCD.text_width(2);
//     uLCD.text_height(2);
//     uLCD.printf("The");
//     uLCD.locate(1,4);
//     uLCD.text_width(2);
//     uLCD.text_height(2);
//     uLCD.printf("Transformers!");
//     // mySpeaker.PlayNote(400.0, .25, .1);
//     // wait(0.2);
//     // mySpeaker.PlayNote(200.0, .25, .1);
//     // wait(0.2);
//     // mySpeaker.PlayNote(100.0, .25, .1);
//     wait(3);
//     uLCD.cls();
//     GameInputs in = read_inputs();
//     while (in.b1)
//     {
//         in = read_inputs();
//         continue;
//     }
//     uLCD.cls();
//
//     // Initial drawing
//     draw_game(true);
//
//
//     // Initial player data
//     Player.omni = false;
//     Player.health = 3;
//     Player.has_key = false;
//     Player.has_gun = false;
//     Player.state = 0;
//
//     // Main game loop
//     while(1)
//     {
//         // Timer to measure game update speed
//         Timer t; t.start();
//
//         // Actually do the game update:
//
//         // 1. Read inputs
//         input = read_inputs();
//         // 2. Determine action (get_action)
//         action = get_action(input);
//         // 3. Update game (update_game)
//         updated = update_game(action);
//         // 3b. Check for game over / game win conditions
//         if (updated == 1)
//         {
//             draw_gameover();
//             break;
//         } else if (updated == 3)
//         {
//             draw_gamewin();
//             break;
//         }
//         // 4. Draw frame (draw_game)
//         draw_game(updated);
//         // 5. Frame delay
//         t.stop();
//         int dt = t.read_ms();
//         if (dt < 100) wait_ms(100 - dt);
//     }
// }
//
//
//
//
//
//
// // // // Project includes
// // // #include "globals.h"
// // // #include "hardware.h"
// // // #include "map.h"
// // // #include "graphics.h"
// // // #include "Speaker.h"
// // // #include "speech.h"
// //
// // // Project includes
// // #include "globals.h"
// // #include "hardware.h"
// // #include "map.h"
// // #include "graphics.h"
// // #include "speech.h"
// //
// //
// // // // Functions in this file
// // // int get_action (GameInputs inputs);
// // // int update_game (int action);
// // // int go_up ();
// // // int go_left ();
// // // int go_down ();
// // // int go_right ();
// // // int action_button ();
// // // void menu_button ();
// // // void f_button1 ();
// // // void f_button2 ();
// // // void draw_game (int init);
// // // void init_main_map ();
// // // int main ();
// // // void npc_interact();
// // // void map2();
// //
// // // Functions in this file
// // int get_action (GameInputs inputs);
// // int update_game (int action);
// // void draw_game (int init);
// // void init_main_map ();
// // //void map2();
// // void map2()
// // int Do_Action(MapItem* item);
// // void ncp1_action(MapItem* item);
// // void ncp2_action(MapItem* item);
// // int DEBUG = 0;
// // int main ();
// //
// //
// //
// // // /**
// // //  * The main game state. Must include Player locations and previous locations for
// // //  * drawing to work properly. Other items can be added as needed.
// // //  */
// //
// // // struct
// // // {
// // //     int x,y;    // Current locations
// // //     int px, py; // Previous locations
// // //     int has_key;
// // //     int has_gun;
// // //     int omni;
// // //     int health;
// // //     int state;
// // // } Player;
// // //
// //
// // struct {
// //     int x,y;    // Current locations
// //     int px, py; // Previous locations
// //     int quest;
// //     int has_key; //
// //     int has_book;
// //     int map;
// //     int applesGot;
// //     int lives;
// //     int HP;
// //     bool ncp1Meeting;
// //     bool ncp2Meeting;
// //     bool ghostMode; // Omnipotient Mode;
// //     bool menu;
// // } Player;
// //
// //
// // // /**
// // //  * Given the game inputs, determine what kind of update needs to happen.
// // //  * Possbile return input are defined below.
// // //  */
// // // #define NO_ACTION 0
// // // #define ACTION_BUTTON 3
// // // #define MENU_BUTTON 1
// // // #define OMNI_BUTTON 2
// // // #define C_BUTTON 4
// // // #define GO_LEFT 5
// // // #define GO_RIGHT 6
// // // #define GO_UP 7
// // // #define GO_DOWN 8
// //
// // #define NO_ACTION 0
// // #define ACTION_BUTTON 1
// // #define MENU_BUTTON 2
// // #define OMNI_BUTTON 3 // my addition
// // #define DROP_BUTTON 8
// // #define GO_LEFT 4
// // #define GO_RIGHT 5
// // #define GO_UP 6
// // #define GO_DOWN 7
// //
// //
// // int get_action(GameInputs inputs)
// // {
// //     if (!inputs.b1)
// //     {
// //
// //         return MENU_BUTTON;
// //     } else if (!inputs.b3)
// //     { // other buttons follow, and accelerometer input is last
// //
// //         return ACTION_BUTTON;
// //     } else if (!inputs.b2)
// //     {
// //
// //         return OMNI_BUTTON;
// //     } else if (!inputs.b4)
// //     {
// //
// //         return C_BUTTON;
// //     } else if (inputs.ax > 0.3)
// //     {
// //
// //         return GO_RIGHT;
// //     } else if (inputs.ax < -0.3)
// //     {
// //
// //         return GO_LEFT;
// //     } else if (inputs.ay > 0.3)
// //     {
// //
// //         return GO_UP;
// //     } else if (inputs.ay < -0.3)
// //     {
// //
// //         return GO_DOWN;
// //     } else
// //     {
// //         return NO_ACTION;
// //     }
// //
// // }
// //
// //
// //
// // // int get_action(GameInputs inputs)
// // // {
// // //     //uLCD.printf("%d,%d",inputs.ax,inputs.ay);
// // //     if(!inputs.b1) return ACTION_BUTTON;
// // //     if(!inputs.b2) return MENU_BUTTON;
// // //     if(!inputs.b3) return OMNI_BUTTON;
// // //     if(!inputs.b4) return DROP_BUTTON;
// // //     if((inputs.ax)*(inputs.ax) > (inputs.ay)*(inputs.ay))
// // //     {
// // //         if(inputs.ax > 0.2)
// // //         return GO_RIGHT;
// // //         if(inputs.ax < -0.2)
// // //         return GO_LEFT;
// // //     }
// // //     if((inputs.ax)*(inputs.ax) < (inputs.ay)*(inputs.ay))
// // //     {
// // //         if(inputs.ay > 0.2)
// // //         return GO_UP;
// // //         if(inputs.ay < -0.2)
// // //         return GO_DOWN;
// // //     }
// // //     return NO_ACTION;
// // // }
// //
// //
// //
// // // /**
// // //  * Update the game state based on the user action. For example, if the user
// // //  * requests GO_UP, then this function should determine if that is possible by
// // //  * consulting the map, and update the Player position accordingly.
// // //  *
// // //  * Return input are defined below. FULL_DRAW indicates that for this frame,
// // //  * draw_game should not optimize drawing and should draw every tile, even if
// // //  * the player has not moved.
// // //  */
// //
// // #define NO_RESULT 0
// // #define GAME_OVER 1
// // #define FULL_DRAW 2
// // #define FAIL 3
// //
// // // #define NO_RESULT 0
// // // #define GAME_OVER 1
// // // #define FULL_DRAW 2
// // // #define GAME_WIN 3
// //
// //
// //
// // //
// // // int update_game(int action)
// // // {
// // //     if (Player.health == 0) return GAME_OVER;
// // //     if (Player.state > 4) return GAME_WIN;
// // //     // Save player previous location before updating
// // //     Player.px = Player.x;
// // //     Player.py = Player.y;
// // //     ////
// // //
// // //     if ((Player.x ==22) && (Player.y == 0))
// // //     {
// // //         speech("Going into", "second map");
// // //         map_erase(Player.x, Player.y);
// // //         fflush(pc);
// // //         set_active_map(1);
// // //         map2();
// // //         //px py
// // //         Player.x =30;
// // //         Player.y =30;
// // //         //set_active_map(1);
// // //         return FULL_DRAW;
// // //     }
// // //
// // //
// // //
// // //     if ((Player.x ==49) && (Player.y == 13))
// // //     {
// // //         speech("Going into", "first map");
// // //         map_erase(Player.x, Player.y);
// // //         fflush(pc);
// // //         set_active_map(0);
// // //         init_main_map();
// // //         //px py
// // //         Player.x =4;
// // //         Player.y =4;
// // //         //set_active_map(1);
// // //         return FULL_DRAW;
// // //     }
// // //
// // //
// // //
// // //
// // //
// // //     // if ((Player.x ==15) && (Player.y == 13))
// // //     // {
// // //     //    speech("Hi there", "Optimus");
// // //     //
// // //     // }
// // //
// //
// // int update_game(int action)
// // {
// //     // Save player previous location before updating
// //     Player.px = Player.x;
// //     Player.py = Player.y;
// //
// //     // Do different things based on the each action.
// //     // You can define functions like "go_up()" that get called for each case.
// //     switch(action)
// //     {
// //         case GO_UP: MapItem* itemN = get_north(Player.x,Player.y);
// //                     if(itemN->walkable || Player.ghostMode) Player.y = Player.y-1;
// //                     return FULL_DRAW;
// //
// //         case GO_LEFT: MapItem* itemW = get_west(Player.x,Player.y);
// //                     if(itemW->walkable || Player.ghostMode) Player.x = Player.x-1;
// //                     return FULL_DRAW;
// //
// //         case GO_DOWN: MapItem* itemS = get_south(Player.x,Player.y);
// //                     if(itemS->walkable || Player.ghostMode) Player.y = Player.y+1;
// //                     return FULL_DRAW;
// //
// //         case GO_RIGHT: MapItem* itemE = get_east(Player.x,Player.y);
// //                     if(itemE->walkable || Player.ghostMode) Player.x = Player.x+1;
// //                     return FULL_DRAW;
// //
// //         case ACTION_BUTTON: if(Player.menu) return FAIL;
// //                             int action;
// //                             action = Do_Action(get_north(Player.x,Player.y));
// //                             if(action == GAME_OVER) return action;
// //                             action = Do_Action(get_west(Player.x,Player.y));
// //                             if(action == GAME_OVER) return action;
// //                             action = Do_Action(get_south(Player.x,Player.y));
// //                             if(action == GAME_OVER) return action;
// //                             action = Do_Action(get_east(Player.x,Player.y));
// //                             if(action == GAME_OVER) return action;
// //
// //                             if(NO_ACTION) return action;
// //                             return FULL_DRAW;
// //
// //         case MENU_BUTTON: if(!Player.menu)
// //                           {
// //                             menu(Player.quest);
// //                             Player.menu = true;
// //                           }
// //
// //
// //                           break;
// //
// //         case OMNI_BUTTON: Player.ghostMode = !Player.ghostMode;
// //                             if(Player.ghostMode)print_ghost();
// //                             else clear_ghost();
// //                           break;
// //         case DROP_BUTTON: if(Player.applesGot > 0)
// //                           {   //drop items
// //                             Player.applesGot--;
// //                           }
// //                           break;
// //     }
// //     return NO_RESULT;
// // }
// //
// //
// //
// //
// // //state machine
// // //quest and buttons inputs
// // int Do_Action(MapItem* item)
// // {
// //     switch(item->type)
// //     {
// //         case NPC1: ncp1_action(item);
// //                   return FULL_DRAW;
// //         case NPC2: ncp2_action(item);
// //                   return FULL_DRAW;
// //         case TREE: Player.applesGot++;
// //                    return FULL_DRAW;
// //         case BOOK: Player.has_book = 1;
// //                    map_erase(1, 1);
// //                    draw_game(FULL_DRAW);
// //                    Player.quest = 5;
// //                    return FULL_DRAW;
// //         case CHEST: map_erase(31, 21);
// //                     draw_game(FULL_DRAW);
// //                     add_diamond(31,21);
// //                     draw_game(FULL_DRAW);
// //                     return FULL_DRAW;;
// //         case DIAMOND: map_erase(31, 21);
// //                       //print_diamond();
// //                       draw_game(FULL_DRAW);
// //                       return GAME_OVER;
// //         case HOUSE:
// //             switch(Player.map)
// //             {
// //                 case 0: if(Player.ncp2Meeting || DEBUG)
// //                 {
// //                             map2();
// //                             set_active_map(1);
// //                             print_book();
// //                             Player.map = 1;
// //                             Player.x = 1;
// //                             Player.y = 6;
// //                             draw_game(FULL_DRAW);
// //                             break;
// //                         }
// //                 case 1: if(Player.has_book || DEBUG)
// //                 {
// //                             init_main_map();
// //                             set_active_map(0);
// //                             print_book();
// //                             Player.map = 0;
// //                             Player.x = 29;
// //                             Player.y = 16;
// //                             MapItem* npc2 = get_here(31, 19);
// //                             npc2->data++;
// //                             draw_game(FULL_DRAW);
// //                             break;
// //                         }
// //                         else
// //                         {
// //                             char* line1;
// //                             char* line2;
// //                             line1 = "Get the book!";
// //                             line2 = "@#$&^*";
// //                             speech(line1,line2);
// //                             return FULL_DRAW;
// //                         }
// //             }
// //             return FULL_DRAW;
// //         case DOOR1: if(Player.has_key)
// //                     {
// //                         add_Open_door1(19,28);
// //                         draw_game(FULL_DRAW);
// //                         clear_item();
// //                         Player.quest = 3;
// //                         mySpeaker.PlayNote(500.0,0.1,0.1);
// //                         mySpeaker.PlayNote(600.0,0.1,0.1);
// //                     }
// //                     else
// //                     {
// //                         char* line1;
// //                         char* line2;
// //                         line1 = "You need a Key";
// //                         line2 = "to open.";
// //                         speech(line1,line2);
// //                     }
// //                     return FULL_DRAW;
// //         default : return NO_RESULT;
// //     }
// // }
// //
// //
// //
// //
// // //first quest set free a friend
// // void ncp1_action(MapItem* item)
// // {
// //     char* line1;
// //     char* line2;
// //     switch(item->data)
// //     {
// //         case 0:
// //                 line1 = "Hi there Optimus";
// //                 line2 = "can you get me";
// //                 speech(line1,line2);
// //                 line1 = "out of here?";
// //                 line2 = "you need to blow";
// //                 speech(line1,line2);
// //                 line1 = "up this gate";
// //                 line2 = "look for some TNT";
// //                 speech(line1,line2);
// //                 Player.quest = 1;
// //                 item->data++;
// //                 print_apple();
// //                 Player.applesGot = 0;
// //                 Player.ncp1Meeting = true;
// //                 break;
// //         case 1:
// //                 if(Player.applesGot < 1)
// //                 {
// //                     line1 = "empty handed";
// //                     line2 = "won't help";
// //                     speech(line1,line2);
// //                 }
// //                 else{
// //                     line1 = "thank you!";
// //                     line2 = "my friend";
// //                     speech(line1,line2);
// //                     line1 = "I'll keep my pr";
// //                     line2 = "omise. ";
// //                     speech(line1,line2);
// //                     line1 = "here you Go.";
// //                     line2 = "This will get";
// //                     speech(line1,line2);
// //                     line1 = "you there.";
// //                     line2 = "";
// //                     speech(line1,line2);
// //                     item->data++;
// //                     clear_item();
// //                     clear_item_count();
// //                     print_key();
// //                     Player.has_key = 1;
// //                     Player.applesGot = NULL;
// //                     Player.ncp1Meeting = false;
// //                     mySpeaker.PlayNote(500.0,0.1,0.1);
// //                     mySpeaker.PlayNote(600.0,0.1,0.1);
// //                     Player.quest = 2;
// //                     }
// //                     break;
// //         default:
// //                 line1 = "I did what I ";
// //                 line2 = "could.......";
// //                 speech(line1,line2);
// //
// //
// //     }
// // }
// //
// //
// //
// //
// // void ncp2_action(MapItem* item)
// // {
// //     char* line1;
// //     char* line2;
// //     switch(item->data)
// //     {
// //         case 0: line1 = "I'm guessing you";
// //                 line2 = "already meet him";
// //                 speech(line1,line2);
// //                 line1 = "Bring me";
// //                 line2 = "the book of spell";
// //                 speech(line1,line2);
// //                 line1 = "and you will get";
// //                 line2 = "what you want.";
// //                 speech(line1,line2);
// //                 Player.quest = 4;
// //                 item->data++;
// //                 print_book();
// //                 Player.has_book = 0;
// //                 Player.ncp2Meeting = true;
// //                 break;
// //
// //         case 1: if(!Player.has_book)
// //                   {
// //                     line1 = "You need to";
// //                     line2 = "get the book";
// //                     speech(line1,line2);
// //                     line1 = "for me to";
// //                     line2 = "help you!";
// //                     speech(line1,line2);
// //                   }
// //                 else
// //                   {
// //                     line1 = "Haha!";
// //                     line2 = "";
// //                     speech(line1,line2);
// //                     line1 = "you pass the";
// //                     line2 = "test. ";
// //                     speech(line1,line2);
// //                     line1 = "here you Go.";
// //                     line2 = "go get the";
// //                     speech(line1,line2);
// //                     line1 = "Diamond!!.";
// //                     line2 = "";
// //                     speech(line1,line2);
// //                     item->data++;
// //                     clear_item();
// //                     clear_item_count();
// //                     Player.ncp2Meeting = false;
// //                     add_chest(31,21);
// //                     draw_game(FULL_DRAW);
// //                     mySpeaker.PlayNote(500.0,0.1,0.1);
// //                     mySpeaker.PlayNote(600.0,0.1,0.1);
// //                     Player.quest = 6;
// //                 }
// //                     break;
// //
// //         default:
// //                 line1 = "Go on!";
// //                 line2 = "GET IT!!";
// //                 speech(line1,line2);
// //               }
// // }
// //
// //
// //
// //
// // //
// // //
// // //     // Do different things based on the each action.
// // //     // You can define functions like "go_up()" that get called for each case.
// // //     switch(action)
// // //     {
// // //         case GO_UP:
// // //             return go_up();
// // //         case GO_LEFT:
// // //             return go_left();
// // //         case GO_DOWN:
// // //             return go_down();
// // //         case GO_RIGHT:
// // //             return go_right();
// // //         case ACTION_BUTTON:
// // //             return action_button();
// // //         case MENU_BUTTON:
// // //             menu_button();
// // //             break;
// // //         case OMNI_BUTTON:
// // //             Player.omni = !Player.omni;
// // //             break;
// // //         case C_BUTTON:
// // //             f_button2();
// // //             break;
// // //         default:
// // //          break;
// // //     }
// // //     return NO_RESULT;
// // // }
// // //
// // //
// //
// //
// // //
// // // /**
// // //  * Helper function for if the player goes up. Returns the action to take
// // //  */
// // // #define NO_RESULT 0
// // // #define GAME_OVER 1
// // // #define FULL_DRAW 2
// // // #define GAME_WIN 3
// // //
// // //
// // // int go_up()
// // // {
// // //     MapItem* north = get_north(Player.x, Player.y);
// // //     if (Player.omni || !north || north -> walkable == true)
// // //     {
// // //      Player.py = Player.y;
// // //      Player.y--;
// // //         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
// // //         {
// // //             int health = --Player.health;
// // //             draw_upper_status(health, Player.x, Player.y);
// // //         }
// // //         if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py))
// // //         {
// // //             return FULL_DRAW;
// // //         }
// // //     }
// // //     return NO_RESULT;
// // // }
// // //
// // // /**
// // //  * Helper function for if the player goes left.
// // //  */
// // // int go_left()
// // // {
// // //     MapItem* west = get_west(Player.x, Player.y);
// // //     if (Player.omni || !west || west -> walkable == true)
// // //      {
// // //      Player.px = Player.x;
// // //      Player.x--;
// // //         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
// // //         {
// // //             int health = --Player.health;
// // //             draw_upper_status(health, Player.x, Player.y);
// // //         }
// // //         if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py))
// // //         {
// // //             return FULL_DRAW;
// // //         }
// // //     }
// // //     return NO_RESULT;
// // // }
// // //
// // // /**
// // //  * Helper function for if the player goes down.
// // //  */
// // // int go_down()
// // // {
// // //     MapItem* south = get_south(Player.x, Player.y);
// // //     if (Player.omni || !south || south -> walkable == true)
// // //     {
// // //      Player.py = Player.y;
// // //      Player.y++;
// // //         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
// // //         {
// // //             int health = --Player.health;
// // //             draw_upper_status(health, Player.x, Player.y);
// // //         }
// // //         if (get_here(Player.x, Player.y) || get_here(Player.px, Player.py))
// // //         {
// // //             return FULL_DRAW;
// // //         }
// // //     }
// // //     return NO_RESULT;
// // // }
// // //
// // // /**
// // //  * Helper function for if the player goes right.
// // //  */
// // // int go_right()
// // // {
// // //     MapItem* east = get_east(Player.x, Player.y);
// // //     if (Player.omni || !east || east -> walkable == true)
// // //     {
// // //      Player.px = Player.x;
// // //      Player.x++;
// // //         if (!Player.omni && ((int) (get_here(Player.x, Player.y)) -> data) == 1)
// // //         {
// // //             int health = --Player.health;
// // //             draw_upper_status(health, Player.x, Player.y);
// // //         }
// // //         if (get_here(Player.x, Player.y)|| get_here(Player.px, Player.py))
// // //         {
// // //             return FULL_DRAW;
// // //         }
// // //     }
// // //     return NO_RESULT;
// // // }
// // //
// // // /**
// // //  * Helper function for if the player uses the action button.
// // //  */
// // // #define NO_RESULT 0
// // // #define GAME_OVER 1
// // // #define FULL_DRAW 2
// // // #define GAME_WIN  3
// // //
// // //
// // // int action_button()
// // // {
// // //
// // //     MapItem* east = get_east(Player.x, Player.y);
// // //     MapItem* west = get_west(Player.x, Player.y);
// // //     MapItem* north = get_north(Player.x, Player.y);
// // //     MapItem* south = get_south(Player.x, Player.y);
// // //
// // //      if (east -> type == KEY)
// // //     {
// // //         Player.has_key = true;
// // //         map_erase(Player.x + 1, Player.y);
// // //     }
// // //     else if (west -> type == KEY)
// // //     {
// // //         Player.has_key = true;
// // //         map_erase(Player.x - 1, Player.y);
// // //     }
// // //     else if (north -> type == KEY)
// // //     {
// // //         Player.has_key = true;
// // //         map_erase(Player.x, Player.y - 1);
// // //     }
// // //     else if (south -> type == KEY)
// // //     {
// // //         Player.has_key = true;
// // //         map_erase(Player.x, Player.y + 1);
// // //     }
// // //     else if (east -> type == NPC || west -> type == NPC || north -> type == NPC || south -> type == NPC)
// // //     {   speech("Its good to", "see you Optimus!");
// // //
// // //         npc_interact();
// // //         return FULL_DRAW;
// // //     }
// // //     else if (east -> type == DOOR && Player.has_key)
// // //     {
// // //         speech("Door is open.", "Nice!");
// // //         map_erase(Player.x + 1, Player.y);
// // //     }
// // //     else if (west -> type == DOOR && Player.has_key)
// // //     {
// // //         speech("Door is open.","A");
// // //         map_erase(Player.x - 1, Player.y);
// // //     }
// // //     else if (north -> type == DOOR && Player.has_key)
// // //     {
// // //         speech("Door is open.","B");
// // //         map_erase(Player.x, Player.y - 1);
// // //     }
// // //     else if (south -> type == DOOR && Player.has_key)
// // //     {
// // //         speech("Door","C");
// // //         map_erase(Player.x, Player.y + 1);
// // //     }
// // //     else if (east -> type == LIFE)
// // //     {
// // //             if (Player.health < 3)
// // //                 {
// // //                     Player.health++;
// // //                     map_erase(Player.x + 1, Player.y);
// // //                 }
// // //     }
// // //     else if (west -> type == LIFE)
// // //     {
// // //             if (Player.health < 3)
// // //                 {
// // //                     Player.health++;
// // //                     map_erase(Player.x - 1, Player.y);
// // //                 }
// // //     }
// // //     else if (north -> type == LIFE)
// // //     {
// // //             if (Player.health < 3)
// // //                 {
// // //                     Player.health++;
// // //                     map_erase(Player.x - 1, Player.y);
// // //                 }
// // //     }
// // //     else if (south -> type == LIFE)
// // //     {
// // //             if (Player.health < 3)
// // //                 {
// // //                     Player.health++;
// // //                     map_erase(Player.x - 1, Player.y);
// // //                 }
// // //     }
// // //     else if (east -> type == GUN)
// // //     {
// // //             speech("gun found!","D");
// // //             Player.has_gun = true;
// // //             map_erase(Player.x + 1, Player.y);
// // //     }
// // //     else if (west -> type == GUN)
// // //     {
// // //             speech("gun found!","E");
// // //             Player.has_gun = true;
// // //             map_erase(Player.x - 1, Player.y);
// // //     }
// // //     else if (north -> type == GUN)
// // //     {
// // //             speech("ohhh great!","F");
// // //             Player.has_gun = true;
// // //             map_erase(Player.x, Player.y - 1);
// // //     }
// // //     else if (south -> type == GUN)
// // //     {
// // //             speech("gun found!","G");
// // //             Player.has_gun = true;
// // //             map_erase(Player.x, Player.y + 1);
// // //     }
// // //     else if (east -> type == TURRET || west -> type == TURRET || north -> type == TURRET || south -> type == TURRET)
// // //     {
// // //         Player.state = 5;
// // //         return GAME_WIN;
// // //     }
// // //     else
// // //     {
// // //         return NO_RESULT;
// // //     }
// // //     return FULL_DRAW;
// // // }
// // //
// //
// //
// // // /**
// // //  * Helper function for if the player uses the menu button.
// // //  */
// // // void menu_button()
// // // {
// // // }
// // //
// // // /**
// // //  * Helper function for if the player uses the second button.
// // //  */
// // // void f_button2()
// // // {
// // //   //use to fire gun
// // // }
// // //
// // // /**
// // //  * Helper function to interact with the NPC.
// // //  */
// // //  //state machine to det next state
// // // void npc_interact()
// // // {
// // //
// // //  // const char* speak1 = "You will need a gun!";
// // //  // const char* speak2 = "Look around for one";
// // //  // const char* speak3 = "found it";
// // //  // const char* speak4 = "Lets kill some troopers";
// // //     if (Player.state == 0)
// // //     {
// // //         speech("A","Nice!");
// // //         Player.state++;
// // //     } else
// // //     {
// // //         if (!Player.has_gun && (Player.state == 1 || Player.state == 3))
// // //         {
// // //             speech("B","Nice2");
// // //         } else if (Player.state == 1)
// // //         {
// // //             speech("B", "A");
// // //             Player.has_key = true;
// // //             Player.state++;
// // //             Player.has_gun = false;
// // //         } else if (Player.state == 2)
// // //         {
// // //             speech("ABC","B");
// // //             Player.state++;
// // //         } else if (Player.state == 3)
// // //         {
// // //             speech("FGH", "F");
// // //              if (Player.health < 3)
// // //                 {
// // //                     Player.health++;
// // //                 }
// // //             Player.state++;
// // //         }
// // //     }
// // // }
// // //
// // //
// // // /**
// // //  * Entry point for frame drawing. This should be called once per iteration of
// // //  * the game loop. This draws all tiles on the screen, followed by the status
// // //  * bars. Unless init is nonzero, this function will optimize drawing by only
// // //  * drawing tiles that have changed from the previous frame.
// // //  */
// // //
// //
// // void draw_game(int init)
// // {
// //     // Draw game border first
// //     if(init) draw_border();
// //     //uLCD.locate(0,15);
// //     //uLCD.printf("%d",Player.menu);
// // //    uLCD.printf("%d,: %d",Player.lives,Player.HP);
// //     // Iterate over all visible map tiles
// //     for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
// //     {
// //         for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
// //         {
// //             // Here, we have a given (i,j)
// //
// //             // Compute the current map (x,y) of this tile
// //             int x = i + Player.x;
// //             int y = j + Player.y;
// //
// //             // Compute the previous map (px, py) of this tile
// //             int px = i + Player.px;
// //             int py = j + Player.py;
// //
// //             // Compute u,v coordinates for drawing
// //             int u = (i+5)*11 + 3;
// //             int v = (j+4)*11 + 15;
// //
// //             // Figure out what to draw
// //             DrawFunc draw = NULL;
// //             //DrawMovingFunc drawMove = NULL;
// //             //int state;
// //             if (init && i == 0 && j == 0) // Only draw the player on init
// //             {
// //                 if(x == 20 && y == 28)
// //                 draw_door15(u,v);
// //                 else
// //                 draw_player(u, v, Player.has_key);
// //                 if(x > 19)Player.has_key = 0;
// //                 continue;
// //             }
// //             else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
// //             {
// //                 MapItem* curr_item = get_here(x, y);
// //                 MapItem* prev_item = get_here(px, py);
// //
// //                 if (init || curr_item != prev_item) // Only draw if they're different
// //                 {
// //                     if (curr_item) // There's something here! Draw it
// //                     {
// //
// //                         draw = curr_item->draw;
// //                     }
// //                     else // There used to be something, but now there isn't
// //                     {
// //                         draw = draw_nothing;
// //                     }
// //                     if(Player.menu)Player.menu = false;
// //                 }
// //             }
// //             else if (init) // If doing a  but we're out of bounds, draw the walls.
// //             {
// //                 draw = draw_wall;
// //             }
// //
// //             // Actually draw the tile
// //             if (draw) draw(u, v);
// //         }
// //     }
// //
// //     // Draw status bars
// //     draw_lower_status(Player.lives,Player.HP);
// //     draw_upper_status(Player.x,Player.y,Player.px,Player.py);
// //
// //     if(Player.applesGot && Player.ncp1Meeting)
// //     {
// //         uLCD.locate(2,0);
// //         uLCD.printf("%d",Player.applesGot);
// //     }
// //     if(Player.ncp2Meeting)
// //     {
// //         checkBook(Player.has_book);
// //     }
// //
// // }
// //
// //
// // // void draw_game(int init)
// // // {
// // //     // Draw game border first
// // //     if(init) draw_border();
// // //     // Iterate over all visible map tiles
// // //     for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
// // //     {
// // //         for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
// // //         {
// // //             // Here, we have a given (i,j)
// // //
// // //             // Compute the current map (x,y) of this tile
// // //             int x = i + Player.x;
// // //             int y = j + Player.y;
// // //
// // //             // Compute the previous map (px, py) of this tile
// // //             int px = i + Player.px;
// // //             int py = j + Player.py;
// // //
// // //             // Compute u,v coordinates for drawing
// // //             int u = (i+5)*11 + 3;
// // //             int v = (j+4)*11 + 15;
// // //
// // //             // Figure out what to draw
// // //             DrawFunc draw = NULL;
// // //             if (init && i == 0 && j == 0) // Only draw the player on init
// // //             {
// // //                 draw_player(u, v, Player.has_key);
// // //                 continue;
// // //             }
// // //             else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
// // //             {
// // //                 MapItem* curr_item = get_here(x, y);
// // //                 MapItem* prev_item = get_here(px, py);
// // //                 if (init || curr_item != prev_item) // Only draw if they're different
// // //                 {
// // //                     if (curr_item) // There's something here! Draw it
// // //                     {
// // //                         draw = curr_item->draw;
// // //                     }
// // //                     else // There used to be something, but now there isn't
// // //                     {
// // //                         draw = draw_nothing;
// // //                     }
// // //                 }
// // //             }
// // //             else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
// // //             {
// // //                 draw = draw_wall;
// // //             }
// // //
// // //             // Actually draw the tile
// // //             if (draw)
// // //             draw(u, v);
// // //         }
// // //     }
// // //
// // //     // Draw status bars
// // //     draw_upper_status(Player.health, Player.x, Player.y);
// // //     draw_lower_status();
// // // }
// // //
// //
// //
// //
// // void init_main_map()
// // {
// //     // "Random" plants
// //     Map* map = set_active_map(0);
// //
// //     for(int i = map_width() + 7; i < map_area(); i += 39)
// //     {
// //         add_tree(i / map_width(), i % map_width());
// //     }
// //
// //     pc.printf("Adding walls!\r\n");
// //     add_wall(0,              0,              HORIZONTAL, map_width());
// //     add_wall(0,              map_height()-1, HORIZONTAL, map_width());
// //     add_wall(0,              0,              VERTICAL,   map_height());
// //     add_wall(map_width()-1,  0,              VERTICAL,   map_height());
// //     add_wall(map_width()-(2*(map_width()/3)-1),0, VERTICAL, (map_height()/2)+2);
// //     add_wall(map_width()-(2*(map_width()/3)-1),30,VERTICAL,map_height() - (map_height()/2)+2);
// //     pc.printf("Walls done!\r\n");
// //
// //     add_door1(19,28);
// //     add_ncp1(10,10);
// //     add_ncp2(31,19);
// //     add_house(29,15);
// //
// //     //added Walls
// //         add_wall(0, 0, HORIZONTAL, map_width());
// //          add_wall(0, map_height()-1, HORIZONTAL, map_width());
// //          add_wall(0, 0,  VERTICAL,   map_height());
// //          add_wall(map_width()-1,  0, VERTICAL,   map_height());
// //          add_wall(map_width()/2,  0, VERTICAL,    map_height());
// //          add_wall(10, 1, VERTICAL, 6);
// //          add_door(10,16);
// //          add_wall(10, 1, VERTICAL, 6);
// //          add_wall(10,8, VERTICAL, 6);
// //          add_door(10,7);
// //          add_wall(10,9,HORIZONTAL, 5);
// //          add_wall(10,14,VERTICAL, 3);
// //          add_wall(10,17,HORIZONTAL, 4);
// //          add_wall(14,17,VERTICAL, 17);
// //          add_wall(15,9, HORIZONTAL, 3);
// //          add_wall(18,9,VERTICAL,6);
// //          add_wall(24,9,HORIZONTAL, -14);
// //          add_wall(14,33, VERTICAL,6);
// //          add_wall(14,40, VERTICAL,9);
// //          add_wall(10,14,HORIZONTAL,5);
// //          add_trooper(17,11);
// //          add_NPC(15,12);
// //          add_turret(23,5);
// //          add_trooper(10,29);
// //          add_trooper(5,4);
// //          add_trooper(46,40);
// //          add_wall(26,18,HORIZONTAL,15);
// //          add_door(16,21);
// //          add_door(22,0);
// //          draw_nothing(22,0);
// //
// //
// //     print_map();
// // }
// //
// //
// //
// // // void map2()
// // // {
// // //     Map* map = set_active_map(1);
// // //     add_wall(0,              0,              HORIZONTAL, map_width());
// // //     add_wall(0,              map_height()-1, HORIZONTAL, map_width());
// // //     add_wall(0,              0,              VERTICAL,   map_height());
// // //     add_wall(map_width()-1,  0,              VERTICAL,   map_height());
// // //     add_house(1,7);
// // //
// // //     for(int i = 5; i < 9; i++){
// // //         add_fire(i,4);
// // //     }
// // //     for(int i = 1; i < 5; i++){
// // //         add_fire(i,2);
// // //     }
// // //     add_book(1,1);
// // //
// // //     print_map();
// // // }
// //
// //
// //
// //
// // void map2()
// // {
// //
// //      Map* map2 = set_active_map(1);
// //      for(int i = map_width() + 3; i < map_area(); i += 39)
// //      {
// //          //add_plant2(i % map_width(), i / map_width());
// //          add_plant(i % map_width(), i / map_width());
// //      }
// //
// //      add_wall(0, 0, HORIZONTAL, map_width());
// //      add_wall(0, map_height()-1, HORIZONTAL, map_width());
// //      add_wall(0, 0,  VERTICAL,   map_height());
// //      add_wall(map_width()-1,  0, VERTICAL,   map_height());
// //      add_wall(map_width()/2,  0, VERTICAL,    map_height());
// //      add_wall(10, 1, VERTICAL, 6);
// //      add_door(10,16);
// //      add_wall(10, 1, VERTICAL, 6);
// //      add_wall(10,8, VERTICAL, 6);
// //      add_door(10,7);
// //      add_wall(10,9,HORIZONTAL, 5);
// //      add_wall(10,14,VERTICAL, 3);
// //      add_wall(10,17,HORIZONTAL, 4);
// //      add_wall(14,17,VERTICAL, 17);
// //      add_wall(15,9, HORIZONTAL, 3);
// //      add_wall(18,9,VERTICAL,6);
// //      add_wall(24,9,HORIZONTAL, -14);
// //      add_wall(14,33, VERTICAL,6);
// //      add_wall(14,40, VERTICAL,9);
// //      add_wall(10,14,HORIZONTAL,5);
// //      add_door(49,13);
// //      //draw_nothing(49, 13);
// //
// //
// //      add_wall(30,30,VERTICAL,-4);
// //      add_wall(30,26,HORIZONTAL,5);
// //      add_wall(35,26,VERTICAL,6);
// //      add_wall(35,20,HORIZONTAL,3);
// //      add_wall(38,20,VERTICAL,17);
// //      add_wall(38,37,HORIZONTAL,2);
// //      add_wall(40,37,VERTICAL,-17);
// //      add_wall(40,20,HORIZONTAL,4);
// //      add_wall(44,20,VERTICAL,4);
// //      add_wall(44,24,HORIZONTAL,4);
// //      add_wall(40,18,VERTICAL,7);
// //      add_wall(40,25,HORIZONTAL,-8);
// //      add_wall(32,25,VERTICAL,-4);
// //       add_wall(32,21,HORIZONTAL,-4);
// //       add_wall(28,21,VERTICAL,7);
// //       add_wall(28,28,HORIZONTAL,10);
// //       add_wall(38,28,VERTICAL,3);
// //       add_wall(38,31,HORIZONTAL,4);
// //      print_map();
// //
// // }
// //
// //
// //
// //
// // //
// // // void init_main_map()
// // // {
// // //
// // //      Map* map = set_active_map(0);
// // //      for(int i = map_width() + 3; i < map_area(); i += 39)
// // //      {
// // //          add_plant(i % map_width(), i / map_width());
// // //      }
// // //
// // //
// // //
// // //
// // //
// // //      add_wall(0, 0, HORIZONTAL, map_width());
// // //      add_wall(0, map_height()-1, HORIZONTAL, map_width());
// // //      add_wall(0, 0,  VERTICAL,   map_height());
// // //      add_wall(map_width()-1,  0, VERTICAL,   map_height());
// // //      add_wall(map_width()/2,  0, VERTICAL,    map_height());
// // //      add_wall(10, 1, VERTICAL, 6);
// // //      add_door(10,16);
// // //      add_wall(10, 1, VERTICAL, 6);
// // //      add_wall(10,8, VERTICAL, 6);
// // //      add_door(10,7);
// // //      add_wall(10,9,HORIZONTAL, 5);
// // //      add_wall(10,14,VERTICAL, 3);
// // //      add_wall(10,17,HORIZONTAL, 4);
// // //      add_wall(14,17,VERTICAL, 17);
// // //      add_wall(15,9, HORIZONTAL, 3);
// // //      add_wall(18,9,VERTICAL,6);
// // //      add_wall(24,9,HORIZONTAL, -14);
// // //      add_wall(14,33, VERTICAL,6);
// // //      add_wall(14,40, VERTICAL,9);
// // //      add_wall(10,14,HORIZONTAL,5);
// // //      add_trooper(17,11);
// // //      add_NPC(15,12);
// // //      add_turret(23,5);
// // //      add_trooper(10,29);
// // //      add_trooper(5,4);
// // //      add_trooper(46,40);
// // //      add_wall(26,18,HORIZONTAL,15);
// // //      add_door(16,21);
// // //      add_door(22,0);
// // //      //draw_nothing(22,0);
// // //
// // //
// // //      //  const char* startmap = "K N";
// // //      //
// // //      //
// // //      //  for (int x = 0; x < map_width(); x++)
// // //      //  {
// // //      //    for (int y = 0; y < map_height(); y++)
// // //      //    {
// // //      //        add_plant(i % map_width(), i / map_width());
// // //      //        int XY_KEY = x + y * map_width();
// // //      //        if (startmap[XY_KEY] == 'K') add_key(9,8);
// // //      //        else if (startmap[XY_KEY] == 'N') add_NPC(11,10);
// // //      //        else continue;
// // //      //      }
// // //      // }
// // //
// // //      //add_key(4,20);W
// // //
// // //
// // //
// // //      //print_map();
// // //
// // // }
// // //
// // //
// // // void map2()
// // // {
// // //
// // //      Map* map2 = set_active_map(1);
// // //      for(int i = map_width() + 3; i < map_area(); i += 39)
// // //      {
// // //          add_plant2(i % map_width(), i / map_width());
// // //      }
// // //
// // //      add_wall(0, 0, HORIZONTAL, map_width());
// // //      add_wall(0, map_height()-1, HORIZONTAL, map_width());
// // //      add_wall(0, 0,  VERTICAL,   map_height());
// // //      add_wall(map_width()-1,  0, VERTICAL,   map_height());
// // //      add_wall(map_width()/2,  0, VERTICAL,    map_height());
// // //      add_wall(10, 1, VERTICAL, 6);
// // //      add_door(10,16);
// // //      add_wall(10, 1, VERTICAL, 6);
// // //      add_wall(10,8, VERTICAL, 6);
// // //      add_door(10,7);
// // //      add_wall(10,9,HORIZONTAL, 5);
// // //      add_wall(10,14,VERTICAL, 3);
// // //      add_wall(10,17,HORIZONTAL, 4);
// // //      add_wall(14,17,VERTICAL, 17);
// // //      add_wall(15,9, HORIZONTAL, 3);
// // //      add_wall(18,9,VERTICAL,6);
// // //      add_wall(24,9,HORIZONTAL, -14);
// // //      add_wall(14,33, VERTICAL,6);
// // //      add_wall(14,40, VERTICAL,9);
// // //      add_wall(10,14,HORIZONTAL,5);
// // //      add_door(49,13);
// // //      //draw_nothing(49, 13);
// // //
// // //
// // //      add_wall(30,30,VERTICAL,-4);
// // //      add_wall(30,26,HORIZONTAL,5);
// // //      add_wall(35,26,VERTICAL,6);
// // //      add_wall(35,20,HORIZONTAL,3);
// // //      add_wall(38,20,VERTICAL,17);
// // //      add_wall(38,37,HORIZONTAL,2);
// // //      add_wall(40,37,VERTICAL,-17);
// // //      add_wall(40,20,HORIZONTAL,4);
// // //      add_wall(44,20,VERTICAL,4);
// // //      add_wall(44,24,HORIZONTAL,4);
// // //      add_wall(40,18,VERTICAL,7);
// // //      add_wall(40,25,HORIZONTAL,-8);
// // //      add_wall(32,25,VERTICAL,-4);
// // //       add_wall(32,21,HORIZONTAL,-4);
// // //       add_wall(28,21,VERTICAL,7);
// // //       add_wall(28,28,HORIZONTAL,10);
// // //       add_wall(38,28,VERTICAL,3);
// // //       add_wall(38,31,HORIZONTAL,4);
// // //
// // //
// // //
// // //      print_map();
// // //
// // // }
// // //
// //
// //
// //
// // /**
// //  * Program entry point! This is where it all begins.
// //  * This function orchestrates all the parts of the game. Most of your
// //  * implementation should be elsewhere - this holds the game loop, and should
// //  * read like a road map for the rest of the code.
// //  */
// // int main()
// // {
// //     // First things first: initialize hardware
// //     ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
// //
// //     //Start screen
// //     DisplayStartGame();
// //     uLCD.cls();
// //
// //     // Initialize the maps
// //     maps_init();
// //     init_main_map();
// //
// //     // Initialize game state
// //     set_active_map(0);
// //     if(DEBUG)
// //     Player.x = Player.y = 25;///////////////////////////////////// 5
// //     else
// //     Player.x = Player.y = 5;
// //     Player.lives = 3;
// //     Player.HP = 100;
// //     Player.map = 0;
// //     Player.menu = false;
// //     Player.quest = 0;
// //     // Initial drawing
// //     draw_game(true);
// //     Player.ghostMode = false;
// //
// //     // Main game loop
// //     while(1)
// //     {
// //         // Timer to measure game update speed
// //         Timer t; t.start();
// //         int action = get_action(read_inputs());
// //         //while(action != ACTION_BUTTON || action != MENU_BUTTON) action = get_action(read_inputs());
// //         int gameUpdate = update_game(action);
// //         if((get_here(Player.x,Player.y))->type == FIRE)
// //         {
// //
// //             Player.HP = Player.HP-25;
// //             mySpeaker.PlayNote(100.0,0.25,0.1);
// //             if(Player.HP < 1)
// //             {
// //                 Player.HP = 100;
// //                 Player.lives--;
// //                 if(Player.lives < 1)gameUpdate = FAIL;
// //             }
// //
// //         }
// //
// //
// //         if(gameUpdate == GAME_OVER)
// //         {
// //             DisplayGameOver();
// //             break;
// //         }else draw_game(gameUpdate);
// //         if(gameUpdate == FAIL)
// //         {
// //             DisplayGameFail();
// //             break;
// //         }
// //         // Actuall do the game update:
// //         // 1. Read inputs
// //         // 2. Determine action (get_action)
// //         // 3. Update game (update_game)
// //         // 3b. Check for game over
// //         // 4. Draw frame (draw_game)
// //
// //         // 5. Frame delay
// //         t.stop();
// //         int dt = t.read_ms();
// //         if (dt < 100) wait_ms(100 - dt);
// //     }
// // }
// //
// //
// //
// //
// // //
// // //
// // // /**
// // //  * Program entry point! This is where it all begins.
// // //  * This function orTURRETrates all the parts of the game. Most of your
// // //  * implementation should be elsewhere - this holds the game loop, and should
// // //  * read like a road map for the rest of the code.
// // //  */
// // //
// // // int main()
// // // {
// // //     GameInputs input;
// // //     int action;
// // //     int updated;
// // //
// // //     // First things first: initialize hardware
// // //     ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
// // //
// // //     // Initialize the maps
// // //     maps_init();
// // //     init_main_map();
// // //     //mapq();
// // //     set_active_map(0);
// // //     Player.x = Player.y = 5;
// // //
// // //     // Splash/start screen
// // //     uLCD.filled_rectangle(0, 0, 127, 127, BLACK);
// // //     uLCD.color(WHITE);
// // //     uLCD.cls();
// // //     uLCD.locate(3, 3);
// // //     uLCD.text_width(2);
// // //     uLCD.text_height(2);
// // //     uLCD.printf("Intro");
// // //     wait(2);
// // //     uLCD.cls();
// // //     uLCD.locate(2, 2);
// // //     uLCD.text_width(2);
// // //     uLCD.text_height(2);
// // //     uLCD.printf("The");
// // //     uLCD.locate(1,4);
// // //     uLCD.text_width(2);
// // //     uLCD.text_height(2);
// // //     uLCD.printf("Transformers!");
// // //     // mySpeaker.PlayNote(400.0, .25, .1);
// // //     // wait(0.2);
// // //     // mySpeaker.PlayNote(200.0, .25, .1);
// // //     // wait(0.2);
// // //     // mySpeaker.PlayNote(100.0, .25, .1);
// // //     wait(3);
// // //     uLCD.cls();
// // //     GameInputs in = read_inputs();
// // //     while (in.b1)
// // //     {
// // //         in = read_inputs();
// // //         continue;
// // //     }
// // //     uLCD.cls();
// // //
// // //     // Initial drawing
// // //     draw_game(true);
// // //
// // //
// // //     // Initial player data
// // //     Player.omni = false;
// // //     Player.health = 3;
// // //     Player.has_key = false;
// // //     Player.has_gun = false;
// // //     Player.state = 0;
// // //
// // //     // Main game loop
// // //     while(1)
// // //     {
// // //         // Timer to measure game update speed
// // //         Timer t; t.start();
// // //
// // //         // Actually do the game update:
// // //
// // //         // 1. Read inputs
// // //         input = read_inputs();
// // //         // 2. Determine action (get_action)
// // //         action = get_action(input);
// // //         // 3. Update game (update_game)
// // //         updated = update_game(action);
// // //         // 3b. Check for game over / game win conditions
// // //         if (updated == 1)
// // //         {
// // //             draw_gameover();
// // //             break;
// // //         } else if (updated == 3)
// // //         {
// // //             draw_gamewin();
// // //             break;
// // //         }
// // //         // 4. Draw frame (draw_game)
// // //         draw_game(updated);
// // //         // 5. Frame delay
// // //         t.stop();
// // //         int dt = t.read_ms();
// // //         if (dt < 100) wait_ms(100 - dt);
// // //     }
// // // }
