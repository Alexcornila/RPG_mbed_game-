#include "graphics.h"

#include "globals.h"



void draw_player(int u, int v, int key)
{
    //uLCD.filled_rectangle(u, v, u+11, v+11, RED);
    const char *mainCharacter = "XXXYYBYYXXXXXXYBBBYXXXXXXYBBBYXXXXXYYYYYYYXXXXYXXYXXYXXXXBXXYXXBXXXXYXXYXXYXXXXYXYXYXYXXXXXXYXYXXXXXXXXYXYXXXXXXXXBXBXXXX";;
    draw_img(u, v, mainCharacter);
}

void draw_NPC(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
    const char *npcSprite = "XXXRRWRRXXXXXXRRWRRXXXXXRRWWWRRXXRRRRRRRRRRRRRXXXRXXXRRWWXXXRXXXWWRRXXXRXXXRRRRXRRXRRXRRXXXRRXRRXXXXXXRRXRRXXXXXXWWXWWXXX";
    draw_img(u, v, npcSprite);
}

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN
#define ORANGE 0xFF5722
#define DBROWN 0x4E342E
#define LPINK  0xEF9A9A
#define MGREY  0x808080
#define DBLUE  0x000080
#define SIENNA 0XA0522D
#define DBROWN 0x4E342E

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 's') colors[i] = SIENNA;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = DIRT;
        else if (img[i] == '5') colors[i] = LGREY;
        else if (img[i] == '4') colors[i] = MGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'O') colors[i] = ORANGE;
        else if (img[i] == 'b') colors[i] = DBROWN;
        else if (img[i] == 'P') colors[i] = LPINK;
        else if (img[i] == 'B') colors[i] = BLUE;
        else if (img[i] == 'I') colors[i] = DBLUE;
        else if (img[i] == 'W') colors[i] = WHITE;
        else if (img[i] == 'b') colors[i] = DBROWN;

        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{

    const char *wallSprite = "ssXXsssXsssssXXsssXsssXXXXXXXXXsssssssXssssXXssssXssssssXXXXXXXXXXssXssssXXssssXssssXXssssXXXXXXXXXsssssXssssssXXXXXXXXXX";
    draw_img(u, v, wallSprite);
}

void draw_plant(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
    const char *treeSprite = "XXXGGGGXXXXXXGGGGGGXXXXGGGGGGGGXXXGGGGGGGGXXXGGGbbGGGXXXGGGbbGGGXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXX";
    draw_img(u, v, treeSprite);
}

void draw_chest(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, YELLOW);
    const char *chestSprite = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXYYYYYYYYYXYEEEEEEEEEYYEEEYYYEEEYYYYYY3YYYYYYEEY343YEEYYEEYYYYYEEYYEEEEEEEEEYYYYYYYYYYYY";
    draw_img(u, v, chestSprite);
}

void draw_key(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, YELLOW);
    const char *keySprite = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWXXXXXXXW5XYYXXXXXXWXXXYYYYYWWYYXYYXXYXYYXYYYXXXYXXYXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    draw_img(u, v, keySprite);
}

void draw_door(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
    const char *doorSprite = "55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXX55XX";
    draw_img(u, v, doorSprite);
}

void draw_heart(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, RED);
    const char *heartSprite = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
    draw_img(u, v, heartSprite);
}

void draw_heartempty(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, RED);
    const char *heartemptySprite = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    draw_img(u, v, heartemptySprite);
}

void draw_spikes(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, LGREY);
    const char *spikesSprite = "XX5XXXXX5XXX555XXX555XXXXXX5XXXXXXXXX555XXXXXX5XXXXX5XXX555XXX555XXXXXX5XXXXXXXXX555XXXXXX5XXXXX5XXX555XXX555XXXXXXXXXXXX";
    draw_img(u, v, spikesSprite);
}

//void draw_water(int u, int v)
//{
//    //uLCD.filled_rectangle(u, v, u+10, v+10, DBLUE);
//    const char *waterSprite = "BBBIIBBBBIBBIIBBIBBIBIIBBBBBIIBBBBBBIIBBBBIBBIIBBIBBIBIIBBBBBIIBBBBBBIIBBBBIBBIIBBIBBIBIIBBBBBIIBBBBBBIIBBBBIBBIIBBIBBIBI";
//    draw_img(u, v, waterSprite);
//}

void draw_skeleton(int u, int v)
{
    //uLCD.filled_rectangle(u, v, u+10, v+10, WHITE);
    const char *skeletonSprite = "XXXXWWWXXXXXXXXrrrXXXXXXXXWrWXXXXXXXXXWXXXXXXXXWWWWWXXXXXWXWWWXWXXXXWXWWWXWXXXXWXWWWXWXXXXXXWWWXXXXXXXXWXWXXXXXXXXWXWXXXX";
    draw_img(u, v, skeletonSprite);
}

