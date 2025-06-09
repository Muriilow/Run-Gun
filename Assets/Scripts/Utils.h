struct Position
{
    short x;
    short y;
    unsigned int worldX;
    unsigned int worldY;
};
struct Viewport
{
    float x;
    float y;
    float offsetX;
    float offsetY;
    float width;
    float height;
};
struct Vector2
{
    float x;
    float y;
};
enum State
{
    IDLE,
    RUN,
    JUMPING,
    DOUBLE_JUMP,
};
enum Entities
{
    PLAYER,
    NORMAL_ENEMY,
};
