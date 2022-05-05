/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
// Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
static int skill2clips = -1;
static int skillclips = -1;

struct Circle
{
    double x, y;
    double r;
};
// Texture wrapper class
class LTexture
{
   public:
    // Initializes variables
    LTexture();

    // Deallocates memory
    ~LTexture();

    // Loads image at specified path
    bool loadFromFile(std::string path);

    // Deallocates texture
    void free();

    // Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    // Set blending
    void setBlendMode(SDL_BlendMode blending);

    // Set alpha modulation
    void setAlpha(Uint8 alpha);

    // Renders texture at given point
    void render(int x, int y, SDL_Rect* clip = NULL);
    void renderr(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip);

    // Gets image dimensions
    int getWidth();
    int getHeight();
    void setWidth(int m);
    void setHeight(int n);

   private:
    // The actual hardware texture
    SDL_Texture* mTexture;

    // Image dimensions
    int mWidth;
    int mHeight;
};
class platform
{
   public:
    // The dimensions of the dot
    static const int plat_WIDTH = 90;
    static const int plat_HEIGHT = 16;
    static const int spike_WIDTH = 90;
    static const int spike_HEIGHT = 30;

    // Maximum axis velocity of the dot
    static const int DOT_VEL = 10;

    // Initializes the variables
    platform();

    // Moves the dot
    void move();

    // Shows the dot on the screen
    /*void render();*/
    int type;
    int itemtype;
    double ypos;
    double xpos;

    // The velocity of the dot
    double vel;
};
class item
{
   public:
    // Initializes the variables
    item();
    bool check();
    // Shows the dot on the screen
    int type;
    double ypos;
    double xpos;
};
class Dot
{
   public:
    // The dimensions of the dot
    static const int DOT_WIDTH = 35;
    static const int DOT_HEIGHT = 35;

    // Maximum axis velocity of the dot
    static const int DOT_VEL = 3;
    int life = 12;
    int hpplus = 0;
    int hpmin = 49;
    // Initializes the variables
    Dot(int x, int y, int P);

    // Takes key presses and adjusts the dot's velocity
    void handleEvent(SDL_Event& e);
    void handleEvent2(SDL_Event& e);
    // Moves the dot
    bool move(Dot&);
    bool move(Dot&, platform&);
    void itemcheck(Dot&, Dot&, item&);
    // Shows the dot on the screen
    void render();
    void render2();
    void renderskill1(Dot&);
    void renderskill2(Dot&);
    Circle& getCollider();

    int power;
    // The X and Y offsets of the dot
    double mPosX, mPosY;

    // The velocity of the dot
    double mVelX, mVelY;
    Circle mCollider;

    // Moves the collision circle relative to the dot's offset
    void shiftColliders();
};
bool checkCollision(Circle& a, Circle& b);
bool checkCollision(Dot& a, platform&);
double distanceSquared(int x1, int y1, int x2, int y2);
// Starts up SDL and creates window
bool init();

// Loads media
bool loadMedia();

// Frees media and shuts down SDL
void close();
void vcheck();
void inipos();
// The window we'll be rendering to
SDL_Window* gWindow = NULL;
SDL_Texture* loadTexture(std::string path);
// The window renderer
SDL_Renderer* gRenderer = NULL;
SDL_Surface* gScreenSurface = NULL;

// The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

// Current displayed texture
SDL_Texture* BGTexture = NULL;
// Walking animation
const int WALKING_ANIMATION_FRAMES = 7;
const int WALKING_ANIMATION_FRAME = 8;
const int LIFE = 13;
const int skill = 12;
const int skill2 = 11;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
SDL_Rect gSpriteClips2[WALKING_ANIMATION_FRAME];
SDL_Rect glifeclips[LIFE];
SDL_Rect glifeclips2[LIFE];

SDL_Rect gskillclips[skill];
SDL_Rect gskillclips2[skill2];
LTexture glifetexture;
LTexture glifetexture2;
LTexture gSpriteSheetTexture;
LTexture gSpriteSheetTexture2;
LTexture gskilltexture;
LTexture gskilltexture2;
LTexture gTexture[4];
LTexture gBGTexture;
LTexture gstart;
LTexture gButtonTexture[3];
LTexture gp1;
LTexture gp2;

// The music that will be played
Mix_Music* gMusic = NULL;
Mix_Music* gMusic1 = NULL;
Mix_Chunk* gwin = NULL;
Mix_Chunk* gmissile = NULL;
Mix_Chunk* gmushroom = NULL;
Mix_Chunk* gskill2 = NULL;
Mix_Chunk* gwolf = NULL;
Mix_Chunk* ghit = NULL;

platform a, b, c, d, e, f;
item A, B, C, D, E, F;

LTexture::LTexture()
{
    // Initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    // Deallocate
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    // Get rid of preexisting texture
    free();

    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            // Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    // Return success
    mTexture = newTexture;
    return mTexture != NULL;
}
void LTexture::setWidth(int m)
{
    mWidth = m;
}

