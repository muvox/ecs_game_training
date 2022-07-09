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