void draw_spear(int u, int v)
{
    const char *spearSprite = "XXXXX3XXXXXXXXXX3XXXXXXXXXXYXXXXXXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXX";
    draw_img(u, v, spearSprite);
}

void draw_upper_status(int health, int x, int y)
{
    // Draw bottom border of status bar
    //uLCD.line(0, 9, 127, 9, GREEN);
    for (int i = 0; i < 3; i++) {
        if (health > 0) {
            draw_heart(11*i, 0);
            health--;
        } else {
            draw_heartempty(11*i, 0);
        }
    }
    /*draw_heart(0, 0);
    draw_heart(11, 0);
    draw_heart(22, 0);*/
    // Add other status info drawing code here
    // figure out how to determine player position
    // clear old values first
    uLCD.filled_rectangle(90, 0, 127, 10, BLACK);
    uLCD.locate(50, 0);
    uLCD.color(WHITE);
    uLCD.printf("%d,%d", x, y);
}

void draw_lower_status()
{
    // Draw top border of status bar
    //uLCD.line(0, 118, 127, 118, GREEN);

    // Add other status info drawing code here

}

void draw_border()
{
    uLCD.filled_rectangle(0,     11, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 116, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 116, WHITE); // Right
}


void draw_gameover()
{
    uLCD.locate(0, 15);
    uLCD.printf("You lost!");
}

void draw_gamewin()
{
    uLCD.locate(0, 15);
    uLCD.printf("You won!!");
}


void draw_turret(int u, int v)
{

    const char *turret = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXWWXXXXWWWWWWWWXXXXXXXWWWWXXXXXXWWWWWWWXWWWWWXXWWWWWWWWWXXWWWWWWWWWWWWWWWWXXWWWWWXXW";
    draw_img(u, v, turret);
}


void draw_life(int u, int v)
{

    const char *life = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
    draw_img(u, v, life);
}

void draw_damage(int u, int v)
{

    const char *damage = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
    draw_img(u, v, damage);
}


void draw_trooper(int u, int v)
{

                                //0123456789011234567890112345678901123456789011
    const char *trooper = "XXXXWWWXXXXXXXXrrrXXXXXXXXWrWXXXXXXXXXWXXXXXXXXWWWWWXXXXXWXWWWXWXXXXWXWWWXWXXXXWXWWWXWXXXXXXWWWXXXXXXXXWXWXXXXXXXXWXWXXXX";
    draw_img(u, v, trooper);
}