void LTexture::setHeight(int n)
{
    mHeight = n;
}
void LTexture::free()
{
    // Free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    // Modulate texture rgb
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    // Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    // Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}
void LTexture::renderr(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
    // Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    // Set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    // Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}
platform::platform()
{
    // Initialize the offsets
    xpos = rand() % 300 + 1;
    type = rand() % 10 + 1;
    itemtype = 0;
    // Initialize the velocity
    vel = 1.7;
}
void platform::move()
{
    // Move the dot up or down
    ypos -= vel;

    // If the dot went too far up or down
    if ((ypos < 60))
    {
        ypos = SCREEN_HEIGHT;
        xpos = rand() % 300 + 1;
        type = rand() % 10 + 1;
        itemtype = rand() % 10 + 1;
    }
}

Dot::Dot(int x, int y, int P)
{
    power = P;
    // Initialize the offsets
    mPosX = x;
    mPosY = y;
    mCollider.r = DOT_WIDTH / 2;
    // Initialize the velocity
    mVelX = 0;
    mVelY = 4;
    shiftColliders();
    life = 12;
    hpplus = 0;
    hpmin = 49;
}
Dot dot(205, SCREEN_HEIGHT - 250, 0);
Dot dot2(170, SCREEN_HEIGHT - 250, 0);

item::item()
{
    ypos = 0;
    xpos = 0;
    type = 0;
}
bool item::check()
{
    if (dot.mPosX > xpos - 17 && dot.mPosX < xpos + 47 && dot.mPosY > ypos - 17 && dot.mPosY < ypos + 30)
    {
        xpos = -100;
        ypos = -100;
        return 1;
    }
    if (dot2.mPosX > xpos - 17 && dot2.mPosX < xpos + 47 && dot2.mPosY > ypos - 17 && dot2.mPosY < ypos + 30)
    {
        xpos = -100;
        ypos = -100;
        return 1;
    }
    else
        return 0;
}

static int frame = 3;
static int frame2 = 3;
SDL_Rect* currentClip;
SDL_Rect* currentClip2;
SDL_Rect* currentlifeclip;
SDL_Rect* currentlifeclip2;
SDL_Rect* currentskillclip;
SDL_Rect* currentskillclip2;
void Dot::handleEvent(SDL_Event& e)
{
    // If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_LEFT:
                mVelX -= DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX += DOT_VEL;
                break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_LEFT:
                mVelX += DOT_VEL;
                break;
            case SDLK_RIGHT:
                mVelX -= DOT_VEL;
                break;
        }
    }
}

void Dot::handleEvent2(SDL_Event& e)
{
    // If a key was pressed
    if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_a:
                mVelX -= DOT_VEL;
                break;
            case SDLK_d:
                mVelX += DOT_VEL;
                break;
        }
    }
    // If a key was released
    else if (e.type == SDL_KEYUP && e.key.repeat == 0)
    {
        // Adjust the velocity
        switch (e.key.keysym.sym)
        {
            case SDLK_a:
                mVelX += DOT_VEL;
                break;
            case SDLK_d:
                mVelX -= DOT_VEL;
                break;
        }
    }
}
bool Dot::move(Dot& a)
{
    // Move the dot left or right
    mPosX += mVelX;
    shiftColliders();
    // If the dot went too far to the left or right
    if ((mPosX - mCollider.r <= 0) || (mPosX + mCollider.r >= 400))
    {
        // Move back
        mPosX -= mVelX;
        mPosY += mVelY;
        shiftColliders();
        return true;
    }
    mPosY += mVelY;

    shiftColliders();
    // If the dot went too far up or down
    if ((mPosY - mCollider.r < 60))
    {
        // Move back
        life -= 3;
        Mix_PlayChannel(-1, ghit, 0);
        mPosX += mVelX;
        mPosY += mVelY;
        shiftColliders();
        return true;
    }
    if ((mPosY + mCollider.r > SCREEN_HEIGHT))
    {
        // Move back
        mPosY += mVelY;
        mPosX += mVelX;
        life = 0;
        shiftColliders();
        return true;
    }
    if (checkCollision(mCollider, a.mCollider))
    {
        if (power >= a.power)
        {
            if (mPosX < a.mPosX)
            {
                if (a.mPosX + mCollider.r < 400)
                {
                    a.mPosX = mPosX + 2 * mCollider.r;
                }
                else
                    mPosX -= mVelX;
                return true;
            }
            if (mPosX > a.mPosX)
            {
                if (a.mPosX - mCollider.r > 0)
                {
                    a.mPosX = mPosX - 2 * mCollider.r;
                }
                else
                    mPosX -= mVelX;
                return true;
            }
        }
        if (power < a.power)
        {
            mPosX -= mVelX;
            return true;
        }
        else
            return true;
    }

    // Move the dot up or down

    if (checkCollision(mCollider, a.mCollider))
    {
        // Move back

        mPosY -= mVelY;
        shiftColliders();
        return true;
    }
    return false;
}
bool Dot::move(Dot& a, platform& b)
{
    // Move the dot left or right

    shiftColliders();
    // If the dot went too far to the left or right

    if (checkCollision(a, b))
    {
        a.mPosY = b.ypos - 17;
        a.mVelY = 4;
        if (b.type < 6 || b.type > 8)
        {
            a.hpplus++;
        }
        if (b.type == 6 || b.type == 7 || b.type == 8)
        {
            a.hpmin++;
        }
        if (a.hpplus % 100 == 0)
        {
            a.life += 1;
        }
        if (a.hpmin / 50 == 1)
        {
            a.life -= 1;
            if (a.life % 3 == 0)
            {
                Mix_PlayChannel(-1, ghit, 0);
            }
        }

        shiftColliders();
        return true;
    }
    if (!checkCollision(a, b))
    {
        a.mVelY += 0.02;
        return false;
    }

    else
        return false;
}
void Dot::itemcheck(Dot& a, Dot& b, item& c)
{
    if (a.mPosX > c.xpos - 17 && a.mPosX < c.xpos + 47 && a.mPosY > c.ypos - 17 && a.mPosY < c.ypos + 30)
    {
        if (c.type == 1)
        {
            Mix_PlayChannel(-1, gmissile, 0);
            b.life -= 4;
            c.type = 0;
            Mix_PlayChannel(-1, ghit, 0);
        }
        if (c.type == 2)
        {
            Mix_PlayChannel(-1, gmushroom, 0);
            a.power += 1;
            c.type = 0;
        }
    }
}
void Dot::renderskill1(Dot& dot2)
{
    if (skillclips <= 83)
    {
        currentskillclip = &gskillclips[skillclips / 6];

        gskilltexture.render(0, 60, currentskillclip);
        skillclips++;
    }
    if (skillclips % 16 == 0)
    {
        Mix_PlayChannel(-1, ghit, 0);
        dot2.life -= 1;
    }
}
void Dot::renderskill2(Dot& dot)
{
    if (skill2clips <= 71)
    {
        currentskillclip2 = &gskillclips2[skill2clips / 6];
        gskilltexture2.render(0, 60, currentskillclip2);
        skill2clips++;
    }
    dot.mPosX = 200;
    dot.mPosY = 100;
    if (skill2clips % 70 == 0)
    {
        Mix_PlayChannel(-1, ghit, 0);
        dot.life -= 2;
    }
}
void Dot::render()
{
    // Show the dot
    gSpriteSheetTexture.render(mPosX - mCollider.r, mPosY - mCollider.r, currentClip);
    glifetexture.render(340, 0, currentlifeclip);
}
void Dot::render2()
{
    // Show the dot
    gSpriteSheetTexture2.render(mPosX - mCollider.r, mPosY - mCollider.r, currentClip2);
    glifetexture2.render(10, 0, currentlifeclip2);
}
Circle& Dot::getCollider()
{
    return mCollider;
}
void Dot::shiftColliders()
{
    // Align collider to center of dot
    mCollider.x = mPosX;
    mCollider.y = mPosY;
}
bool init()
{
    // Initialization flag
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        // Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        // Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            // Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                // Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                // Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
                // Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
                {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}
// Button constants
const int BUTTON_WIDTH = 200;
const int BUTTON_HEIGHT = 50;
int flag = 0;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_DOWN = 1,
    BUTTON_SPRITE_MOUSE_OVER = 2,
};
// The mouse button
class LButton
{
   public:
    // Initializes internal variables
    LButton();

