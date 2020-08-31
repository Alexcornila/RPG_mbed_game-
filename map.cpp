


#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static int active_map;
static Map map2;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y)
{
    return X + Y * map.w;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key * 1501081;
}

// void maps_init()
// {
//     // TODO: Implement!
//     // Initialize hash table
//     // Set width & height
//     map.items = createHashTable(map_hash, 250);
//     map.w = 53;
//     map.h = 53;
//     set_active_map(0);
// }

// Map* get_active_map()
// {
//     // There's only one map
//     return &map;
// }

// Map* set_active_map(int m)
// {
//     active_map = m;
//     return &map;
// }

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'H', '^', 'S', 'K', 'C', 'N', '|'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

// int map_width() {
//     return (int) map.w;
// }

// int map_height() {
//     return map.h;
// }

// int map_area() {
//     return map.w * map.h;
// }

MapItem* get_north(int x, int y) {
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y - 1));
}

MapItem* get_south(int x, int y) {
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y + 1));
}

MapItem* get_east(int x, int y) {
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x + 1, y));
}

MapItem* get_west(int x, int y) {
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x - 1, y));
}

MapItem* get_here(int x, int y) {
    return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y));
}

void map_erase(int x, int y) {
    deleteItem(get_active_map() -> items, XY_KEY(x, y));
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_door(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = DOOR;
    w1->draw = draw_door;
    w1->walkable = false;
    w1->data = NULL; // if opened (initialize as unopened)
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_spikes(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SPIKES;
    w1->draw = draw_spikes;
    w1->walkable = true;
    w1->data = (void*) 1; // damage dealt
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

// void add_skeleton(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = TROOPER;
//     w1->draw = draw_trooper;
//     w1->walkable = true;
//     w1->data = (void*) 1; // damage dealt
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }

//void add_water(int x, int y)
//{
//    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//    w1->type = WATER;
//    w1->draw = draw_water;
//    w1->walkable = false;
//    w1->data = NULL;
//    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//    if (val) free(val); // If something is already there, free it
//}

// void add_turret(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = TURRET;
//     w1->draw = draw_turret;
//     w1->walkable = false;
//     w1->data = false; // if opened
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }

void add_key(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = KEY;
    w1->draw = draw_key;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_NPC(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = NPC;
    w1->draw = draw_NPC;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_heart(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LIFE;
    w1->draw = draw_life;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_spear(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SPEAR;
    w1->draw = draw_spear;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}





void add_trooper(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TROOPER;
    w1->draw = draw_trooper;
    w1->walkable = true;
    w1->data = (void*) 1;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_turret(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = TURRET;
    w1->draw = draw_turret;
    w1->walkable = false;
    w1->data = NULL; // if opened
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}


void add_life(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LIFE;
    w1->draw = draw_life;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}










// #include "map.h"
// #include "globals.h"
// #include "graphics.h"
 #define numBuckets 250
//
// /**
//  * The Map structure. This holds a HashTable for all the MapItems, along with
//  * values for the width and height of the Map.
//  */
// struct Map
// {
//     HashTable* items;
//     int w, h;
// };
//
// /**
//  * Storage area for the maps.
//  * This is a global variable, but can only be access from this file because it
//  * is static.
//  */
// static Map map;
// //static Map maps[2];
// static int active_map;
 //static Map map2;
//
// /**
//  * The first step in HashTable access for the map is turning the two-dimensional
//  * key information (x, y) into a one-dimensional unsigned integer.
//  * This function should uniquely map (x,y) onto the space of unsigned integers.
//  */
// static unsigned XY_KEY(int X, int Y)
// {
//     //return Y*map.w + X;
//      //X + Y * map.w;
//      return X + Y * map.w;
// }
//
// /**
//  * This is the hash function actually passed into createHashTable. It takes an
//  * unsigned key (the output of XY_KEY) and turns it into a hash value (some
//  * small non-negative integer).
//  */
// unsigned map_hash(unsigned key)
// {
//     return key%numBuckets;
//
// }
//
//
//
//
void maps_init()
{

    // TODO: Implement!
    // Initialize hash table
    // Set width & height
    // map.items = createHashTable(map_hash, 250);
    // map.w = 50;
    // map.h = 50;
    // set_active_map(0);
     //map[0].w =50


       map.items = createHashTable(map_hash, 250);
       map.w = 50;
       map.h = 50;
       //set_active_map(x);

     //else if(x==1)

    map2.items = createHashTable(map_hash, 250);
    map2.w = 50;
    map2.h = 50;
    //set_active_map(x);


}

//
Map* get_active_map()
{
    // There's only one map

      if(!active_map)
         return &map;
         else
         return &map2;
}
//
//
//
Map* set_active_map(int m)
{

    active_map = m;
    if(!active_map)
       return &map;
    else
       return &map2;
}

//
// void print_map()
// {
//     // As you add more types, you'll need to add more items to this array.
//     char lookup[] = {'W', 'P', 'H','t', 'K', 'T', 'N'};
//     for(int y = 0; y < map_height(); y++)
//     {
//         for (int x = 0; x < map_width(); x++)
//         {
//             MapItem* item = get_here(x,y);
//             if (item) pc.printf("%c", lookup[item->type]);
//             else pc.printf(" ");
//         }
//
//     }
// }
//
//
int map_width()
{

  if(!active_map)
  return (int) map.w;
  else
  return (int) map2.w;
  //return (int) map.w;
}

int map_height()
{
    //return map.h;
    if(active_map)
    return map.h;
    else
    return map2.h;
}

//
int map_area()
{
    //return map.w * map.h;
    if(!active_map)
    return (map.h)*(map.w);
    else
    return (map2.h)*(map2.w);
}

//
// MapItem* get_north(int x, int y)
// {
//     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y - 1));
// }
//
// MapItem* get_south(int x, int y)
// {
//     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y + 1));
// }
//
// MapItem* get_east(int x, int y)
// {
//     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x + 1, y));
// }
//
// MapItem* get_west(int x, int y)
// {
//     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x - 1, y));
// }
//
// MapItem* get_here(int x, int y)
// {
//     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y));
// }
//
// void map_erase(int x, int y)
// {
//     deleteItem(get_active_map() -> items, XY_KEY(x, y));
// }
//
// void add_wall(int x, int y, int dir, int len)
// {
//     for(int i = 0; i < len; i++)
//     {
//         MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//         w1->type = WALL;
//         w1->draw = draw_wall;
//         w1->walkable = false;
//         w1->data = NULL;
//         unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
//         void* val = insertItem(get_active_map()->items, key, w1);
//         if (val) free(val); // If something is already there, free it
//     }
// }
//
//
//
// void add_wall2(int x, int y, int dir, int len)
// {
//     for(int i = 0; i < len; i++)
//     {
//         MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//         w1->type = WALL2;
//         w1->draw = draw_wall2;
//         w1->walkable = false;
//         w1->data = NULL;
//         unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
//         void* val = insertItem(get_active_map()->items, key, w1);
//         if (val) free(val); // If something is already there, free it
//     }
// }
//
//
//
//
// void add_plant(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = PLANT;
//     w1->draw = draw_plant;
//     w1->walkable = true;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
//
// void add_plant2(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = PLANT2;
//     w1->draw = draw_plant2;
//     w1->walkable = true;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
// // void add_nothing(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = NOTHING;
// //     w1->draw = draw_nothing;
// //     w1->walkable = true;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
//
// void add_door(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = DOOR;
//     w1->draw = draw_door;
//     w1->walkable = true;
//     w1->data = NULL; // if opened (initialize as unopened)
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
// void add_trooper(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = TROOPER;
//     w1->draw = draw_trooper;
//     w1->walkable = true;
//     w1->data = (void*) 1;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
// void add_turret(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = TURRET;
//     w1->draw = draw_turret;
//     w1->walkable = false;
//     w1->data = NULL; // if opened
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
// void add_key(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = KEY;
//     w1->draw = draw_key;
//     w1->walkable = false;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
// void add_trans(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = TRANS;
//     w1->draw = draw_trans;
//     w1->walkable = true;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
//
// void add_NPC(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = NPC;
//     w1->draw = draw_NPC;
//     w1->walkable = false;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
// void add_life(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = LIFE;
//     w1->draw = draw_life;
//     w1->walkable = false;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
// void add_gun(int x, int y)
// {
//     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
//     w1->type = GUN;
//     w1->draw = draw_gun;
//     w1->walkable = false;
//     w1->data = NULL;
//     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
//     if (val) free(val); // If something is already there, free it
// }
//
//
//
//
//
//
// // #include "map.h"
// // #include "globals.h"
// // #include "graphics.h"
// // #define numBuckets 10
// //
// // //unsigned int numBuckets = 10;
// //
// // /**
// //  * The Map structure. This holds a HashTable for all the MapItems, along with
// //  * values for the width and height of the Map.
// //  */
// // struct Map
// // {
// //     HashTable* items;
// //     int w, h;
// // };
// //
// // /**
// //  * Storage area for the maps.
// //  * This is a global variable, but can only be access from this file because it
// //  * is static.
// //  */
// // static Map map1;
// // //static Map maps[2];
// // static int active_map;
// // static Map map2;
// //
// // /**
// //  * The first step in HashTable access for the map is turning the two-dimensional
// //  * key information (x, y) into a one-dimensional unsigned integer.
// //  * This function should uniquely map (x,y) onto the space of unsigned integers.
// //  */
// // static unsigned XY_KEY(int X, int Y)
// // {
// //     //return Y*map.w + X;
// //      //X + Y * map.w;
// //      // return X + Y * map.w;
// //      return ((X + Y)*(X + Y + 1)/2 + Y);
// // }
// //
// // /**
// //  * This is the hash function actually passed into createHashTable. It takes an
// //  * unsigned key (the output of XY_KEY) and turns it into a hash value (some
// //  * small non-negative integer).
// //  */
// // unsigned map_hash(unsigned key)
// // {
// //     return key%numBuckets;
// //
// // }
// //
// //
// //
// //
// // void maps_init()
// // {
// //
// //     // TODO: Implement!
// //     // Initialize hash table
// //     // Set width & height
// //     // map.items = createHashTable(map_hash, 250);
// //     // map.w = 50;
// //     // map.h = 50;
// //     // set_active_map(0);
// //      //map[0].w =50
// //        map1.items = createHashTable(map_hash, 10);
// //        map1.w = 50;
// //        map1.h = 50;
// //        //set_active_map(x);
// //
// //      //else if(x==1)
// //
// //       map2.items = createHashTable(map_hash, 10);
// //       map2.w = 20;
// //       map2.h = 20;
// //     //set_active_map(x);
// // }
// //
// //
// // Map* get_active_map()
// // {
// //     // There's only one map
// //
// //       if(!active_map)
// //          return &map1;
// //          else
// //          return &map2;
// // }
// //
// //
// // Map* set_active_map(int m)
// // {
// //
// //     active_map = m;
// //     if(!active_map)
// //        return &map1;
// //     else
// //        return &map2;
// // }
// //
// //
// // void print_map()
// // {
// //     // As you add more types, you'll need to add more items to this array.
// //     char lookup[] = {'W', 'P', 'H','t', 'K', 'T', 'N'};
// //     for(int y = 0; y < map_height(); y++)
// //     {
// //         for (int x = 0; x < map_width(); x++)
// //         {
// //             MapItem* item = get_here(x,y);
// //             if (item) pc.printf("%c", lookup[item->type]);
// //             else pc.printf(" ");
// //         }
// //
// //     }
// // }
// //
// //
// // int map_width()
// // {
// //
// //   if(!active_map)
// //   return map1.w;
// //   else
// //   return  map2.w;
// //   //return (int) map.w; tpcast
// // }
// //
// // int map_height()
// // {
// //     //return map.h;
// //     if(active_map)
// //     return map1.h;
// //     else
// //     return map2.h;
// // }
// //
// //
// // int map_area()
// // {
// //     //return map.w * map.h;
// //     if(!active_map)
// //     return (map1.h)*(map1.w);
// //     else
// //     return (map2.h)*(map2.w);
// // }
// //
// //
// // MapItem* get_north(int x, int y)
// // {
// //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y - 1));
// // }
// //
// // MapItem* get_south(int x, int y)
// // {
// //
// //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y + 1));
// // }
// //
// // MapItem* get_east(int x, int y)
// // {
// //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x + 1, y));
// // }
// //
// // MapItem* get_west(int x, int y)
// // {
// //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x - 1, y));
// // }
// //
// // MapItem* get_here(int x, int y)
// // {
// //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y));
// // }
// //
// // void map_erase(int x, int y)
// // {
// //     deleteItem(get_active_map() -> items, XY_KEY(x, y));
// // }
// //
// //
// //
// //
// // //define add functionsto populate map
// // void add_wall(int x, int y, int dir, int len)
// // {
// //     for(int i = 0; i < len; i++)
// //     {
// //         MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //         w1->type = WALL;
// //         w1->draw = draw_wall;
// //         w1->walkable = false;
// //         w1->data = NULL;
// //         unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
// //         void* val = insertItem(get_active_map()->items, key, w1);
// //         if (val) free(val); // If something is already there, free it
// //     }
// // }
// //
// //
// //
// // void add_wall2(int x, int y, int dir, int len)
// // {
// //     for(int i = 0; i < len; i++)
// //     {
// //         MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //         w1->type = WALL2;
// //         w1->draw = draw_wall2;
// //         w1->walkable = false;
// //         w1->data = NULL;
// //         unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
// //         void* val = insertItem(get_active_map()->items, key, w1);
// //         if (val) free(val); // If something is already there, free it
// //     }
// // }
// //
// //
// // void add_plant(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = PLANT;
// //     w1->draw = draw_plant;
// //     w1->walkable = true;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// //
// // void add_plant2(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = PLANT2;
// //     w1->draw = draw_plant2;
// //     w1->walkable = true;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// //
// // void add_door(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = DOOR;
// //     w1->draw = draw_door;
// //     w1->walkable = true;
// //     w1->data = NULL; // if opened (initialize as unopened)
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// // void add_trooper(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = TROOPER;
// //     w1->draw = draw_trooper;
// //     w1->walkable = true;
// //     w1->data = (void*) 1;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// // void add_turret(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = TURRET;
// //     w1->draw = draw_turret;
// //     w1->walkable = false;
// //     w1->data = NULL; // if opened
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// // void add_key(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = KEY;
// //     w1->draw = draw_key;
// //     w1->walkable = false;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// // void add_trans(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = TRANS;
// //     w1->draw = draw_trans;
// //     w1->walkable = true;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// //
// // void add_NPC(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = NPC;
// //     w1->draw = draw_NPC;
// //     w1->walkable = false;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// // void add_life(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = LIFE;
// //     w1->draw = draw_life;
// //     w1->walkable = false;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// // void add_gun(int x, int y)
// // {
// //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// //     w1->type = GUN;
// //     w1->draw = draw_gun;
// //     w1->walkable = false;
// //     w1->data = NULL;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// //     if (val) free(val); // If something is already there, free it
// // }
// //
// //
// //
// //
// //
// // void add_ncp1(int x, int y)
// // {
// //     MapItem* npc = (MapItem*) malloc(sizeof(MapItem));
// //     npc->type = NPC1;
// //     npc->draw = draw_npc1;
// //     npc->drawMove = NULL;
// //     npc->walkable = false;
// //     npc->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), npc);
// //     if (val) free(val);
// // }
// //
// //
// //
// // void add_ncp2(int x, int y)
// // {
// //     MapItem* npc = (MapItem*) malloc(sizeof(MapItem));
// //     npc->type = NPC2;
// //     npc->draw = draw_npc2;
// //     npc->drawMove = NULL;
// //     npc->walkable = false;
// //     npc->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), npc);
// //     if (val) free(val);
// // }
// //
// //
// // void add_door1(int x, int y)
// // {
// //     MapItem* door1 = makeDoor(x,y-1,draw_door11);
// //     MapItem* door2 = makeDoor(x,y,draw_door12);
// //     MapItem* door3 = makeDoor(x,y+1,draw_door13);
// //     MapItem* door4 = makeDoor(x+1,y-1,draw_door14);
// //     MapItem* door5 = makeDoor(x+1,y,draw_door15);
// //     MapItem* door6 = makeDoor(x+1,y+1,draw_door16);
// // }
// //
// //
// //
// // void add_house(int x, int y)
// // {
// //     MapItem* house = (MapItem*) malloc(sizeof(MapItem));
// //     house->type = HOUSE;
// //     house->draw = draw_house;
// //     house->drawMove = NULL;
// //     house->walkable = false;
// //     house->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), house);
// //     if (val) free(val);
// // }
// //
// // void add_fire(int x, int y)
// // {
// //     MapItem* fire = (MapItem*) malloc(sizeof(MapItem));
// //     fire->type = FIRE;
// //     fire->draw = draw_fire;
// //     fire->drawMove = NULL;
// //     fire->walkable = true;
// //     fire->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), fire);
// //     if (val) free(val);
// // }
// //
// // void add_book(int x,int y)
// // {
// //     MapItem* book = (MapItem*) malloc(sizeof(MapItem));
// //     book->type = BOOK;
// //     book->draw = draw_BOOK;
// //     book->drawMove = NULL;
// //     book->walkable = false;
// //     book->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), book);
// //     if (val) free(val);
// // }
// //
// // void add_chest(int x,int y)
// // {
// //     MapItem* chest = (MapItem*) malloc(sizeof(MapItem));
// //     chest->type = CHEST;
// //     chest->draw = draw_chest;
// //     chest->drawMove = NULL;
// //     chest->walkable = false;
// //     chest->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), chest);
// //     if (val) free(val);
// // }
// //
// // void add_diamond(int x,int y)
// // {
// //     MapItem* diamond = (MapItem*) malloc(sizeof(MapItem));
// //     diamond->type = DIAMOND;
// //     diamond->draw = draw_diamond;
// //     diamond->drawMove = NULL;
// //     diamond->walkable = false;
// //     diamond->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), diamond);
// //     if (val) free(val);
// // }
// //
// // void add_Open_door1(int x, int y)
// // {
// //     MapItem* door1 = makeDoor(x,y-1,draw_Open_door11);
// //     MapItem* door2 = makeDoor(x,y,draw_Open_door12);
// //     door2->walkable = true;
// //     MapItem* door3 = makeDoor(x,y+1,draw_Open_door13);
// //     MapItem* door4 = makeDoor(x+1,y-1,draw_door14);
// //     MapItem* door5 = makeDoor(x+1,y,draw_door15);
// //     door5->walkable = true;
// //     MapItem* door6 = makeDoor(x+1,y+1,draw_door16);
// // }
// //
// // MapItem* makeDoor(int x, int y, DrawFunc draw){
// //     MapItem* door = (MapItem*) malloc(sizeof(MapItem));
// //     door->type = DOOR1;
// //     door->draw = draw;
// //     door->drawMove = NULL;
// //     door->walkable = false;
// //     door->data = 0;
// //     void* val = insertItem(get_active_map()->items, XY_KEY(x,y), door);
// //     if (val) free(val);
// //
// //     return door;
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
// //
// //
// //
// //
// //
// //
// //
// //
// //
// //
// //
// // // #include "map.h"
// // // #include "globals.h"
// // // #include "graphics.h"
// // // #define numBuckets 250
// // //
// // // /**
// // //  * The Map structure. This holds a HashTable for all the MapItems, along with
// // //  * values for the width and height of the Map.
// // //  */
// // // struct Map
// // // {
// // //     HashTable* items;
// // //     int w, h;
// // // };
// // //
// // // /**
// // //  * Storage area for the maps.
// // //  * This is a global variable, but can only be access from this file because it
// // //  * is static.
// // //  */
// // // static Map map1;
// // // //static Map maps[2];
// // // static int active_map;
// // // static Map map2;
// // //
// // // /**
// // //  * The first step in HashTable access for the map is turning the two-dimensional
// // //  * key information (x, y) into a one-dimensional unsigned integer.
// // //  * This function should uniquely map (x,y) onto the space of unsigned integers.
// // //  */
// // // static unsigned XY_KEY(int X, int Y)
// // // {
// // //     //return Y*map.w + X;
// // //      //X + Y * map.w;
// // //      //return X + Y * map.w;
// // //      unsigned int pos;
// // //     pos = (X*get_active_map()->w)+Y;
// // //     return pos;
// // // }
// // //
// // // /**
// // //  * This is the hash function actually passed into createHashTable. It takes an
// // //  * unsigned key (the output of XY_KEY) and turns it into a hash value (some
// // //  * small non-negative integer).
// // //  */
// // // unsigned map_hash(unsigned key)
// // // {
// // //     //return key%numBuckets;
// // //     int val;
// // //     val = key%2;
// // //     return val;
// // //
// // // }
// // //
// // //
// // //
// // //
// // // void maps_init()
// // // {
// // //
// // //     // TODO: Implement!
// // //     // Initialize hash table
// // //     // Set width & height
// // //     // map.items = createHashTable(map_hash, 250);
// // //     // map.w = 50;
// // //     // map.h = 50;
// // //     // set_active_map(0);
// // //      //map[0].w =50
// // //      HashTable* HT1 = createHashTable(map_hash, 250);
// // //      HashTable* HT2 = createHashTable(map_hash, 250);
// // //      map1.items = HT1;
// // //      map2.items = HT2;
// // //      map1.w = 50;
// // //      map1.h = 50;
// // //      map2.w = 20;
// // //      map2.h = 20;
// // //
// // //     // map.items = createHashTable(map_hash, 250);
// // //     // map.w = 50;
// // //     // map.h = 50;
// // //        //set_active_map(x);
// // //
// // //      //else if(x==1)
// // //
// // //     // map2.items = createHashTable(map_hash, 250);
// // //     // map2.w = 50;
// // //     // map2.h = 50;
// // //     //set_active_map(x);
// // //
// // //
// // // }
// // //
// // //
// // // Map* get_active_map()
// // // {
// // //     // There's only one map
// // //
// // //       // if(!active_map)
// // //       //    return &map;
// // //       //    else
// // //       //    return &map2;
// // //       if(active_map==0){
// // //               return &map1;
// // //           }else if(active_map==1){
// // //                   return &map2;
// // //               }
// // //           return NULL;
// // //
// // // }
// // //
// // //
// // //
// // // Map* set_active_map(int m)
// // // {
// // //
// // //     // active_map = m;
// // //     // if(!active_map)
// // //     //    return &map;
// // //     // else
// // //     //    return &map2;
// // //         active_map = m;
// // //         return get_active_map();
// // //
// // // }
// // //
// // //
// // // void print_map()
// // // {
// // //     // As you add more types, you'll need to add more items to this array.
// // //     char lookup[] = {'W', 'P', 'H','t', 'K', 'T', 'N'};
// // //     for(int y = 0; y < map_height(); y++)
// // //     {
// // //         for (int x = 0; x < map_width(); x++)
// // //         {
// // //             MapItem* item = get_here(x,y);
// // //             if (item) pc.printf("%c", lookup[item->type]);
// // //             else pc.printf(" ");
// // //         }
// // //
// // //     }
// // // }
// // //
// // //
// // // int map_width()
// // // {
// // //
// // //   // if(!active_map)
// // //   // return (int) map.w;
// // //   // else
// // //   // return (int) map2.w;
// // //   //return (int) map.w;
// // //    return get_active_map()->w;
// // //
// // //  }
// // //
// // // int map_height()
// // // {
// // //     // //return map.h;
// // //     // if(active_map)
// // //     // return map.h;
// // //     // else
// // //     // return map2.h;
// // //     return get_active_map()->h;
// // // }
// // //
// // //
// // // int map_area()
// // // {
// // //     //return map.w * map.h;
// // //     // if(!active_map)
// // //     // return (map.h)*(map.w);
// // //     // else
// // //     // return (map2.h)*(map2.w);
// // //     return map_width() * map_height();
// // // }
// // //
// // //
// // // MapItem* get_north(int x, int y)
// // // {
// // //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y - 1));
// // // }
// // //
// // // MapItem* get_south(int x, int y)
// // // {
// // //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y + 1));
// // // }
// // //
// // // MapItem* get_east(int x, int y)
// // // {
// // //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x + 1, y));
// // // }
// // //
// // // MapItem* get_west(int x, int y)
// // // {
// // //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x - 1, y));
// // // }
// // //
// // // MapItem* get_here(int x, int y)
// // // {
// // //     return (MapItem*) getItem(get_active_map() -> items, XY_KEY(x, y));
// // // }
// // //
// // // void map_erase(int x, int y)
// // // {
// // //     deleteItem(get_active_map() -> items, XY_KEY(x, y));
// // // }
// // //
// // // void add_wall(int x, int y, int dir, int len)
// // // {
// // //     for(int i = 0; i < len; i++)
// // //     {
// // //         MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //         w1->type = WALL;
// // //         w1->draw = draw_wall;
// // //         w1->walkable = false;
// // //         w1->data = NULL;
// // //         unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
// // //         void* val = insertItem(get_active_map()->items, key, w1);
// // //         if (val) free(val); // If something is already there, free it
// // //     }
// // // }
// // //
// // //
// // //
// // // void add_wall2(int x, int y, int dir, int len)
// // // {
// // //     for(int i = 0; i < len; i++)
// // //     {
// // //         MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //         w1->type = WALL2;
// // //         w1->draw = draw_wall2;
// // //         w1->walkable = false;
// // //         w1->data = NULL;
// // //         unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
// // //         void* val = insertItem(get_active_map()->items, key, w1);
// // //         if (val) free(val); // If something is already there, free it
// // //     }
// // // }
// // //
// // //
// // //
// // //
// // // void add_plant(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = PLANT;
// // //     w1->draw = draw_plant;
// // //     w1->walkable = true;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // //
// // // void add_plant2(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = PLANT2;
// // //     w1->draw = draw_plant2;
// // //     w1->walkable = true;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // // // void add_nothing(int x, int y)
// // // // {
// // // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // // //     w1->type = NOTHING;
// // // //     w1->draw = draw_nothing;
// // // //     w1->walkable = true;
// // // //     w1->data = NULL;
// // // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // // //     if (val) free(val); // If something is already there, free it
// // // // }
// // //
// // // void add_door(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = DOOR;
// // //     w1->draw = draw_door;
// // //     w1->walkable = true;
// // //     w1->data = NULL; // if opened (initialize as unopened)
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // // void add_trooper(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = TROOPER;
// // //     w1->draw = draw_trooper;
// // //     w1->walkable = true;
// // //     w1->data = (void*) 1;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // // void add_turret(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = TURRET;
// // //     w1->draw = draw_turret;
// // //     w1->walkable = false;
// // //     w1->data = NULL; // if opened
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // // void add_key(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = KEY;
// // //     w1->draw = draw_key;
// // //     w1->walkable = false;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // // void add_trans(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = TRANS;
// // //     w1->draw = draw_trans;
// // //     w1->walkable = true;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // //
// // // void add_NPC(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = NPC;
// // //     w1->draw = draw_NPC;
// // //     w1->walkable = false;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // // void add_life(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = LIFE;
// // //     w1->draw = draw_life;
// // //     w1->walkable = false;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // // void add_gun(int x, int y)
// // // {
// // //     MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
// // //     w1->type = GUN;
// // //     w1->draw = draw_gun;
// // //     w1->walkable = false;
// // //     w1->data = NULL;
// // //     void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
// // //     if (val) free(val); // If something is already there, free it
// // // }
// // //
// // //
// // //
// // //
// // //
// // //