// #include "graphics.h"
//
// #include "globals.h"
//
//
//
// void draw_player(int u, int v, int key)
// {
//
//     const char *mainCharacter = "XXXYYBYYXXXXXXYBBBYXXXXXXYBBBYXXXXXYYYYYYYXXXXYXXYXXYXXXXBXXYXXBXXXXYXXYXXYXXXXYXYXYXYXXXXXXYXYXXXXXXXXYXYXXXXXXXXBXBXXXX";
//     draw_img(u, v, mainCharacter);
// }
//
// void draw_NPC(int u, int v)
// {
//
//     const char *npcSprite = "XXXRRWRRXXXXXXRRWRRXXXXXRRWWWRRXXRRRRRRRRRRRRRXXXRXXXRRWWXXXRXXXWWRRXXXRXXXRRRRXRRXRRXRRXXXRRXRRXXXXXXRRXRRXXXXXXWWXWWXXX";
//     draw_img(u, v, npcSprite);
// }
// #define BRED   0xff0000
// #define RED2   0x6a1e10
// #define YELLOW 0xFFFF00
// #define SIENNA 0XA0522D
// #define BROWN  0xD2691E
// #define DIRT   BROWN
// #define ORANGE 0xFF5722
// #define DBROWN 0x4E342E
// #define LPINK  0xEF9A9A
// #define MGREY  0x808080
// #define DBLUE  0x000080
//
//
//
// void draw_img(int u, int v, const char* img)
// {
//     int colors[11*11];
//     for (int i = 0; i < 11*11; i++)
//     {
//         if (img[i] == 'R') colors[i] = RED;
//
//         else if (img[i] == 'r') colors[i] = BRED;
//         else if (img[i] == 'Y') colors[i] = YELLOW;
//         else if (img[i] == 'G') colors[i] = GREEN;
//         else if (img[i] == 'b') colors[i] = SIENNA;
//         else if (img[i] == 'D') colors[i] = DIRT;
//         else if (img[i] == '5') colors[i] = LGREY;
//         else if (img[i] == '4') colors[i] = MGREY;
//         else if (img[i] == '3') colors[i] = DBROWN;
//         else if (img[i] == 'O') colors[i] = ORANGE;
//         else if (img[i] == 'E') colors[i] = DBROWN;
//         else if (img[i] == 'P') colors[i] = LPINK;
//         else if (img[i] == 'B') colors[i] = BLUE;
//         else if (img[i] == 'I') colors[i] = DBLUE;
//         else if (img[i] == 'W') colors[i] = WHITE;
//         else if (img[i] ==  'X') colors[i]= BLACK;
//     }
//     uLCD.BLIT(u, v, 11, 11, colors);
//     wait_us(250);
// }
//
// void draw_nothing(int u, int v)
// {
//     // Fill a tile with blackness
//     uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
//
//
// }
//
//
//
// // void draw_nothing2(int u, int v)
// // {
// //     // Fill a tile with blackness
// //     //uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
// //     const char* nothing = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// //
// // }
//
//
//
//
//
// void draw_wall(int u, int v)
// {
//
//     const char* wall = "X3333333333X3333333333X3333333XXXXXXXXXXX333333X333333333333333333333X3333XXXXXXXXXXX3333X3333333333X3333333333X33333";
//     draw_img(u, v, wall);
// }
//
// void draw_wall2(int u, int v)
// {
//
//
//     const char* wall2 = "XbbbbbbbbbbXbbbbbbbbbbXbbbbbbbXXXXXXXXXXXbbbbbbXbbbbbbbbbbbbbbbbbbbbbXbbbbXXXXXXXXXXXbbbbXbbbbbbbbbbXbbbbbbbbbbXbbbbb";
//     draw_img(u, v, wall2);
// }
//
//
// void draw_plant(int u, int v)
// {
//     // use const char to reduce memory ussage
//     const char* tree = "XXXGGGGXXXXXXGGGGGGXXXXGGGGGGGGXXXGGGGGGGGXXXGGGbbGGGXXXGGGbbGGGXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXX";
//
//     draw_img(u, v, tree);
// }
//
//
// void draw_plant2(int u, int v)
// {
//     // use const char to reduce memory ussage
//     const char* tree2 = "XXXBBBBXXXXXXBBBBBBXXXXBBBBBBBBXXXBBBBBBBBXXXBBBbbBBBXXXBBBbbBBBXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXX";
//
//     draw_img(u, v, tree2);
// }
//
// void draw_turret(int u, int v)
// {
//
//     const char *turret = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXWWXXXXWWWWWWWWXXXXXXXWWWWXXXXXXWWWWWWWXWWWWWXXWWWWWWWWWXXWWWWWWWWWWWWWWWWXXWWWWWXXW";
//     draw_img(u, v, turret);
// }
//
// void draw_key(int u, int v)
// {
//
//     const char *key = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWXXXXXXXW5XYYXXXXXXWXXXYYYYYWWYYXYYXXYXYYXYYYXXXYXXYXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
//     draw_img(u, v, key);
// }
//
// void draw_trans(int u, int v)
// {
//
//     const char *trans = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWWWWWWWXXXXXXWWWWWWWXXXXXXWXXXYYYYYWWWWWWYYXXYXYYXWWWXXXYXXWWWWXXXXXXXXXXXXXXXXXXWWWWWWXXXX";
//     draw_img(u, v, trans);
// }
//
//
//
//
//
// void draw_door(int u, int v)
// {
//
//     const char *door = "55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXX55XX";
//     draw_img(u, v, door);
// }
//
// void draw_life(int u, int v)
// {
//
//     const char *life = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
//     draw_img(u, v, life);
// }
//
// void draw_damage(int u, int v)
// {
//
//     const char *damage = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
//     draw_img(u, v, damage);
// }
//
//
// void draw_trooper(int u, int v)
// {
//
//                                 //0123456789011234567890112345678901123456789011
//     const char *trooper = "XXXXWWWXXXXXXXXrrrXXXXXXXXWrWXXXXXXXXXWXXXXXXXXWWWWWXXXXXWXWWWXWXXXXWXWWWXWXXXXWXWWWXWXXXXXXWWWXXXXXXXXWXWXXXXXXXXWXWXXXX";
//     draw_img(u, v, trooper);
// }
//
// void draw_gun(int u, int v)
// {
//     const char *gun = "X33333XXXXXXXXXX3333XXXXXXXYXXXXXXXXXXEXXXXXXXXXXEX333XXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXX3333XXXEXXXXXXXXXXEXXXXXXXXXXEXXXXX";
//     draw_img(u, v, gun);
// }
//
// void draw_upper_status(int health, int x, int y)
// {
//     // Draw bottom border of status bar // add some stuff here
//     //uLCD.line(0, 9, 127, 9, GREEN);
//     for (int i = 0; i < 3; i++)
//     {
//         if (health > 0)
//         {
//             draw_life(11*i, 0);
//             health--;
//         } else
//         {
//             draw_damage(11*i, 0);
//         }
//     }
//
//     uLCD.filled_rectangle(90, 0, 127, 10, BLACK);
//     uLCD.locate(50, 0);
//     uLCD.color(WHITE);
//     uLCD.printf("%d,%d", x, y);
// }
//
// void draw_lower_status()
// {
//     // Draw top border of status bar
//     uLCD.line(0, 118, 127, 118, GREEN);
//
//     // Add other status info drawing code here
//
// }
//
// void draw_border()
// {
//     uLCD.filled_rectangle(0,     10, 127,  14, MGREY); // Top
//     uLCD.filled_rectangle(0,    10,   2, 114, MGREY); // Left
//     uLCD.filled_rectangle(0,   114, 127, 117, MGREY); // Bottom
//     uLCD.filled_rectangle(124,  14, 127, 117, MGREY); // Right
// }
//
//
// void draw_gameover()
// {
//     uLCD.locate(1, 15);
//     uLCD.printf("GAME OVER!");
// }
//
// void draw_gamewin()
// {
//     uLCD.locate(1, 15);
//     uLCD.printf("You won!");
// }
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
//
//
// // #include "graphics.h"
// // #include "globals.h"
// //
// //
// //
// //
// // void draw_player(int u, int v, int key)
// // {
// //
// //     const char *mainCharacter = "XXXYYBYYXXXXXXYBBBYXXXXXXYBBBYXXXXXYYYYYYYXXXXYXXYXXYXXXXBXXYXXBXXXXYXXYXXYXXXXYXYXYXYXXXXXXYXYXXXXXXXXYXYXXXXXXXXBXBXXXX";
// //     draw_img(u, v, mainCharacter);
// // }
// //
// //
// // void draw_npc1(int u, int v)
// // {
// //
// //     const char *npcSprite = "XXXRRWRRXXXXXXRRWRRXXXXXRRWWWRRXXRRRRRRRRRRRRRXXXRXXXRRWWXXXRXXXWWRRXXXRXXXRRRRXRRXRRXRRXXXRRXRRXXXXXXRRXRRXXXXXXWWXWWXXX";
// //     draw_img(u, v, npcSprite);
// // }
// //
// //
// // #define BRED   0xff0000
// // #define RED2   0x6a1e10
// // #define YELLOW 0xFFFF00
// // #define SIENNA 0XA0522D
// // #define BROWN  0xD2691E
// // #define DIRT   BROWN
// // #define ORANGE 0xFF5722
// // #define DBROWN 0x4E342E
// // #define LPINK  0xEF9A9A
// // #define MGREY  0x808080
// // #define DBLUE  0x000080
// //
// //
// //
// // #define YELLOW 0xFFFF00
// // #define BROWN  0xD2691E
// // #define DIRT   BROWN
// // #define _ 0x000000 //BLACK
// //
// // #define X 0xFFFFFF //WHITE
// // #define G 0x00FF00 //GREEN
// // #define F 0x228B22 //Forest GREEN
// // #define V 0X006400 //DARK GREEN
// // #define R 0xFF0000 //RED
// // #define B 0x0000FF //BLUE
// // #define O 0xFFA500 //ORANGE
// // #define Y 0xFFFF00 //YELLOW
// // #define D 0xD2691E //BROWN
// // #define L 0xD3D3D3 // Light Gray
// // #define W 0x808080 // Gray
// //
// //
// // //trooper
// // void draw_npc2(int u, int v)
// // {
// //     //uLCD.BLIT(u, v, 11, 11, NPC2_sprite);
// //     const char *npcSprite = "XXXBBYBBXXXXXXBBYBBXXXXXBBYYYBBXXBBBBBBBBBBBBBXXXBXXXBBYYXXXBXXXYYBBXXXBXXXBBBBXBBXBBXBBXXXBBXBBXXXXXXBBXBBXXXXXXYYXYYXXX";
// //     draw_img(u, v, npcSprite);
// //
// // }
// //
// //
// // // void draw_img(int u, int v, const char* img)
// // // {
// // //     int colors[11*11];
// // //     for (int i = 0; i < 11*11; i++)
// // //     {
// // //         if (img[i] == 'R') colors[i] = RED;
// // //
// // //         else if (img[i] == 'r') colors[i] = BRED;
// // //         else if (img[i] == 'Y') colors[i] = YELLOW;
// // //         else if (img[i] == 'G') colors[i] = GREEN;
// // //         else if (img[i] == 'b') colors[i] = SIENNA;
// // //         else if (img[i] == 'D') colors[i] = DIRT;
// // //         else if (img[i] == '5') colors[i] = LGREY;
// // //         else if (img[i] == '4') colors[i] = MGREY;
// // //         else if (img[i] == '3') colors[i] = DBROWN;
// // //         else if (img[i] == 'O') colors[i] = ORANGE;
// // //         else if (img[i] == 'E') colors[i] = DBROWN;
// // //         else if (img[i] == 'P') colors[i] = LPINK;
// // //         else if (img[i] == 'B') colors[i] = BLUE;
// // //         else if (img[i] == 'I') colors[i] = DBLUE;
// // //         else if (img[i] == 'W') colors[i] = WHITE;
// // //         else if (img[i] ==  'X') colors[i]= BLACK;
// // //     }
// // //     uLCD.BLIT(u, v, 11, 11, colors);
// // //     wait_us(250);
// // // }
// //
// //
// // void draw_img(int u, int v, const char* img)
// // {
// //     int colors[11*11];
// //     for (int i = 0; i < 11*11; i++)
// //     {
// //         if (img[i] == 'R') colors[i] = RED;
// //
// //         else if (img[i] == 'r') colors[i] = BRED;
// //         else if (img[i] == 'Y') colors[i] = YELLOW;
// //         else if (img[i] == 'G') colors[i] = GREEN;
// //         else if (img[i] == 'b') colors[i] = SIENNA;
// //         else if (img[i] == 'D') colors[i] = DIRT;
// //         else if (img[i] == '5') colors[i] = LGREY;
// //         else if (img[i] == '4') colors[i] = MGREY;
// //         else if (img[i] == '3') colors[i] = DBROWN;
// //         else if (img[i] == 'O') colors[i] = ORANGE;
// //         else if (img[i] == 'E') colors[i] = DBROWN;
// //         else if (img[i] == 'P') colors[i] = LPINK;
// //         else if (img[i] == 'B') colors[i] = BLUE;
// //         else if (img[i] == 'I') colors[i] = DBLUE;
// //         else if (img[i] == 'W') colors[i] = WHITE;
// //         else if (img[i] ==  'X') colors[i]= BLACK;
// //     }
// //     uLCD.BLIT(u, v, 11, 11, colors);
// //     wait_us(250);
// // }
// //
// //
// //
// // void draw_nothing(int u, int v)
// // {
// //     // Fill a tile with blackness
// //     uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
// //
// // }
// //
// //
// //
// // // void draw_nothing2(int u, int v)
// // // {
// // //     // Fill a tile with blackness
// // //     //uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
// // //     const char* nothing = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// // //
// // // }
// //
// // void draw_wall(int u, int v)
// // {
// //     const char* wall = "X3333333333X3333333333X3333333XXXXXXXXXXX333333X333333333333333333333X3333XXXXXXXXXXX3333X3333333333X3333333333X33333";
// //     draw_img(u, v, wall);
// // }
// //
// //
// // void draw_wall2(int u, int v)
// // {
// //
// //     const char* wall2 = "XbbbbbbbbbbXbbbbbbbbbbXbbbbbbbXXXXXXXXXXXbbbbbbXbbbbbbbbbbbbbbbbbbbbbXbbbbXXXXXXXXXXXbbbbXbbbbbbbbbbXbbbbbbbbbbXbbbbb";
// //     draw_img(u, v, wall2);
// // }
// //
// //
// // void draw_plant(int u, int v)
// // {
// //     // use const char to reduce memory ussage
// //     const char* tree = "XXXGGGGXXXXXXGGGGGGXXXXGGGGGGGGXXXGGGGGGGGXXXGGGbbGGGXXXGGGbbGGGXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXX";
// //     draw_img(u, v, tree);
// // }
// //
// // void draw_tree(int u, int v)
// // {
// //
// //     const char* tree = "XXXGGGGXXXXXXGGGGGGXXXXGGGGGGGGXXXGGGGGGGGXXXGGGbbGGGXXXGGGbbGGGXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXX";
// //     draw_img(u, v, tree);
// //
// // }
// //
// //
// // void draw_plant2(int u, int v)
// // {
// //     // use const char to reduce memory ussage
// //     const char* tree2 = "XXXBBBBXXXXXXBBBBBBXXXXBBBBBBBBXXXBBBBBBBBXXXBBBbbBBBXXXBBBbbBBBXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXXXXXXbbXXXXX";
// //
// //     draw_img(u, v, tree2);
// // }
// //
// // //also enemy make it fire + animation
// // void draw_turret(int u, int v)
// // {
// //
// //     const char *turret = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXWWXXXXWWWWWWWWXXXXXXXWWWWXXXXXXWWWWWWWXWWWWWXXWWWWWWWWWXXWWWWWWWWWWWWWWWWXXWWWWWXXW";
// //     draw_img(u, v, turret);
// // }
// //
// //
// // void draw_key(int u, int v)
// // {
// //
// //     const char *key = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWXXXXXXXW5XYYXXXXXXWXXXYYYYYWWYYXYYXXYXYYXYYYXXXYXXYXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// //     draw_img(u, v, key);
// // }
// //
// // void draw_trans(int u, int v)
// // {
// //
// //     const char *trans = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWWWWWWWXXXXXXWWWWWWWXXXXXXWXXXYYYYYWWWWWWYYXXYXYYXWWWXXXYXXWWWWXXXXXXXXXXXXXXXXXXWWWWWWXXXX";
// //     draw_img(u, v, trans);
// // }
// //
// //
// //
// // // void draw_door(int u, int v)
// // // {
// // //
// // //     const char *door = "55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXXXXXXXXXXXXX55XX55555555555XX55XXX55XX";
// // //     draw_img(u, v, door);
// // // }
// //
// // void draw_life(int u, int v)
// // {
// //
// //     const char *life = "GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";
// //     draw_img(u, v, life);
// // }
// //
// // void draw_damage(int u, int v)
// // {
// //
// //     const char *damage = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// //     draw_img(u, v, damage);
// // }
// //
// // void draw_trooper(int u, int v)
// // {
// //                               //0123456789011234567890112345678901123456789011
// // const char *trooper = "XXXXWWWXXXXXXXXrrrXXXXXXXXWrWXXXXXXXXXWXXXXXXXXWWWWWXXXXXWXWWWXWXXXXWXWWWXWXXXXWXWWWXWXXXXXXWWWXXXXXXXXWXWXXXXXXXXWXWXXXX";
// // draw_img(u, v, trooper);
// // }
// //
// // void draw_gun(int u, int v)
// // {
// //     const char *gun = "X33333XXXXXXXXXX3333XXXXXXXYXXXXXXXXXXEXXXXXXXXXXEX333XXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXX3333XXXEXXXXXXXXXXEXXXXXXXXXXEXXXXX";
// //     draw_img(u, v, gun);
// // }
// //
// //
// // void draw_diamond(int u, int v)
// // {
// //     const char *gun = "X33333XXXXXXXXXX3333XXXXXXXYXXXXXXXXXXEXXXXXXXXXXEX333XXXXXXEXXXXXXXXXXEXXXXXXXXXXEXXX3333XXXEXXXXXXXXXXEXXXXXXXXXXEXXXXX";
// // }
// //
// // void draw_fire(int u, int v)
// // {
// //     const char *gun = "XRRRRRXXXXXXXXXXRRRRXXXXXXRRRRXXXXXXXXEXXXXXXXXXRRRRRRXXXXXXEXXXXXXXRRRRRRRXXXXXXXEXXXRRRRXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXX";
// // }
// //
// // void draw_BOOK(int u,int v)
// // {
// //     const char *gun = "XBBBBBXXXXXXXXXXBBBBXXXXXBBBBBXXXXXXXXEXXXXXXXBBBBBBBBXXXXXXEXXXXXXXBBBBBBXXXXXXXEXXXBBBBXXXEXXXXXXXXXXEXXXXXXXXXXEXXXXX";
// // }
// //
// //
// //
// //
// //
// //
// //
// //
// //
// // // void print_apple()
// // // {
// // //     //uLCD.BLIT(0, 0, 8, 10, );
// // //     const char *damage = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// // //     draw_img(u, v, damage);
// // //
// // // }
// //
// //
// //
// //
// // void print_key()
// // {
// //   int KEY_sprite[10 * 8] =
// //   {
// // _,_,_,Y,Y,_,_,_,
// // _,_,_,Y,Y,Y,Y,_,
// // _,_,_,Y,Y,Y,_,_,
// // _,_,_,Y,Y,Y,Y,_,
// // _,_,_,Y,Y,_,_,_,
// // _,_,X,Y,Y,X,_,_,
// // _,_,Y,_,_,Y,_,_,
// // _,_,Y,Y,Y,Y,_,_,
// // };
// //   uLCD.BLIT(0, 0, 8, 10, KEY_sprite);
// //
// // }
// //
// //
// // void print_book()
// // {
// //
// // int BOOK_sprite[11 * 11] =
// // {
// // F,V,F,V,F,V,F,V,F,V,V,
// // V,F,V,V,V,D,D,D,D,D,D,
// // F,V,F,V,D,B,B,B,B,D,X,
// // V,F,V,D,B,B,B,B,D,X,X,
// // F,V,D,B,B,B,B,D,X,X,D,
// // V,D,B,B,B,B,D,X,X,D,V,
// // D,D,D,D,D,D,X,X,D,V,V,
// // D,X,X,X,X,X,X,D,V,F,V,
// // D,D,D,D,D,D,D,V,F,V,V,
// // V,F,V,V,V,F,V,V,V,F,V,
// // V,F,V,F,V,F,V,F,V,F,V,
// // };
// //
// //     //uLCD.BLIT(0, 0, 8, 10, book_sprite);
// // }
// //
// // /*void print_diamond()
// // {
// //     uLCD.BLIT(2, 2, 6, 6, DiamondICON_sprite);
// // }*/
// //
// // // void checkBook(int has)
// // // {
// // //     const char *damage = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
// // //
// // // }
// //
// //
// //
// // // void print_ghost()
// // // {
// // //     uLCD.BLIT(0,122,8,10,ghostMode_sprite);
// // // }
// //
// // void clear_ghost()
// // {
// //     uLCD.filled_rectangle(0,122,10,128, BLACK);
// // }
// //
// //
// // void clear_item()
// // {
// //     uLCD.filled_rectangle(0,0,7,8, BLACK);
// // }
// //
// //
// //  void clear_item_count()
// // {
// //     uLCD.filled_rectangle(0,0,28,8, BLACK);
// // }
// //
// //
// //
// // void draw_door(int u, int v)
// // {
// //   const char *trans = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWWWWWWWXXXXXXWWWWWWWXXXXXXWXXXYYYYYWWWWWWYYXXYXYYXWWWXXXYXXWWWWXXXXXXXXXXXXXXXXXXWWWWWWXXXX";
// //   draw_img(u, v, trans);
// //
// // }
// //
// // // void draw_door12(int u, int v)
// // // {
// // //     uLCD.BLIT(u, v, 11, 11, DOOR12_sprite);
// // // }
// // // void draw_door13(int u, int v)
// // // {
// // //     uLCD.BLIT(u, v+1, 11, 11, DOOR13_sprite);
// // // }
// // // void draw_door14(int u, int v)
// // // {
// // //     uLCD.BLIT(u+1, v-1, 11, 11, DOOR14_sprite);
// // // }
// // // void draw_door15(int u, int v)
// // // {
// // //     uLCD.BLIT(u+1, v, 11, 11, DOOR15_sprite);
// // // }
// // // void draw_door16(int u, int v)
// // // {
// // //     uLCD.BLIT(u+1, v+1, 11, 11, DOOR16_sprite);
// // // }
// // //
// // // void draw_Open_door11(int u, int v)
// // // {
// // //     uLCD.BLIT(u, v-1, 11, 11, DOOR11_Open_sprite);
// // // }
// // //
// // // void draw_Open_door12(int u, int v)
// // // {
// // //     uLCD.BLIT(u, v, 11, 11, DOOR12_Open_sprite);
// // // }
// // // void draw_Open_door13(int u, int v)
// // // {
// // //     uLCD.BLIT(u, v+1, 11, 11, DOOR13_Open_sprite);
// // // }
// //
// //
// // void draw_house(int u, int v)
// // {
// //     //uLCD.BLIT(u, v, 11, 11, house_sprite);
// //     const char *trans = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXWWWWWWWWWXXXXXXWWWWWWWXXXXXXWXXXYYYYYWWWWWWYYXXYXYYXWWWXXXYXXWWWWXXXXXXXXXXXXXXXXXXWWWWWWXXXX";
// //
// // }
// //
// //
// // void DisplayStartGame()
// // {
// //
// //     uLCD.filled_rectangle(10,20,118,108,BLACK);
// //     uLCD.locate(7,6);
// //     uLCD.printf("MBED\n");
// //     uLCD.locate(4,9);
// //     uLCD.printf("ADVENTURES!");
// //
// //     //draw_player(58,75,1);
// //
// //     float note[18]= {1568.0,1396.9,1244.5,1244.5,1396.9,1568.0,1568.0,1568.0,1396.9,
// //                  1244.5,1396.9,1568.0,1396.9,1244.5,1174.7,1244.5,1244.5, 0.0
// //                 };
// //     float duration[18]= {0.48,0.24,0.72,0.48,0.24,0.48,0.24,0.24,0.24,
// //                      0.24,0.24,0.24,0.24,0.48,0.24,0.48,0.48, 0.0
// //                     };
// //     for(int i=0; i < 18; i++){
// //         //mySpeaker.PlayNote(note[i],duration[i],0.1);
// //     }
// //     uLCD.cls();
// // }
// //
// //
// //
// //
// //
// // void menu(int quest)
// // {
// //     uLCD.filled_rectangle(10,20,118,108,BLACK);
// //     uLCD.locate(2,3);
// //     switch(quest){
// //         case 0: uLCD.printf("Find the wizard");
// //                 break;
// //         case 1: uLCD.printf("Connect ");
// //                 uLCD.locate(2,4);
// //                 uLCD.printf("6 apples");
// //                 break;
// //         case 2: uLCD.printf("Find the gate");
// //                 break;
// //         case 3: uLCD.printf("Find the second ");
// //                 uLCD.locate(2,4);
// //                 uLCD.printf("wizard");
// //                 break;
// //         case 4: uLCD.printf("Go find the book");
// //                 break;
// //         case 5: uLCD.printf("Give the book to ");
// //                 uLCD.locate(2,4);
// //                 uLCD.printf("the wizard");
// //                 break;
// //         case 6: uLCD.printf("Go get the ");
// //                 uLCD.locate(2,4);
// //                 uLCD.printf("Diamond!!");
// //                 break;
// //     }
// //     uLCD.locate(2,10);
// //     uLCD.printf("action to quit");
// //     wait(1);
// // }
// //
// //
// //
// //
// //
// // //oldstats
// // void draw_upper_status(int health, int x, int y)
// // {
// //     // Draw bottom border of status bar // add some stuff here
// //     //uLCD.line(0, 9, 127, 9, GREEN);
// //     for (int i = 0; i < 3; i++)
// //     {
// //         if (health > 0)
// //         {
// //             draw_life(11*i, 0);
// //             health--;
// //         } else
// //         {
// //             draw_damage(11*i, 0);
// //         }
// //     }
// //
// //     uLCD.filled_rectangle(90, 0, 127, 10, BLACK);
// //     uLCD.locate(50, 0);
// //     uLCD.color(WHITE);
// //     uLCD.printf("%d,%d", x, y);
// // }
// //
// //
// // // void draw_upper_status(int lives, int HP)
// // // {
// // //     // Draw bottom border of status bar
// // //     uLCD.line(0, 9, 128, 9, GREEN);
// // //     uLCD.filled_rectangle(30,2,128,5,BLACK);
// // //     uLCD.rectangle(90,2,103,5,GREEN);
// // //     uLCD.rectangle(107,2,127,5,RED);
// // //     switch(lives){
// // //         case 3: uLCD.filled_rectangle(100,2,103,5,GREEN);
// // //         case 2: uLCD.filled_rectangle(95,2,98,5,GREEN);
// // //         case 1: uLCD.filled_rectangle(90,2,93,5,GREEN);
// // //                 break;
// // //         default: uLCD.filled_rectangle(90,2,128,5,BLACK);
// // //     }
// // //     if(lives > 0){
// // //     uLCD.filled_rectangle((107+((100-HP)/25)*4),2,127,5,RED);
// // //     }
// // //     // Add other status info drawing code here
// // // }
// //
// //
// // void draw_lower_status(int u,int v,int uP,int vP)
// // {
// //     // Draw top border of status bar
// //     uLCD.line(0, 118, 128, 118, GREEN);
// //     if(u != uP || v != vP)
// //     uLCD.filled_rectangle(25,119,128,128,BLACK);
// //     uLCD.locate(11,15);
// //     uLCD.printf("[%d,%d]",u,v);
// //
// //     // Add other status info drawing code here
// // }
// //
// //
// // // void draw_lower_status()
// // // {
// // //     // Draw top border of status bar
// // //     uLCD.line(0, 118, 127, 118, GREEN);
// // //
// // //     // Add other status info drawing code here
// // //
// // // }
// //
// //
// //
// // void draw_border()
// // {
// //     uLCD.filled_rectangle(0,     10, 127,  14, MGREY); // Top
// //     uLCD.filled_rectangle(0,    10,   2, 114, MGREY); // Left
// //     uLCD.filled_rectangle(0,   114, 127, 117, MGREY); // Bottom
// //     uLCD.filled_rectangle(124,  14, 127, 117, MGREY); // Right
// // }
// //
// //
// // // void DisplayGameOver()
// // // {
// // //     uLCD.filled_rectangle(10,20,118,108,BLACK);
// // //     uLCD.locate(7,6);
// // //     uLCD.printf("GAME\n");
// // //     uLCD.locate(4,7);
// // //     uLCD.printf("COMPLETED!");
// // //
// // //     float note[18]= { 0.0,1396.9,1244.5,1244.5,1396.9,1568.0,1568.0,1568.0,1396.9,
// // //                  1244.5,1396.9,1568.0,1396.9,1244.5,1174.7,1244.5,1244.5,1568.0
// // //                 };
// // //     float duration[18]= {0.0,0.72,0.72,0.48,0.24,0.48,0.24,0.24,0.24,
// // //                      0.24,0.24,0.24,0.24,0.48,0.24,0.48,0.48, 0.48
// // //                     };
// // //     for(int i=18; i > 0; i--)
// // //     {
// // //         mySpeaker.PlayNote(note[i],duration[i],0.1);
// // //     }
// // // }
// //
// //
// //
// // void draw_gameover()
// // {
// //   uLCD.filled_rectangle(10,20,118,108,BLACK);
// //   uLCD.locate(7,6);
// //   uLCD.printf("GAME\n");
// //   uLCD.locate(4,7);
// //   uLCD.printf("COMPLETED!");
// //
// //   float note[18]= { 0.0,1396.9,1244.5,1244.5,1396.9,1568.0,1568.0,1568.0,1396.9,
// //                1244.5,1396.9,1568.0,1396.9,1244.5,1174.7,1244.5,1244.5,1568.0
// //               };
// //   float duration[18]= {0.0,0.72,0.72,0.48,0.24,0.48,0.24,0.24,0.24,
// //                    0.24,0.24,0.24,0.24,0.48,0.24,0.48,0.48, 0.48
// //                   };
// //   for(int i=18; i > 0; i--)
// //   {
// //
// //   }
// // }
// //
// //
// //
// //
// //
// // void draw_gamewin()
// // {
// //     uLCD.locate(1, 15);
// //     uLCD.printf("You won!");
// //     uLCD.locate(1, 18);
// //     uLCD.printf("Congratulations!");
// //
// // }