    // Sets top left position
    void setPosition(int x, int y);

    // Handles mouse event
    void handleEvent(SDL_Event* a);

    // Shows button sprite
    void render();
    // Top left position
    SDL_Point mPosition;

    // Currently used global sprite
    LButtonSprite mCurrentSprite;
};
LButton gButtons[3];
LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}
void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}
void LButton::handleEvent(SDL_Event* e)
{
    // If mouse event happened
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN)
    {
        // Get mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Check if mouse is in button
        bool inside = true;

        // Mouse is left of the button
        if (x < mPosition.x)
        {
            inside = false;
        }
        // Mouse is right of the button
        else if (x > mPosition.x + BUTTON_WIDTH)
        {
            inside = false;
        }
        // Mouse above the button
        else if (y < mPosition.y)
        {
            inside = false;
        }
        // Mouse below the button
        else if (y > mPosition.y + BUTTON_HEIGHT)
        {
            inside = false;
        }

        // Mouse is outside button
        if (!inside)
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        // Mouse is inside button
        else
        {
            // Set mouse over sprite
            switch (e->type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
            }
        }
    }
}

bool loadMedia()
{
    // Loading success flag
    bool success = true;
    BGTexture = loadTexture("image/background3.png");
    if (BGTexture == NULL)
    {
        printf("Failed to load texture image!\n");
        success = false;
    }
    if (!gBGTexture.loadFromFile("image/background2.png"))
    {
        printf("Failed to load background texture!\n");
        success = false;
    }
    if (!gTexture[0].loadFromFile("image/spike.png"))
    {
        printf("Failed to load platform texture!\n");
        success = false;
    }
    if (!gTexture[1].loadFromFile("image/platform.jpg"))
    {
        printf("Failed to load spike texture!\n");
        success = false;
    }
    if (!gTexture[2].loadFromFile("image/missile.png"))
    {
        printf("Failed to load spike texture!\n");
        success = false;
    }
    if (!gTexture[3].loadFromFile("image/mushroom.png"))
    {
        printf("Failed to load spike texture!\n");
        success = false;
    }
    // Load sprite sheet texture
    if (!gSpriteSheetTexture.loadFromFile("image/DOT1-1-1.PNG"))
    {
        printf("Failed to load walking animation texture!\n");
        success = false;
    }
    if (!gSpriteSheetTexture2.loadFromFile("image/FIGURE2-1-1.PNG"))
    {
        printf("Failed to load walking animation texture!\n");
        success = false;
    }
    if (!glifetexture.loadFromFile("image/LIFEE.PNG"))
    {
        printf("WTF");
        success = false;
    }
    if (!glifetexture2.loadFromFile("image/LIFEE.PNG"))
    {
        printf("WTFF");
        success = false;
    }
    if (!gskilltexture2.loadFromFile("image/SKILL2.PNG"))
    {
        printf("WTFFF");
        success = false;
    }
    if (!gskilltexture.loadFromFile("image/DOG.PNG"))
    {
        printf("WTFFFFF");
        success = false;
    }
    if (!gstart.loadFromFile("image/start.PNG"))
    {
        printf("WTFFFFF");
        success = false;
    }
    if (!gp1.loadFromFile("image/p1.jpg"))
    {
        printf("WTFFFFF");
        success = false;
    }
    if (!gp2.loadFromFile("image/p2.jpg"))
    {
        printf("WTFFFFF");
        success = false;
    }
    if (!gButtonTexture[0].loadFromFile("image/startbutton.PNG"))
    {
        printf("WTFFFFF");
        success = false;
    }
    if (!gButtonTexture[1].loadFromFile("image/again-1.PNG"))
    {
        printf("WTFFFFF");
        success = false;
    }
    if (!gButtonTexture[2].loadFromFile("image/quit.PNG"))
    {
        printf("WTFFFFF");
        success = false;
    }
    gMusic = Mix_LoadMUS("music/startmusic.wav");
    if (gMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gMusic1 = Mix_LoadMUS("music/ingmusic.wav");
    if (gMusic1 == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gwin = Mix_LoadWAV("music/victory.wav");
    if (gwin == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gmissile = Mix_LoadWAV("music/missile.wav");
    if (gwin == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gmushroom = Mix_LoadWAV("music/mushroom.wav");
    if (gwin == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gskill2 = Mix_LoadWAV("music/skill2.wav");
    if (gwin == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    gwolf = Mix_LoadWAV("music/wolf.wav");
    if (gwin == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    ghit = Mix_LoadWAV("music/hit.wav");
    if (gwin == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }
    else
    {
        // Set sprite clips
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 35;
        gSpriteClips[0].h = 35;

        gSpriteClips[1].x = 35;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 35;
        gSpriteClips[1].h = 35;

        gSpriteClips[2].x = 70;
        gSpriteClips[2].y = 0;
        gSpriteClips[2].w = 35;
        gSpriteClips[2].h = 35;

        gSpriteClips[3].x = 105;
        gSpriteClips[3].y = 0;
        gSpriteClips[3].w = 35;
        gSpriteClips[3].h = 35;

        gSpriteClips[4].x = 140;
        gSpriteClips[4].y = 0;
        gSpriteClips[4].w = 35;
        gSpriteClips[4].h = 35;

        gSpriteClips[5].x = 175;
        gSpriteClips[5].y = 0;
        gSpriteClips[5].w = 35;
        gSpriteClips[5].h = 35;

        gSpriteClips[6].x = 210;
        gSpriteClips[6].y = 0;
        gSpriteClips[6].w = 35;
        gSpriteClips[6].h = 35;

        gSpriteClips2[0].x = 0;
        gSpriteClips2[0].y = 0;
        gSpriteClips2[0].w = 35;
        gSpriteClips2[0].h = 35;

        gSpriteClips2[1].x = 35;
        gSpriteClips2[1].y = 0;
        gSpriteClips2[1].w = 35;
        gSpriteClips2[1].h = 35;

        gSpriteClips2[2].x = 70;
        gSpriteClips2[2].y = 0;
        gSpriteClips2[2].w = 35;
        gSpriteClips2[2].h = 35;

        gSpriteClips2[3].x = 110;
        gSpriteClips2[3].y = 0;
        gSpriteClips2[3].w = 35;
        gSpriteClips2[3].h = 35;

        gSpriteClips2[4].x = 150;
        gSpriteClips2[4].y = 0;
        gSpriteClips2[4].w = 35;
        gSpriteClips2[4].h = 35;

        gSpriteClips2[5].x = 185;
        gSpriteClips2[5].y = 0;
        gSpriteClips2[5].w = 35;
        gSpriteClips2[5].h = 35;

        gSpriteClips2[6].x = 220;
        gSpriteClips2[6].y = 0;
        gSpriteClips2[6].w = 35;
        gSpriteClips2[6].h = 35;
        gSpriteClips2[7].x = 255;
        gSpriteClips2[7].y = 0;
        gSpriteClips2[7].w = 70;
        gSpriteClips2[7].h = 35;

        glifeclips[0].x = 0;
        glifeclips[0].y = 0;
        glifeclips[0].w = 110;
        glifeclips[0].h = 55;

        glifeclips[1].x = 110;
        glifeclips[1].y = 0;
        glifeclips[1].w = 110;
        glifeclips[1].h = 55;

        glifeclips[2].x = 220;
        glifeclips[2].y = 0;
        glifeclips[2].w = 110;
        glifeclips[2].h = 55;

        glifeclips[3].x = 330;
        glifeclips[3].y = 0;
        glifeclips[3].w = 110;
        glifeclips[3].h = 55;

        glifeclips[4].x = 440;
        glifeclips[4].y = 0;
        glifeclips[4].w = 110;
        glifeclips[4].h = 55;

        glifeclips[5].x = 550;
        glifeclips[5].y = 0;
        glifeclips[5].w = 110;
        glifeclips[5].h = 55;

        glifeclips[6].x = 660;
        glifeclips[6].y = 0;
        glifeclips[6].w = 110;
        glifeclips[6].h = 55;

        glifeclips[7].x = 770;
        glifeclips[7].y = 0;
        glifeclips[7].w = 110;
        glifeclips[7].h = 55;

        glifeclips[8].x = 880;
        glifeclips[8].y = 0;
        glifeclips[8].w = 110;
        glifeclips[8].h = 55;

        glifeclips[9].x = 990;
        glifeclips[9].y = 0;
        glifeclips[9].w = 110;
        glifeclips[9].h = 55;

        glifeclips[10].x = 1100;
        glifeclips[10].y = 0;
        glifeclips[10].w = 110;
        glifeclips[10].h = 55;

        glifeclips[11].x = 1210;
        glifeclips[11].y = 0;
        glifeclips[11].w = 110;
        glifeclips[11].h = 55;

        glifeclips[12].x = 1320;
        glifeclips[12].y = 0;
        glifeclips[12].w = 110;
        glifeclips[12].h = 55;
        glifeclips2[0].x = 0;
        glifeclips2[0].y = 0;
        glifeclips2[0].w = 110;
        glifeclips2[0].h = 55;

        glifeclips2[1].x = 110;
        glifeclips2[1].y = 0;
        glifeclips2[1].w = 110;
        glifeclips2[1].h = 55;

        glifeclips2[2].x = 220;
        glifeclips2[2].y = 0;
        glifeclips2[2].w = 110;
        glifeclips2[2].h = 55;

        glifeclips2[3].x = 330;
        glifeclips2[3].y = 0;
        glifeclips2[3].w = 110;
        glifeclips2[3].h = 55;

        glifeclips2[4].x = 440;
        glifeclips2[4].y = 0;
        glifeclips2[4].w = 110;
        glifeclips2[4].h = 55;

        glifeclips2[5].x = 550;
        glifeclips2[5].y = 0;
        glifeclips2[5].w = 110;
        glifeclips2[5].h = 55;

        glifeclips2[6].x = 660;
        glifeclips2[6].y = 0;
        glifeclips2[6].w = 110;
        glifeclips2[6].h = 55;

        glifeclips2[7].x = 770;
        glifeclips2[7].y = 0;
        glifeclips2[7].w = 110;
        glifeclips2[7].h = 55;

        glifeclips2[8].x = 880;
        glifeclips2[8].y = 0;
        glifeclips2[8].w = 110;
        glifeclips2[8].h = 55;

        glifeclips2[9].x = 990;
        glifeclips2[9].y = 0;
        glifeclips2[9].w = 110;
        glifeclips2[9].h = 55;

        glifeclips2[10].x = 1100;
        glifeclips2[10].y = 0;
        glifeclips2[10].w = 110;
        glifeclips2[10].h = 55;

        glifeclips2[11].x = 1210;
        glifeclips2[11].y = 0;
        glifeclips2[11].w = 110;
        glifeclips2[11].h = 55;

        glifeclips2[12].x = 1320;
        glifeclips2[12].y = 0;
        glifeclips2[12].w = 110;
        glifeclips2[12].h = 55;
        gskillclips[0].x = 0;
        gskillclips[0].y = 0;
        gskillclips[0].w = 400;
        gskillclips[0].h = 420;

        gskillclips[1].x = 400;
        gskillclips[1].y = 0;
        gskillclips[1].w = 400;
        gskillclips[1].h = 420;

        gskillclips[2].x = 800;
        gskillclips[2].y = 0;
        gskillclips[2].w = 400;
        gskillclips[2].h = 420;

        gskillclips[3].x = 1200;
        gskillclips[3].y = 0;
        gskillclips[3].w = 400;
        gskillclips[3].h = 420;

        gskillclips[4].x = 1600;
        gskillclips[4].y = 0;
        gskillclips[4].w = 400;
        gskillclips[4].h = 420;

        gskillclips[5].x = 2000;
        gskillclips[5].y = 0;
        gskillclips[5].w = 400;
        gskillclips[5].h = 420;

        gskillclips[6].x = 2400;
        gskillclips[6].y = 0;
        gskillclips[6].w = 400;
        gskillclips[6].h = 420;

        gskillclips[7].x = 2800;
        gskillclips[7].y = 0;
        gskillclips[7].w = 400;
        gskillclips[7].h = 420;

        gskillclips[8].x = 3200;
        gskillclips[8].y = 0;
        gskillclips[8].w = 400;
        gskillclips[8].h = 420;

        gskillclips[9].x = 3600;
        gskillclips[9].y = 0;
        gskillclips[9].w = 400;
        gskillclips[9].h = 420;

        gskillclips[10].x = 4000;
        gskillclips[10].y = 0;
        gskillclips[10].w = 400;
        gskillclips[10].h = 420;
        gskillclips[11].x = 4400;
        gskillclips[11].y = 0;
        gskillclips[11].w = 400;
        gskillclips[11].h = 420;

        gskillclips2[0].x = 0;
        gskillclips2[0].y = 0;
        gskillclips2[0].w = 400;
        gskillclips2[0].h = 420;

        gskillclips2[1].x = 400;
        gskillclips2[1].y = 0;
        gskillclips2[1].w = 400;
        gskillclips2[1].h = 420;

        gskillclips2[2].x = 800;
        gskillclips2[2].y = 0;
        gskillclips2[2].w = 400;
        gskillclips2[2].h = 420;

        gskillclips2[3].x = 1200;
        gskillclips2[3].y = 0;
        gskillclips2[3].w = 400;
        gskillclips2[3].h = 420;

        gskillclips2[4].x = 1600;
        gskillclips2[4].y = 0;
        gskillclips2[4].w = 400;
        gskillclips2[4].h = 420;

        gskillclips2[5].x = 2000;
        gskillclips2[5].y = 0;
        gskillclips2[5].w = 400;
        gskillclips2[5].h = 420;

        gskillclips2[6].x = 2400;
        gskillclips2[6].y = 0;
        gskillclips2[6].w = 400;
        gskillclips2[6].h = 420;

        gskillclips2[7].x = 2800;
        gskillclips2[7].y = 0;
        gskillclips2[7].w = 400;
        gskillclips2[7].h = 420;

        gskillclips2[8].x = 3200;
        gskillclips2[8].y = 0;
        gskillclips2[8].w = 400;
        gskillclips2[8].h = 420;

        gskillclips2[9].x = 3600;
        gskillclips2[9].y = 0;
        gskillclips2[9].w = 400;
        gskillclips2[9].h = 420;

        gskillclips2[10].x = 4000;
        gskillclips2[10].y = 0;
        gskillclips2[10].w = 400;
        gskillclips2[10].h = 420;
    }

    return success;
}

void close()
{
    // Free loaded images
    SDL_FreeSurface(gHelloWorld);
    gHelloWorld = NULL;
    SDL_DestroyTexture(BGTexture);
    BGTexture = NULL;
    // Free loaded images
    gskilltexture2.free();
    gSpriteSheetTexture.free();
    gSpriteSheetTexture2.free();
    // Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;
    // Free the music
    Mix_FreeMusic(gMusic);
    gMusic = NULL;
    Mix_FreeMusic(gMusic1);
    gMusic1 = NULL;
    Mix_FreeChunk(gwin);
    gwin = NULL;
    Mix_FreeChunk(gmissile);
    gmissile = NULL;
    Mix_FreeChunk(gmushroom);
    gmushroom = NULL;
    Mix_FreeChunk(gskill2);
    gskill2 = NULL;
    Mix_FreeChunk(gwolf);
    gwolf = NULL;
    Mix_FreeChunk(ghit);
    ghit = NULL;
    // Quit SDL subsystems
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}
SDL_Texture* loadTexture(std::string path)
{
    // The final texture
    SDL_Texture* newTexture = NULL;

    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        // Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        // Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    return newTexture;
}
bool checkCollision(Circle& a, Circle& b)
{
    // Calculate total radius squared
    int totalRadiusSquared = a.r + b.r;
    totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    // If the distance between the centers of the circles is less than the sum of their radii
    if (distanceSquared(a.x, a.y, b.x, b.y) < (totalRadiusSquared))
    {
        // The circles have collided
        return true;
    }

    // Start/stop

    // If not
    return false;
}
bool checkCollision(Dot& a, platform& b)
{
    // Closest point on collision box

    // Find closest x offset

    // Find closest y offset
    if (a.mPosX + a.mCollider.r >= b.xpos && a.mPosX - a.mCollider.r <= b.xpos + b.plat_WIDTH)
    {
        if (abs(b.ypos - a.mPosY - 17) <= 5)
        {
            return true;
        }
    }
    // If the shapes have not collided
    return false;
}
double distanceSquared(int x1, int y1, int x2, int y2)
{
    int deltaX = x2 - x1;
    int deltaY = y2 - y1;
    return deltaX * deltaX + deltaY * deltaY;
}
bool skill_b = false;
bool skill2_b = false;
static int skilltime = 500;
static int skilltime2 = 500;

void inipos()
{
    a.ypos = SCREEN_HEIGHT - 366;
    b.ypos = SCREEN_HEIGHT - 296;
    c.ypos = SCREEN_HEIGHT - 226;
    d.ypos = SCREEN_HEIGHT - 156;
    e.ypos = SCREEN_HEIGHT - 86;
    f.ypos = SCREEN_HEIGHT - 16;
    c.xpos = 155;
    gButtons[0].setPosition(410, 85);
    gButtons[1].setPosition(100, 400);
    gButtons[2].setPosition(320, 400);
}
void vcheck()
{
    if (a.itemtype == 1 || a.itemtype == 2)
    {
        A.xpos = a.xpos + 45;
        A.ypos = a.ypos - 25;
        if (a.itemtype == 1) A.type = 1;
        if (a.itemtype == 2) A.type = 2;
    }
    else
    {
        A.xpos = 0;
        A.ypos = 0;
        A.type = 0;
    }
    if (b.itemtype == 1 || b.itemtype == 2)
    {
        B.xpos = b.xpos + 45;
        B.ypos = b.ypos - 25;
        if (b.itemtype == 1) B.type = 1;
        if (b.itemtype == 2) B.type = 2;
    }
    else
    {
        B.xpos = 0;
        B.ypos = 0;
        B.type = 0;
    }
    if (c.itemtype == 1 || c.itemtype == 2)
    {
        C.xpos = c.xpos + 45;
        C.ypos = c.ypos - 25;
        if (c.itemtype == 1) C.type = 1;
        if (c.itemtype == 2) C.type = 2;
    }
    else
    {
        C.xpos = 0;
        C.ypos = 0;
        C.type = 0;
    }
    if (d.itemtype == 1 || d.itemtype == 2)
    {
        D.xpos = d.xpos + 45;
        D.ypos = d.ypos - 25;
        if (d.itemtype == 1) D.type = 1;
        if (d.itemtype == 2) D.type = 2;
    }
    else
    {
        D.xpos = 0;
        D.ypos = 0;
        D.type = 0;
    }
    if (e.itemtype == 1 || e.itemtype == 2)
    {
        E.xpos = e.xpos + 45;
        E.ypos = e.ypos - 25;
        if (e.itemtype == 1) E.type = 1;
        if (e.itemtype == 2) E.type = 2;
    }
    else
    {
        E.xpos = 0;
        E.ypos = 0;
        E.type = 0;
    }
    if (f.itemtype == 1 || f.itemtype == 2)
    {
        F.xpos = f.xpos + 45;
        F.ypos = f.ypos - 25;
        if (f.itemtype == 1) F.type = 1;
        if (f.itemtype == 2) F.type = 2;
    }
    else
    {
        F.xpos = 0;
        F.ypos = 0;
        F.type = 0;
    }
}
int main(int argc, char* args[])
{
    srand(time(NULL));
    inipos();
    // Start up SDL and create window
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        // Load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            // Main loop flag
            bool quit = false;
            int scrollingOffset = 0;
            // Event handler
            SDL_Event ee;
            SDL_Event e2;
            SDL_Event bu;
            SDL_Event en;
            // Current animation frame
            gstart.setWidth(SCREEN_WIDTH);
            gstart.setHeight(SCREEN_HEIGHT);
            for (int i = 0; i < 3; i++)
            {
                gButtonTexture[i].setWidth(BUTTON_WIDTH);
                gButtonTexture[i].setHeight(BUTTON_HEIGHT);
            }
            gTexture[3].setWidth(30);
            gTexture[3].setHeight(25);
            gTexture[2].setWidth(40);
            gTexture[2].setHeight(35);
            gTexture[1].setWidth(90);
            gTexture[1].setHeight(15);
            gTexture[0].setWidth(90);
            gTexture[0].setHeight(25);
            // While application is running

            while (!quit)
            {
            again:
                if (flag == 0)
                {  // Handle events on queue
                    if (Mix_PlayingMusic() == 0)
                    {
                        // Play the music
                        Mix_PlayMusic(gMusic, -1);
                    }
                    while (SDL_PollEvent(&bu) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                        }

                        // Handle button events
                        gButtons[0].handleEvent(&bu);
                    }

                    if (gButtons[0].mCurrentSprite == BUTTON_SPRITE_MOUSE_DOWN)
                    {
                        Mix_PauseMusic();
                        flag = 1;
                    }
                    else
                    {
                        gstart.render(0, 0, NULL);
                        gButtonTexture[0].render(gButtons[0].mPosition.x, gButtons[0].mPosition.y, NULL);
                        SDL_RenderPresent(gRenderer);
                    }
                }
                if (flag == 1)
                {
                    if (Mix_PausedMusic() == 1)
                    {
                        if (Mix_PlayingMusic() == 1)
                        {  // Play the music
                            Mix_PlayMusic(gMusic1, -1);
                        }

                        // Resume the music
                        Mix_ResumeMusic();
                    }

                    // Handle events on queue
                    while (SDL_PollEvent(&ee) != 0)
                    {
                        dot.handleEvent(ee);
                        dot2.handleEvent2(ee);
                        if (frame2 == 0)
                        {
                            frame2 = 2;
                        }
                        if (frame2 == 6)
                        {
                            frame2 = 4;
                        }
                        if (frame == 0)
                        {
                            frame = 2;
                        }
                        if (frame == 6)
                        {
                            frame = 4;
                        }
                        // User requests quit
                        if (ee.type == SDL_QUIT)
                        {
                            quit = true;
                        }
                        // User presses a key

                        else if (ee.type == SDL_KEYDOWN)
                        {
                            // Select surfaces based on key press
                            const Uint8* keystate = SDL_GetKeyboardState(NULL);
                            if (keystate[SDL_SCANCODE_A])
                            {
                                frame2--;
                            }
                            if (keystate[SDL_SCANCODE_D])
                            {
                                frame2++;
                            }
                            if (keystate[SDL_SCANCODE_RIGHT])
                            {
                                frame++;
                            }

                            if (keystate[SDL_SCANCODE_LEFT])
                            {
                                frame--;
                            }
                            void renderskill1(Dot&);
                        }
                        else
                        {
                            frame = 3;
                            frame2 = 3;
                        }
                    }

                    vcheck();
                    // Move the dot
                    a.move();
                    b.move();
                    c.move();
                    d.move();
                    e.move();
                    f.move();

                    if (dot.life <= 0)
                    {
                        dot.life = 0;
                    }
                    if (dot2.life <= 0)
                    {
                        dot2.life = 0;
                    }
                    if (dot.life >= 12)
                    {
                        dot.life = 12;
                    }
                    if (dot2.life >= 12)
                    {
                        dot2.life = 12;
                    }
                    if (dot.hpplus == 101)
                    {
                        dot.hpplus = 1;
                    }
                    if (dot2.hpplus == 101)
                    {
                        dot2.hpplus = 1;
                    }
                    if (dot.hpmin == 51)
                    {
                        dot.hpmin = 1;
                    }
                    if (dot2.hpmin == 51)
                    {
                        dot2.hpmin = 1;
                    }

                    const Uint8* keystate2 = SDL_GetKeyboardState(NULL);

                    if (keystate2[SDL_SCANCODE_S])
                    {
                        if (skilltime2 >= 500)
                        {
                            Mix_PlayChannel(-1, gskill2, 0);
                            skill2clips = 0;

                            skilltime2 = 0;

                            skill2_b = true;
                        }
                    }

                    if (keystate2[SDL_SCANCODE_DOWN])
                    {
                        if (skilltime >= 500)
                        {
                            Mix_PlayChannel(-1, gwolf, 0);
                            skilltime = 0;
                            skillclips = 0;
                            skill_b = true;
                        }
                    }
                    if (skill2_b == true)
                    {
                        skilltime2++;
                        if (skilltime2 >= 500)
                        {
                            skilltime2 = 500;
                            skill2_b = false;
                        }
                    }
                    if (skill_b == true)
                    {
                        skilltime++;
                        if (skilltime >= 500)
                        {
                            skilltime = 500;
                            skill_b = false;
                        }
                    }

                    dot.move(dot2);
                    dot2.move(dot);
                    dot.move(dot, a);
                    dot.move(dot, b);
                    dot.move(dot, c);
                    dot.move(dot, d);
                    dot.move(dot, e);
                    dot.move(dot, f);
                    dot2.move(dot2, a);
                    dot2.move(dot2, b);
                    dot2.move(dot2, c);
                    dot2.move(dot2, d);
                    dot2.move(dot2, e);
                    dot2.move(dot2, f);
                    --scrollingOffset;
                    if (scrollingOffset < -gBGTexture.getHeight())
                    {
                        scrollingOffset = 0;
                    }

                    // Clear screen
                    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                    SDL_RenderClear(gRenderer);
                    gBGTexture.render(0, scrollingOffset);
                    gBGTexture.render(0, scrollingOffset + gBGTexture.getHeight());

                    currentClip = &gSpriteClips[frame];
                    currentClip2 = &gSpriteClips2[frame2];
                    currentlifeclip = &glifeclips[dot.life];
                    currentlifeclip2 = &glifeclips2[dot2.life];
                    SDL_RenderCopy(gRenderer, BGTexture, NULL, NULL);
                    if (skill2clips <= 71 && skill2clips != -1)
                    {
                        dot2.renderskill2(dot);
                        frame2 = 7;
                    }
                    if (skill2clips == 72)
                    {
                        frame2 = 3;
                        skill2clips++;
                    }
                    if (skillclips <= 83 && skillclips != -1)
                    {
                        dot.renderskill1(dot2);
                    }
                    dot.render();
                    dot2.render2();
                    if (a.type >= 6 && a.type <= 8)
                    {
                        a.itemtype = 0;
                        gTexture[0].render(a.xpos, a.ypos, NULL);
                    }
                    else
                    {
                        gTexture[1].render(a.xpos, a.ypos, NULL);
                        dot.itemcheck(dot, dot2, A);
                        dot.itemcheck(dot2, dot, A);
                        if (A.check()) a.itemtype = 0;
                        if (a.itemtype == 1)
                        {
                            gTexture[2].render(a.xpos + 45, a.ypos - 25, NULL);
                        }
                        else if (a.itemtype == 2)
                        {
                            gTexture[3].render(a.xpos + 45, a.ypos - 25, NULL);
                        }
                    }
                    if (b.type >= 6 && b.type <= 8)
                    {
                        b.itemtype = 0;
                        gTexture[0].render(b.xpos, b.ypos, NULL);
                    }
                    else
                    {
                        gTexture[1].render(b.xpos, b.ypos, NULL);
                        dot.itemcheck(dot, dot2, B);
                        dot.itemcheck(dot2, dot, B);
                        if (B.check()) b.itemtype = 0;
                        if (b.itemtype == 1)
                        {
                            gTexture[2].render(b.xpos + 45, b.ypos - 25, NULL);
                        }
                        else if (b.itemtype == 2)
                        {
                            gTexture[3].render(b.xpos + 45, b.ypos - 25, NULL);
                        }
                    }
                    if (c.type >= 6 && c.type <= 8)
                    {
                        c.itemtype = 0;
                        gTexture[0].render(c.xpos, c.ypos, NULL);
                    }
                    else
                    {
                        gTexture[1].render(c.xpos, c.ypos, NULL);
                        dot.itemcheck(dot, dot2, C);
                        dot.itemcheck(dot2, dot, C);
                        if (C.check()) c.itemtype = 0;
                        if (c.itemtype == 1)
                        {
                            gTexture[2].render(c.xpos + 45, c.ypos - 25, NULL);
                        }
                        else if (c.itemtype == 2)
                        {
                            gTexture[3].render(c.xpos + 45, c.ypos - 25, NULL);
                        }
                    }
                    if (d.type >= 6 && d.type <= 8)
                    {
                        d.itemtype = 0;
                        gTexture[0].render(d.xpos, d.ypos, NULL);
                    }
                    else
                    {
                        gTexture[1].render(d.xpos, d.ypos, NULL);
                        dot.itemcheck(dot, dot2, D);
                        dot.itemcheck(dot2, dot, D);
                        if (D.check()) d.itemtype = 0;
                        if (d.itemtype == 1)
                        {
                            gTexture[2].render(d.xpos + 45, d.ypos - 25, NULL);
                        }
                        else if (d.itemtype == 2)
                        {
                            gTexture[3].render(d.xpos + 45, d.ypos - 25, NULL);
                        }
                    }
                    if (e.type >= 6 && e.type <= 8)
                    {
                        e.itemtype = 0;
                        gTexture[0].render(e.xpos, e.ypos, NULL);
                    }
                    else
                    {
                        gTexture[1].render(e.xpos, e.ypos, NULL);
                        dot.itemcheck(dot, dot2, E);
                        dot.itemcheck(dot2, dot, E);
                        if (E.check()) e.itemtype = 0;
                        if (e.itemtype == 1)
                        {
                            gTexture[2].render(e.xpos + 45, e.ypos - 25, NULL);
                        }
                        else if (e.itemtype == 2)
                        {
                            gTexture[3].render(e.xpos + 45, e.ypos - 25, NULL);
                        }
                    }
                    if (f.type >= 6 && f.type <= 8)
                    {
                        f.itemtype = 0;
                        gTexture[0].render(f.xpos, f.ypos, NULL);
                    }
                    else
                    {
                        gTexture[1].render(f.xpos, f.ypos, NULL);
                        dot.itemcheck(dot, dot2, F);
                        dot.itemcheck(dot2, dot, F);
                        if (F.check()) f.itemtype = 0;
                        if (f.itemtype == 1)
                        {
                            gTexture[2].render(f.xpos + 45, f.ypos - 25, NULL);
                        }
                        else if (f.itemtype == 2)
                        {
                            gTexture[3].render(f.xpos + 45, f.ypos - 25, NULL);
                        }
                    }
                    SDL_RenderPresent(gRenderer);
                    if (dot.life == 0)
                    {
                        Mix_PauseMusic();
                        Mix_PlayChannel(-1, gwin, 0);
                        flag = 2;
                    }
                    if (dot2.life == 0)
                    {
                        Mix_PauseMusic();
                        Mix_PlayChannel(-1, gwin, 0);
                        flag = 3;
                    }

                    // Update screen
                }
                if (flag == 2)
                {
                    while (SDL_PollEvent(&en) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                        }

                        // Handle button events
                        gButtons[1].handleEvent(&en);
                        gButtons[2].handleEvent(&en);
                    }

                    if (gButtons[1].mCurrentSprite == BUTTON_SPRITE_MOUSE_DOWN)
                    {
                        flag = 1;
                        a.ypos = SCREEN_HEIGHT - 366;
                        b.ypos = SCREEN_HEIGHT - 296;
                        c.ypos = SCREEN_HEIGHT - 226;
                        d.ypos = SCREEN_HEIGHT - 156;
                        e.ypos = SCREEN_HEIGHT - 86;
                        f.ypos = SCREEN_HEIGHT - 16;
                        c.xpos = 155;
                        a.type = 1;
                        b.type = 1;
                        c.type = 1;
                        d.type = 1;
                        e.type = 1;
                        f.type = 1;
                        dot.life = 12;
                        dot.hpplus = 0;
                        dot.hpmin = 49;
                        dot.mPosX = 205;
                        dot.mVelX = 0;
                        dot.mPosY = SCREEN_HEIGHT - 260;
                        dot2.life = 12;
                        dot2.hpplus = 0;
                        dot2.hpmin = 49;
                        dot2.mPosX = 170;
                        dot2.mVelX = 0;
                        dot.power = 0;
                        dot2.power = 0;
                        skilltime = 500;
                        skilltime2 = 500;
                        skill2clips = -1;
                        skillclips = -1;
                        dot2.mPosY = SCREEN_HEIGHT - 260;
                        gButtons[1].mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                        gButtons[2].mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                        goto again;
                    }
                    else if (gButtons[2].mCurrentSprite == BUTTON_SPRITE_MOUSE_DOWN)
                    {
                        quit = true;
                    }
                    else
                    {
                        gp2.render(0, 0, NULL);
                        gButtonTexture[1].render(gButtons[1].mPosition.x, gButtons[1].mPosition.y, NULL);
                        gButtonTexture[2].render(gButtons[2].mPosition.x, gButtons[2].mPosition.y, NULL);
                        SDL_RenderPresent(gRenderer);
                    }
                }
                if (flag == 3)
                {
                    while (SDL_PollEvent(&en) != 0)
                    {
                        // User requests quit
                        if (e.type == SDL_QUIT)
                        {
                            quit = true;
                        }

                        // Handle button events
                        gButtons[1].handleEvent(&en);
                        gButtons[2].handleEvent(&en);
                    }

                    if (gButtons[1].mCurrentSprite == BUTTON_SPRITE_MOUSE_DOWN)
                    {
                        flag = 1;
                        a.ypos = SCREEN_HEIGHT - 366;
                        b.ypos = SCREEN_HEIGHT - 296;
                        c.ypos = SCREEN_HEIGHT - 226;
                        d.ypos = SCREEN_HEIGHT - 156;
                        e.ypos = SCREEN_HEIGHT - 86;
                        f.ypos = SCREEN_HEIGHT - 16;
                        c.xpos = 155;
                        a.type = 1;
                        b.type = 1;
                        c.type = 1;
                        d.type = 1;
                        e.type = 1;
                        f.type = 1;
                        skill2clips = -1;
                        skillclips = -1;
                        dot.life = 12;
                        dot.hpplus = 0;
                        dot.hpmin = 49;
                        dot.mPosX = 205;
                        dot.mVelX = 0;
                        dot.mPosY = SCREEN_HEIGHT - 260;
                        dot2.life = 12;
                        dot2.hpplus = 0;
                        dot2.hpmin = 49;
                        dot2.mPosX = 170;
                        dot2.mVelX = 0;
                        dot2.mPosY = SCREEN_HEIGHT - 260;
                        dot.power = 0;
                        dot2.power = 0;
                        skilltime = 500;
                        skilltime2 = 500;
                        skill2clips = -1;
                        skillclips = -1;
                        gButtons[1].mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                        gButtons[2].mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
                        goto again;
                    }
                    else if (gButtons[2].mCurrentSprite == BUTTON_SPRITE_MOUSE_DOWN)
                    {
                        quit = true;
                    }
                    else
                    {
                        gp1.render(0, 0, NULL);
                        gButtonTexture[1].render(gButtons[1].mPosition.x, gButtons[1].mPosition.y, NULL);
                        gButtonTexture[2].render(gButtons[2].mPosition.x, gButtons[2].mPosition.y, NULL);
                        SDL_RenderPresent(gRenderer);
                    }
                }

                // Go to next frame
            }
        }
    }

    // Free resources and close SDL
    close();
    return 0;
}
