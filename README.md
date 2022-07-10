# Game1

Config legend:

## Font
### F S R G B
    Font File           F           std::string
    Font Size           S           int
    Font Color          R, G, B     int, int, int

## Player Specification:
### SR, CR, S, FR, FG, FB, OR, OG, OB, OT, V
    Shape Radius        SR          int
    Collision Radius    CR          int
    Speed               S           float
    Fill Color          FR, FG, FB  int, int, int
    Outline Color       OR, OG, OB  int, int, int
    Outline Thickness   OT          int
    Shape Vertices      V           int

## Enemy Specification:
### SSR, CR, SMIN, SMAX OR, OG, OB, OT, VMIN, VMAX, L, SI,
    Shape Radius        SR          int
    Collision Radius    CR          int
    Min/Max Speed       SMIN, SMAX  float, float
    Fill Color          FR, FG, FB  int, int, int
    Outline Color       OR, OG, OB  int, int, int
    Outline Thickness   OT          int
    Min/Max Vertices    VMIN, VMAX  int, int
    Small Lifespan      L           int
    Spawn Interval      SP          int

## Bullet Specification:
### SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L
    Shape Radius        SR          int
    Collision Radius    CR          int
    Speed               S           float
    Fill Color          FR, FG, FB  int, int, int
    Outline Color       OR, OG, OB  int, int, int
    Outline Thickness   OT          int
    Shape Vertices      V           int

## Tips from the master
0. Save the config file reading till later
1. Implement the basic functionality in the entitymanager class addEntity and update so you can start testing. Handle dead entities later.
2. Implement basics:
   1. construct a player using spawnPlayer
   2. Implement basic drawing of entities in render
   3. Spawn enemies with spawnEnemy()
   4. spawnBullet()
3. Implement playermovement, sUserInput and sMovement
4. EntityManager::update() to delete dead
5. getEntities(tag)
6. Entity collision
7. Score, config, etc.
