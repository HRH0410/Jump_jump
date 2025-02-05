#include "main.h"

//定义一些基础数值
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_BLOCKS 150
#define MAX_BLOCK_STYLES 7
#define PORTAL_SOUND_CHANNEL 1

// 三种游戏状态
typedef enum {
    MENU,
    GAME,
    GAME_OVER
} GameState;

// 落脚平台
typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
} Block;

// 平台的样式
typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
} BlockStyle;

// 传送门
typedef struct {
    SDL_Rect rect;
    SDL_Texture* texture;
} Portal;

// 初始化游戏中需要用到的各项元素
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* Menu_backgroundImage = NULL;
SDL_Texture* Game_backgroundImage1 = NULL;
SDL_Texture* Game_backgroundImage2 = NULL;
SDL_Texture* NewGame_backgroundImage1 = NULL;
SDL_Texture* NewGame_backgroundImage2 = NULL;
SDL_Texture* NewGame_backgroundImage3 = NULL;
SDL_Texture* NewGame_backgroundImage4 = NULL;
SDL_Texture* GameOver_backgroundImage = NULL;
SDL_Texture* startButtonTextTexture = NULL;
SDL_Texture* exitButtonTextTexture = NULL;
SDL_Texture* exit2ButtonTextTexture = NULL;
SDL_Texture* restartButtonTextTexture = NULL;
SDL_Texture* startButtonImageTexture = NULL;
SDL_Texture* exitButtonImageTexture = NULL;
SDL_Texture* restartButtonImageTexture = NULL;
SDL_Texture* exit2ButtonImageTexture = NULL;
SDL_Texture* TitleTexture = NULL;
SDL_Texture* ScoreBoardTexture = NULL;
SDL_Texture* ScoreBoxTexture = NULL;
SDL_Texture* playerTexture = NULL;
SDL_Surface* firstblockSurface=NULL;
SDL_Surface* blockSurface2=NULL;
SDL_Surface* blockSurface1=NULL;
SDL_Surface* blockSurface3=NULL;
SDL_Surface* blockSurface4=NULL;
SDL_Surface* blockSurface5=NULL;
SDL_Surface* blockSurface6=NULL;
SDL_Surface* blockSurface7=NULL;
Mix_Chunk *jumpSound = NULL;
Mix_Music *backgroundMusic = NULL;
Mix_Chunk *portalSound=NULL;

SDL_Rect startButtonRect = { 225, 350, 350, 90 };
SDL_Rect exitButtonRect = { 225, 450, 350, 90 };
SDL_Rect restartButtonRect = { 225, 350, 350, 90 };
SDL_Rect exit2ButtonRect = { 225, 450, 350, 90 };
SDL_Rect playerRect = { 545 , SCREEN_HEIGHT - 130, 50, 50 };
SDL_Rect backgroundRect1 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect backgroundRect2 = {-SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect newbackgroundRect1 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect newbackgroundRect2 = {-SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect newbackgroundRect3 = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect newbackgroundRect4 = {-SCREEN_WIDTH, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
SDL_Rect ScoreBoardRect={250,130,300,150};
SDL_Rect ScoreBoxRect = { 15, 0, 200, 100 };

Portal portal;
GameState gameState = MENU;
Block blocks[MAX_BLOCKS];
BlockStyle blockStyles[MAX_BLOCK_STYLES];

char scoreText[50];
char maxText[50];

int numBlocks = 0;
int blockWidth=150;
int score = 0;
int max=0;
int scoreAnimationValue = 0;

float playerVelocityX = 0.0f;
float playerVelocityY = 0.0f;
float Jump_Strength = -10.0f;
float Horizental_movement_Strength=-10.0f;
float chargingPower = 0.0f;
float backgroundVelocityX = 1.35f;

Uint32 scoreAnimationStartTime = 0;
Uint32 jumpStartTime = 0;

SDL_bool shouldQuitGame = SDL_FALSE;
SDL_bool isSpacePressed = SDL_FALSE;
SDL_bool isOnBlock = SDL_TRUE;
SDL_bool isOnGround=SDL_FALSE;
SDL_bool triggerBackwardMovement = SDL_FALSE;
SDL_bool isNewGame1=SDL_FALSE;
SDL_bool isNewGame2=SDL_FALSE;
SDL_bool isPortalSoundPlaying=SDL_FALSE;
SDL_bool isSpecialStone=SDL_FALSE;
SDL_bool showPortal = SDL_FALSE;

int getRandomX1() {
    return rand() % (SCREEN_WIDTH-500);
}

int getRandomX2() {
    return -rand() % (-100);
}

// 初始化函数
void initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);
    }

    IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);

    // 创建游戏窗口
    window = SDL_CreateWindow("Jump_jump", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }



    //  Menu背景画面
    SDL_Surface *Menu_imageSurface = IMG_Load("res/image/Surface.jpg");
    if (Menu_imageSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    Menu_backgroundImage = SDL_CreateTextureFromSurface(renderer, Menu_imageSurface);
    SDL_FreeSurface(Menu_imageSurface);

    if (Menu_backgroundImage == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 游戏场景1的背景画面
    SDL_Surface *Game_imageSurface1 = IMG_Load("res/image/map1.1.png");
    if (Game_imageSurface1 == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    Game_backgroundImage1 = SDL_CreateTextureFromSurface(renderer, Game_imageSurface1);
    SDL_FreeSurface(Game_imageSurface1);

    if (Game_backgroundImage1 == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface *Game_imageSurface2 = IMG_Load("res/image/map1.2.png");
    if (Game_imageSurface2 == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    Game_backgroundImage2 = SDL_CreateTextureFromSurface(renderer, Game_imageSurface2);
    SDL_FreeSurface(Game_imageSurface2);

    if (Game_backgroundImage2 == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 游戏场景2的背景画面
    SDL_Surface *NewGame_imageSurface1 = IMG_Load("res/image/map2.1.png");
    if (NewGame_imageSurface1 == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    NewGame_backgroundImage1 = SDL_CreateTextureFromSurface(renderer, NewGame_imageSurface1);
    SDL_FreeSurface(NewGame_imageSurface1);

    if (NewGame_backgroundImage1 == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface *NewGame_imageSurface2 = IMG_Load("res/image/map2.2.png");
    if (NewGame_imageSurface2 == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    NewGame_backgroundImage2 = SDL_CreateTextureFromSurface(renderer, NewGame_imageSurface2);
    SDL_FreeSurface(NewGame_imageSurface2);

    if (NewGame_backgroundImage2 == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 游戏场景3的背景画面
    SDL_Surface *NewGame_imageSurface3 = IMG_Load("res/image/map3.1.png");
    if (NewGame_imageSurface3 == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    NewGame_backgroundImage3 = SDL_CreateTextureFromSurface(renderer, NewGame_imageSurface3);
    SDL_FreeSurface(NewGame_imageSurface3);

    if (NewGame_backgroundImage3 == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface *NewGame_imageSurface4 = IMG_Load("res/image/map3.2.png");
    if (NewGame_imageSurface4 == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    NewGame_backgroundImage4= SDL_CreateTextureFromSurface(renderer, NewGame_imageSurface4);
    SDL_FreeSurface(NewGame_imageSurface4);

    if (NewGame_backgroundImage4 == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // Game_Over背景画面
    SDL_Surface *GameOver_imageSurface = IMG_Load("res/image/Surface.jpg");
    if (GameOver_imageSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    GameOver_backgroundImage = SDL_CreateTextureFromSurface(renderer, GameOver_imageSurface);
    SDL_FreeSurface(GameOver_imageSurface);

    if (GameOver_backgroundImage == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 人物形象
    SDL_Surface *Player_imageSurface= IMG_Load("res/image/Penguin.png");
    if (Player_imageSurface == NULL) {
        printf("Unable to load player image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, Player_imageSurface);
    SDL_FreeSurface(Player_imageSurface);

    if (playerTexture == NULL) {
        printf("Unable to create texture from player image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 初始字体
    if (TTF_Init() < 0) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont("res/fond/Fond1.ttf", 20);
    SDL_Color textColor = {101,67,33};

    // 各类按键的文本以及背景图片
    SDL_Surface *startTextSurface = TTF_RenderText_Solid(font, "Start Game", textColor);
    startButtonTextTexture = SDL_CreateTextureFromSurface(renderer, startTextSurface);
    SDL_FreeSurface(startTextSurface);

    SDL_Surface *exitTextSurface = TTF_RenderText_Solid(font, "Exit Game", textColor);
    exitButtonTextTexture = SDL_CreateTextureFromSurface(renderer, exitTextSurface);
    SDL_FreeSurface(exitTextSurface);

    SDL_Surface *restartTextSurface = TTF_RenderText_Solid(font, "Restart Game", textColor);
    restartButtonTextTexture = SDL_CreateTextureFromSurface(renderer, restartTextSurface);
    SDL_FreeSurface(restartTextSurface);

    SDL_Surface *exit2TextSurface = TTF_RenderText_Solid(font, "Exit Game", textColor);
    exit2ButtonTextTexture = SDL_CreateTextureFromSurface(renderer, exit2TextSurface);
    SDL_FreeSurface(exit2TextSurface);

    SDL_Surface* startButtonImageSurface = IMG_Load("res/image/WoodButton.png");
    if (startButtonImageSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    startButtonImageTexture = SDL_CreateTextureFromSurface(renderer, startButtonImageSurface);
    SDL_FreeSurface(startButtonImageSurface);

    if (startButtonImageTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface* exitButtonImageSurface = IMG_Load("res/image/WoodButton.png");
    if (exitButtonImageSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    exitButtonImageTexture = SDL_CreateTextureFromSurface(renderer, exitButtonImageSurface);
    SDL_FreeSurface(exitButtonImageSurface);

    if (exitButtonImageTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface* restartButtonImageSurface = IMG_Load("res/image/WoodButton.png");
    if (restartButtonImageSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    restartButtonImageTexture = SDL_CreateTextureFromSurface(renderer, restartButtonImageSurface);
    SDL_FreeSurface(restartButtonImageSurface);

    if (restartButtonImageTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Surface* exit2ButtonImageSurface = IMG_Load("res/image/WoodButton.png");
    if (exit2ButtonImageSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    exit2ButtonImageTexture = SDL_CreateTextureFromSurface(renderer, exit2ButtonImageSurface);
    SDL_FreeSurface(exit2ButtonImageSurface);

    if (exit2ButtonImageTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 得分板
    SDL_Surface* ScoreBoardSurface = IMG_Load("res/image/ScoreBoard.png");
    if (ScoreBoardSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    ScoreBoardTexture = SDL_CreateTextureFromSurface(renderer, ScoreBoardSurface);
    SDL_FreeSurface(ScoreBoardSurface);

    if (ScoreBoardTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 得分框
    SDL_Surface* ScoreBoxSurface = IMG_Load("res/image/ScoreBox.png");
    if (ScoreBoxSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    ScoreBoxTexture = SDL_CreateTextureFromSurface(renderer, ScoreBoxSurface);
    SDL_FreeSurface(ScoreBoxSurface);

    if (ScoreBoxTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 初始化SDL_mixer，加入各种音效
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    jumpSound = Mix_LoadWAV("res/sound/Ready_Jump.wav");
    if (jumpSound == NULL) {
        printf("Failed to load jump sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    backgroundMusic = Mix_LoadMUS("res/sound/Background_Music.mp3");
    if (backgroundMusic == NULL) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    Mix_PlayMusic(backgroundMusic, -1);

    portalSound = Mix_LoadWAV("res/sound/Portal_Sound.mp3");

    if (portalSound == NULL) {
        printf("Failed to load portal sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        exit(1);
    }

    // Menu界面游戏大标题
    SDL_Surface* TitleSurface = IMG_Load("res/image/Title.png");
    if (TitleSurface == NULL) {
        printf("Unable to load image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    TitleTexture = SDL_CreateTextureFromSurface(renderer, TitleSurface);
    SDL_FreeSurface(TitleSurface);

    if (TitleTexture == NULL) {
        printf("Unable to create texture from image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    // 添加第一个方块
    blocks[numBlocks].rect.x = 520;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth+25;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    firstblockSurface = IMG_Load("res/image/stone3.png");
    if (firstblockSurface == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, firstblockSurface);
    SDL_FreeSurface(firstblockSurface);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

    //添加第二个方块
    blocks[numBlocks].rect.x = getRandomX1() + 25;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    blockSurface2 = IMG_Load("res/image/stone1.png");
    if (blockSurface2 == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, blockSurface2);
    SDL_FreeSurface(blockSurface2);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

    // 传送门
    SDL_Surface *portalSurface = IMG_Load("res/image/Portal.png");
    if (portalSurface == NULL) {
        printf("Unable to load portal image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    portal.texture = SDL_CreateTextureFromSurface(renderer, portalSurface);
    SDL_FreeSurface(portalSurface);

    if (portal.texture == NULL) {
        printf("Unable to create texture from portal image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    portal.rect.x = -60;
    portal.rect.y = SCREEN_HEIGHT - 480;
    portal.rect.w = 350;
    portal.rect.h = 500;

}

// 重新开始游戏后进行的初始化
void Restart_Initialize(){

    // 对于一些数值重新进行设置
    playerRect.x=545;
    playerRect.y=SCREEN_HEIGHT - 148;
    blockWidth=150;
    showPortal=SDL_FALSE;
    isPortalSoundPlaying=SDL_FALSE;
    isNewGame1=SDL_FALSE;
    isNewGame2=SDL_FALSE;
    backgroundRect1.x=0;
    backgroundRect2.x=-SCREEN_WIDTH;
    newbackgroundRect1.x=0;
    newbackgroundRect2.x=-SCREEN_WIDTH;
    newbackgroundRect3.x=0;
    newbackgroundRect4.x=-SCREEN_WIDTH;
    score=0;
    numBlocks=0;

    for(int i=0;i<numBlocks;i++){
        SDL_DestroyTexture(blocks[i].texture);
    }

    // 在画面中新建两个方块
    blocks[numBlocks].rect.x = 520;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth+25;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    firstblockSurface = IMG_Load("res/image/stone3.png");
    if (firstblockSurface == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, firstblockSurface);
    SDL_FreeSurface(firstblockSurface);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

    blockSurface2= IMG_Load("res/image/stone1.png");
    blocks[numBlocks].rect.x = getRandomX1()+25;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    if (blockSurface2 == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, blockSurface2);
    SDL_FreeSurface(blockSurface2);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

}

//切换到第二个场景后进行的初始化
void NewGame1_Initialize(){

    //基础数值的修改
    playerRect.x=545;
    playerRect.y=SCREEN_HEIGHT - 148;
    blockWidth=125;
    showPortal=SDL_FALSE;
    isSpecialStone=SDL_FALSE;
    numBlocks=0;

    for(int i=0;i<numBlocks;i++){
        SDL_DestroyTexture(blocks[i].texture);
    }

    // 经过传送门得分+5并产生动画
    score+=5;
    scoreAnimationValue = 5;
    scoreAnimationStartTime = SDL_GetTicks();

    // 添加两个平台
    blocks[numBlocks].rect.x = 520;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth+25;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    firstblockSurface = IMG_Load("res/image/stone2.png");
    if (firstblockSurface == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, firstblockSurface);
    SDL_FreeSurface(firstblockSurface);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

    blockSurface2= IMG_Load("res/image/stone6.png");
    blocks[numBlocks].rect.x = getRandomX1()+25;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    if (blockSurface2 == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, blockSurface2);
    SDL_FreeSurface(blockSurface2);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

}

//切换到第三个场景后进行的初始化
void NewGame2_Initialize(){

    // 基础数据设定
    playerRect.x=545;
    playerRect.y=SCREEN_HEIGHT - 148;
    blockWidth=100;
    showPortal=SDL_FALSE;
    isNewGame1=SDL_FALSE;
    isSpecialStone=SDL_FALSE;
    numBlocks=0;

    for(int i=0;i<numBlocks;i++){
        SDL_DestroyTexture(blocks[i].texture);
    }

    // 第二次通过传送门得分+10并产生动画
    score+=10;
    scoreAnimationValue = 10;
    scoreAnimationStartTime = SDL_GetTicks();

    // 添加两个平台
    blocks[numBlocks].rect.x = 520;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth+25;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    firstblockSurface = IMG_Load("res/image/stone5.png");
    if (firstblockSurface == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, firstblockSurface);
    SDL_FreeSurface(firstblockSurface);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

    blockSurface2= IMG_Load("res/image/stone3.png");
    blocks[numBlocks].rect.x = getRandomX1()+25;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth;
    blocks[numBlocks].rect.h = 100;
    blockWidth-=3;

    if (blockSurface2 == NULL) {
        printf("Unable to load block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }

    blocks[numBlocks].texture = SDL_CreateTextureFromSurface(renderer, blockSurface2);
    SDL_FreeSurface(blockSurface2);

    if (blocks[numBlocks].texture == NULL) {
        printf("Unable to create texture from block image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    numBlocks++;

}

// 处理键盘和鼠标输入的函数
void handleInput(SDL_Event* event) {
    while (SDL_PollEvent(event) != 0) {
        if (event->type == SDL_QUIT) {
            shouldQuitGame = SDL_TRUE;
        }
        else if (event->type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            switch (gameState) {
                case MENU:
                    if (mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
                        mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h) {
                        gameState = GAME;
                    }

                    if (mouseX >= exitButtonRect.x && mouseX <= exitButtonRect.x + exitButtonRect.w &&
                        mouseY >= exitButtonRect.y && mouseY <= exitButtonRect.y + exitButtonRect.h) {
                        shouldQuitGame = SDL_TRUE;
                    }
                    break;

                case GAME_OVER:
                    if (mouseX >= restartButtonRect.x && mouseX <= restartButtonRect.x + restartButtonRect.w &&
                        mouseY >= restartButtonRect.y && mouseY <= restartButtonRect.y + restartButtonRect.h) {
                        isOnBlock=SDL_TRUE;
                        isOnGround=SDL_FALSE;
                        Restart_Initialize();
                        gameState = GAME;
                    }

                    if (mouseX >= exit2ButtonRect.x && mouseX <= exit2ButtonRect.x + exit2ButtonRect.w &&
                        mouseY >= exit2ButtonRect.y && mouseY <= exit2ButtonRect.y + exit2ButtonRect.h) {
                        shouldQuitGame = SDL_TRUE;
                    }
                    break;
            }
        }

        else if (event->type == SDL_KEYDOWN) {
            if (gameState == GAME && event->key.keysym.sym == SDLK_SPACE &&isOnBlock) {
                // 播放跳跃音效
                Mix_PlayChannel(-1, jumpSound, 0);

                // 记录跳跃时间
                jumpStartTime = SDL_GetTicks();
                isSpacePressed = SDL_TRUE;
            }
        }
        else if (event->type == SDL_KEYUP) {
            if (gameState == GAME && event->key.keysym.sym == SDLK_SPACE && isSpacePressed) {
                // 停止跳跃音效
                Mix_HaltChannel(-1);

                // 根据按下空格键的时间计算跳跃高度和距离
                Uint32 Duration = SDL_GetTicks() - jumpStartTime;
                Jump_Strength = -7.5f - 0.02f * Duration;
                Horizental_movement_Strength=-1.0f-0.018*Duration;
                playerVelocityY = Jump_Strength;
                playerVelocityX = Horizental_movement_Strength;

                // 一次跳跃完成，重置一些数值
                chargingPower = 0.0f;
                playerRect.h=50;
                isSpacePressed = SDL_FALSE;
                isOnBlock=SDL_FALSE;
            }
        }
    }

    // 人物跳跃时的蓄力动画
    if (isSpacePressed) {
        chargingPower += 0.01f;
        playerRect.h = 50 - chargingPower * 50.0f;
        playerRect.y= playerRect.y = blocks[0].rect.y - playerRect.h;
        if (playerRect.h < 30) {
            playerRect.h = 30;
        }
    }

    // 落地则进入Game_Over阶段
    if(gameState==GAME&&isOnGround) {
        switch (gameState) {
            case GAME:
                if (isOnGround) {
                    gameState = GAME_OVER;
                }
                break;
        }
    }

    //进入传送门后播放音效
    if(gameState==GAME&&isNewGame1&&isPortalSoundPlaying==SDL_FALSE||gameState==GAME&&isNewGame2&&isPortalSoundPlaying==SDL_FALSE){
        Mix_PlayChannel(PORTAL_SOUND_CHANNEL, portalSound, 0);
        isPortalSoundPlaying = SDL_TRUE;
    }
}

//随机产生新平台的函数
void generateNewBlock() {
    blockSurface1 = IMG_Load("res/image/stone1.png");
    blockSurface2 = IMG_Load("res/image/stone2.png");
    blockSurface3 = IMG_Load("res/image/stone3.png");
    blockSurface4 = IMG_Load("res/image/stone4.png");
    blockSurface5 = IMG_Load("res/image/stone5.png");
    blockSurface6 = IMG_Load("res/image/stone6.png");
    blockSurface7 = IMG_Load("res/image/stone7.png");

    blockStyles[0].texture = SDL_CreateTextureFromSurface(renderer, blockSurface1);
    blockStyles[1].texture = SDL_CreateTextureFromSurface(renderer, blockSurface2);
    blockStyles[2].texture = SDL_CreateTextureFromSurface(renderer, blockSurface3);
    blockStyles[3].texture = SDL_CreateTextureFromSurface(renderer, blockSurface4);
    blockStyles[4].texture = SDL_CreateTextureFromSurface(renderer, blockSurface5);
    blockStyles[5].texture = SDL_CreateTextureFromSurface(renderer, blockSurface6);
    blockStyles[6].texture = SDL_CreateTextureFromSurface(renderer, blockSurface7);

    int randomStyleIndex = rand() % MAX_BLOCK_STYLES;
    BlockStyle selectedStyle = blockStyles[randomStyleIndex];

    // 产生特殊平台时进行记录
    if(randomStyleIndex==6&&score!=9||randomStyleIndex==6&&score!=29){
        isSpecialStone=SDL_TRUE;
    }
 
    blocks[numBlocks].rect.x = getRandomX2()-50;
    blocks[numBlocks].rect.y = SCREEN_HEIGHT - 100;
    blocks[numBlocks].rect.w = blockWidth;
    blocks[numBlocks].rect.h = 100;

    // 生成的方块的宽度会逐渐减小，直至减小到一定程度
    if(blockWidth>64){
        blockWidth-=3;
    }

    blocks[numBlocks].texture = selectedStyle.texture;

    numBlocks++;

    SDL_FreeSurface(blockSurface1);
    SDL_FreeSurface(blockSurface2);
    SDL_FreeSurface(blockSurface3);
    SDL_FreeSurface(blockSurface4);
    SDL_FreeSurface(blockSurface5);
    SDL_FreeSurface(blockSurface6);
    SDL_FreeSurface(blockSurface7);

}

// 处理人物跳跃的函数
void updatePlayer() {
    // 在空格键完成一次跳跃操作后更新位置
    playerRect.x += (int)playerVelocityX;
    playerRect.y += (int)playerVelocityY;

    // 给予一定重力加速度，使跳跃效果更逼真
    if(!isOnBlock){
        playerVelocityY += 0.5f;
    }

    int width=blocks[numBlocks-1].rect.w-35;
    int check=blocks[numBlocks-1].rect.w+25;
    // 检查是否落地
    if (playerRect.y > SCREEN_HEIGHT-10) {
        playerRect.y = SCREEN_HEIGHT-10 ;
        playerVelocityY = 0.0f;
        playerVelocityX = 0.0f;
        isOnGround=SDL_TRUE;
    }
    else if(playerRect.y>510&&playerRect.x<=blocks[numBlocks-1].rect.x+check){
        playerVelocityX=0.0f;
        if (playerRect.y > SCREEN_HEIGHT-10) {
            playerRect.y = SCREEN_HEIGHT-10 ;
            playerVelocityY = 0.0f;
            isOnGround=SDL_TRUE;
        }
    }
    // 检查是否通过一次短距离跳跃落到原方块上
    else if (playerRect.x <= blocks[numBlocks-2].rect.x+width&&
        playerRect.x +25 > blocks[numBlocks-2].rect.x &&
        playerRect.y <= blocks[numBlocks-2].rect.y +blocks[numBlocks-2].rect.h&&
        playerRect.y + playerRect.h-7 >= blocks[numBlocks-2].rect.y) {
        playerVelocityY = 0.0f;
        playerVelocityX = 0.0f;
        isOnBlock=SDL_TRUE;
        if (playerRect.y > blocks[numBlocks-2].rect.y - playerRect.h+2) {
            playerRect.y = blocks[numBlocks-2].rect.y - playerRect.h+2 ;
        }
    }
    // 检查是否落到下一个方块上
    else if (playerRect.x <= blocks[numBlocks-1].rect.x+width &&
        playerRect.x+25 > blocks[numBlocks-1].rect.x &&
        playerRect.y <= blocks[numBlocks-1].rect.y +blocks[numBlocks-1].rect.h&&
        playerRect.y + playerRect.h-7 >= blocks[numBlocks-1].rect.y) {

        // 重置速度
        playerVelocityY = 0.0f;
        playerVelocityX = 0.0f;
        isOnBlock=SDL_TRUE;

        // 设置每次跳跃的得分情况
        if(isSpecialStone==SDL_TRUE) {
            score+=2;
            scoreAnimationValue = 2;
            scoreAnimationStartTime = SDL_GetTicks();
            isSpecialStone=SDL_FALSE;
        }
        else{
            score++;
            scoreAnimationValue = 1;
            scoreAnimationStartTime = SDL_GetTicks();
        }

        //使人物站在平台上
        if (playerRect.y > blocks[numBlocks-1].rect.y - playerRect.h+2) {
            playerRect.y = blocks[numBlocks-1].rect.y - playerRect.h+2;

        }

        // 落到新方块后产生画面和方块移动的动画效果
        if (isOnBlock) {
            triggerBackwardMovement = SDL_TRUE;
        }

        //产生新方块
        generateNewBlock();
    }

    // 判定是否进入传送门进而进入新的游戏界面
    if(playerRect.x<=75&&score==10){
        NewGame1_Initialize();
        isNewGame1=SDL_TRUE;
    }
    if(playerRect.x<=75&&score==30){
        NewGame2_Initialize();
        isNewGame2=SDL_TRUE;
        isPortalSoundPlaying=SDL_FALSE;
    }
}

// 渲染函数
void render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    switch (gameState) {
        case MENU:
            // 渲染背景图
            SDL_RenderCopy(renderer, Menu_backgroundImage, NULL, NULL);

            // 渲染开始和退出按键
            SDL_Rect imageRect = { 160, 50, 480, 250 };
            SDL_RenderCopy(renderer, TitleTexture, NULL, &imageRect);

            // 增加按键的浮动效果
            float startOffsetX = 7.5 * sin(SDL_GetTicks() * 0.002);
            float startOffsetY = 7.5 * cos(SDL_GetTicks() * 0.002);

            SDL_Rect startButtonImageRect = { startButtonRect.x + (int)startOffsetX, startButtonRect.y + (int)startOffsetY, startButtonRect.w, startButtonRect.h };
            SDL_RenderCopy(renderer, startButtonImageTexture, NULL, &startButtonImageRect);

            SDL_Rect startTextRect = { startButtonImageRect.x + 80, startButtonImageRect.y + 24, startButtonImageRect.w - 160, startButtonImageRect.h - 35 };
            SDL_RenderCopy(renderer, startButtonTextTexture, NULL, &startTextRect);

            float exitOffsetX = 7.5 * sin(SDL_GetTicks() * 0.002);
            float exitOffsetY = 7.5 * cos(SDL_GetTicks() * 0.002);

            SDL_Rect exitButtonImageRect = { exitButtonRect.x + (int)exitOffsetX, exitButtonRect.y + (int)exitOffsetY, exitButtonRect.w, exitButtonRect.h };
            SDL_RenderCopy(renderer, exitButtonImageTexture, NULL, &exitButtonImageRect);

            SDL_Rect exitTextRect = { exitButtonImageRect.x + 80, exitButtonImageRect.y + 24, exitButtonImageRect.w - 160, exitButtonImageRect.h - 35 };
            SDL_RenderCopy(renderer, exitButtonTextTexture, NULL, &exitTextRect);

            break;
        case GAME:
            // 第一种游戏场景
            if(!isNewGame1&&!isNewGame2) {
                // 渲染两张背景图
                SDL_RenderCopy(renderer, Game_backgroundImage1, NULL, &backgroundRect1);
                SDL_RenderCopy(renderer, Game_backgroundImage2, NULL, &backgroundRect2);

                // 渲染人物
                SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

                // 调整方块的绘制位置，使其随着背景滚动
                for (int i = 0; i < numBlocks; i++) {
                    SDL_Rect centeredBlockRect = {blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w,
                                                  blocks[i].rect.h};
                    SDL_RenderCopy(renderer, blocks[i].texture, NULL, &centeredBlockRect);
                }

                // 当人物落在新方块上，通过渲染使画面具有向前移动的动态感
                if (triggerBackwardMovement) {
                    // 调整动画速度
                    int animationSpeed = 5;

                    // 使方块和人物同步后移至画面右侧
                    while (blocks[numBlocks - 2].rect.x < 520) {
                        playerRect.x += animationSpeed;
                        for (int i = 0; i < numBlocks; i++) {
                            blocks[i].rect.x += animationSpeed;
                        }

                        SDL_RenderCopy(renderer, Game_backgroundImage1, NULL, &backgroundRect1);
                        SDL_RenderCopy(renderer, Game_backgroundImage2, NULL, &backgroundRect2);

                        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

                        for (int i = 0; i < numBlocks; i++) {
                            SDL_Rect centeredBlockRect = {blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w,
                                                          blocks[i].rect.h};
                            SDL_RenderCopy(renderer, blocks[i].texture, NULL, &centeredBlockRect);
                        }

                        // 渲染得分框和得分文本
                        SDL_RenderCopy(renderer, ScoreBoxTexture, NULL, &ScoreBoxRect);

                        sprintf(scoreText, "Score: %d", score);

                        TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                        if (!font) {
                            printf("Unable to load font: %s\n", TTF_GetError());
                            exit(1);
                        }

                        SDL_Color textColor = {101,67,33};  // White color
                        SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, scoreText, textColor);
                        if (!scoreTextSurface) {
                            printf("Unable to render score text surface: %s\n", TTF_GetError());
                            exit(1);
                        }

                        SDL_Texture *scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
                        if (!scoreTextTexture) {
                            printf("Unable to create score text texture: %s\n", SDL_GetError());
                            exit(1);
                        }

                        SDL_Rect scoreTextRect = {ScoreBoxRect.x + 43, ScoreBoxRect.y + 53, 120,
                                                  40};
                        SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);

                        SDL_FreeSurface(scoreTextSurface);
                        SDL_DestroyTexture(scoreTextTexture);
                        TTF_CloseFont(font);

                        // 渲染显示每次得分的动画
                        Uint32 currentTime = SDL_GetTicks();
                        Uint32 elapsedTime = currentTime - scoreAnimationStartTime;
                        Uint32 animationDuration = 1000;
                        int fadeOutSpeed = 2;

                        if (elapsedTime < animationDuration) {
                            int alpha = 255 - (fadeOutSpeed * elapsedTime) / (animationDuration);

                            TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                            SDL_Color textColor = {255, 255, 255, alpha};
                            char scoreAnimationText[20];
                            sprintf(scoreAnimationText, "+%d", scoreAnimationValue);

                            SDL_Surface *scoreAnimationSurface = TTF_RenderText_Solid(font, scoreAnimationText,
                                                                                      textColor);
                            SDL_Texture *scoreAnimationTexture = SDL_CreateTextureFromSurface(renderer,
                                                                                              scoreAnimationSurface);

                            SDL_Rect scoreAnimationRect = {playerRect.x, playerRect.y - 30, scoreAnimationSurface->w+10,
                                                           scoreAnimationSurface->h+5};

                            SDL_RenderCopy(renderer, scoreAnimationTexture, NULL, &scoreAnimationRect);

                            SDL_FreeSurface(scoreAnimationSurface);
                            SDL_DestroyTexture(scoreAnimationTexture);
                            TTF_CloseFont(font);
                        }

                        // 移动背景画面
                        backgroundRect1.x += (int) backgroundVelocityX;
                        backgroundRect2.x += (int) backgroundVelocityX;

                        // 如果背景完全离开屏幕，重新放置
                        if (backgroundRect1.x >= SCREEN_WIDTH) {
                            backgroundRect1.x = -SCREEN_WIDTH;
                        }

                        if (backgroundRect2.x >= SCREEN_WIDTH) {
                            backgroundRect2.x = -SCREEN_WIDTH;
                        }

                        // 渲染传送门
                        if (score == 10) {
                            showPortal = SDL_TRUE;
                        } else {
                            showPortal = SDL_FALSE;
                        }

                        if (showPortal) {
                            SDL_RenderCopy(renderer, portal.texture, NULL, &portal.rect);
                        }

                        // 刷新屏幕
                        SDL_RenderPresent(renderer);
                        SDL_Delay(16);
                    }

                    triggerBackwardMovement = SDL_FALSE;
                }

                SDL_RenderCopy(renderer, ScoreBoxTexture, NULL, &ScoreBoxRect);

                sprintf(scoreText, "Score: %d", score);

                TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                if (!font) {
                    printf("Unable to load font: %s\n", TTF_GetError());
                    exit(1);
                }

                SDL_Color textColor = {101,67,33};
                SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, scoreText, textColor);
                if (!scoreTextSurface) {
                    printf("Unable to render score text surface: %s\n", TTF_GetError());
                    exit(1);
                }

                SDL_Texture *scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
                if (!scoreTextTexture) {
                    printf("Unable to create score text texture: %s\n", SDL_GetError());
                    exit(1);
                }

                SDL_Rect scoreTextRect = {ScoreBoxRect.x + 43, ScoreBoxRect.y + 53, 120, 40};
                SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);

                SDL_FreeSurface(scoreTextSurface);
                SDL_DestroyTexture(scoreTextTexture);
                TTF_CloseFont(font);

                if (score == 10) {
                    showPortal = SDL_TRUE;
                } else {
                    showPortal = SDL_FALSE;
                }
                if (showPortal) {
                    SDL_RenderCopy(renderer, portal.texture, NULL, &portal.rect);
                }
            }

            // 第2种游戏场景
            if(isNewGame1) {
                SDL_RenderCopy(renderer, NewGame_backgroundImage1, NULL, &newbackgroundRect1);
                SDL_RenderCopy(renderer, NewGame_backgroundImage2, NULL, &newbackgroundRect2);

                SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

                for (int i = 0; i < numBlocks; i++) {
                    SDL_Rect centeredBlockRect = {blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w,
                                                  blocks[i].rect.h};
                    SDL_RenderCopy(renderer, blocks[i].texture, NULL, &centeredBlockRect);
                }

                Uint32 currentTime = SDL_GetTicks();
                Uint32 elapsedTime = currentTime - scoreAnimationStartTime;
                Uint32 animationDuration = 1000;
                int fadeOutSpeed = 2;

                if (elapsedTime < animationDuration) {
                    int alpha = 255 - (fadeOutSpeed * elapsedTime) / (animationDuration);

                    TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                    SDL_Color textColor = {255, 255, 255, alpha};
                    char scoreAnimationText[20];
                    sprintf(scoreAnimationText, "+%d", scoreAnimationValue);

                    SDL_Surface *scoreAnimationSurface = TTF_RenderText_Solid(font, scoreAnimationText,
                                                                              textColor);
                    SDL_Texture *scoreAnimationTexture = SDL_CreateTextureFromSurface(renderer,
                                                                                      scoreAnimationSurface);

                    SDL_Rect scoreAnimationRect = {playerRect.x, playerRect.y - 30, scoreAnimationSurface->w+10,
                                                   scoreAnimationSurface->h+5};

                    SDL_RenderCopy(renderer, scoreAnimationTexture, NULL, &scoreAnimationRect);

                    SDL_FreeSurface(scoreAnimationSurface);
                    SDL_DestroyTexture(scoreAnimationTexture);
                    TTF_CloseFont(font);
                }

                if (triggerBackwardMovement) {
                    int animationSpeed = 5;
                    while (blocks[numBlocks - 2].rect.x < 520) {
                        playerRect.x += animationSpeed;
                        for (int i = 0; i < numBlocks; i++) {
                            blocks[i].rect.x += animationSpeed;
                        }
                        SDL_RenderCopy(renderer, NewGame_backgroundImage1, NULL, &newbackgroundRect1);
                        SDL_RenderCopy(renderer, NewGame_backgroundImage2, NULL, &newbackgroundRect2);

                        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

                        for (int i = 0; i < numBlocks; i++) {
                            SDL_Rect centeredBlockRect = {blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w,
                                                          blocks[i].rect.h};
                            SDL_RenderCopy(renderer, blocks[i].texture, NULL, &centeredBlockRect);
                        }

                        SDL_RenderCopy(renderer, ScoreBoxTexture, NULL, &ScoreBoxRect);

                        sprintf(scoreText, "Score: %d", score);

                        TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                        if (!font) {
                            printf("Unable to load font: %s\n", TTF_GetError());
                            exit(1);
                        }

                        SDL_Color textColor = {101,67,33};
                        SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, scoreText, textColor);
                        if (!scoreTextSurface) {
                            printf("Unable to render score text surface: %s\n", TTF_GetError());
                            exit(1);
                        }

                        SDL_Texture *scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
                        if (!scoreTextTexture) {
                            printf("Unable to create score text texture: %s\n", SDL_GetError());
                            exit(1);
                        }

                        SDL_Rect scoreTextRect = {ScoreBoxRect.x + 43, ScoreBoxRect.y + 53, 120,
                                                  40};
                        SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);

                        SDL_FreeSurface(scoreTextSurface);
                        SDL_DestroyTexture(scoreTextTexture);
                        TTF_CloseFont(font);

                        Uint32 currentTime = SDL_GetTicks();
                        Uint32 elapsedTime = currentTime - scoreAnimationStartTime;
                        Uint32 animationDuration = 1000;
                        int fadeOutSpeed = 2;

                        if (elapsedTime < animationDuration) {
                            int alpha = 255 - (fadeOutSpeed * elapsedTime) / (animationDuration);

                            TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                            SDL_Color textColor = {255, 255, 255, alpha};
                            char scoreAnimationText[20];
                            sprintf(scoreAnimationText, "+%d", scoreAnimationValue);

                            SDL_Surface *scoreAnimationSurface = TTF_RenderText_Solid(font, scoreAnimationText,
                                                                                      textColor);
                            SDL_Texture *scoreAnimationTexture = SDL_CreateTextureFromSurface(renderer,
                                                                                              scoreAnimationSurface);

                            SDL_Rect scoreAnimationRect = {playerRect.x, playerRect.y - 30, scoreAnimationSurface->w+10,
                                                           scoreAnimationSurface->h+5};

                            SDL_RenderCopy(renderer, scoreAnimationTexture, NULL, &scoreAnimationRect);

                            SDL_FreeSurface(scoreAnimationSurface);
                            SDL_DestroyTexture(scoreAnimationTexture);
                            TTF_CloseFont(font);
                        }

                        newbackgroundRect1.x += (int) backgroundVelocityX;
                        newbackgroundRect2.x += (int) backgroundVelocityX;

                        if (newbackgroundRect1.x >= SCREEN_WIDTH) {
                           newbackgroundRect1.x = -SCREEN_WIDTH;
                        }

                        if (newbackgroundRect2.x >= SCREEN_WIDTH) {
                            newbackgroundRect2.x = -SCREEN_WIDTH;
                        }

                        if (score == 30) {
                            showPortal = SDL_TRUE;
                        } else {
                            showPortal = SDL_FALSE;
                        }

                        if (showPortal) {
                            SDL_RenderCopy(renderer, portal.texture, NULL, &portal.rect);
                        }

                        SDL_RenderPresent(renderer);
                        SDL_Delay(16);
                    }

                    triggerBackwardMovement = SDL_FALSE;
                }

                SDL_RenderCopy(renderer, ScoreBoxTexture, NULL, &ScoreBoxRect);

                sprintf(scoreText, "Score: %d", score);

                TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                if (!font) {
                    printf("Unable to load font: %s\n", TTF_GetError());
                    exit(1);
                }

                SDL_Color textColor = {101,67,33};
                SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, scoreText, textColor);
                if (!scoreTextSurface) {
                    printf("Unable to render score text surface: %s\n", TTF_GetError());
                    exit(1);
                }

                SDL_Texture *scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
                if (!scoreTextTexture) {
                    printf("Unable to create score text texture: %s\n", SDL_GetError());
                    exit(1);
                }

                SDL_Rect scoreTextRect = {ScoreBoxRect.x + 43, ScoreBoxRect.y + 53, 120, 40};
                SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);

                SDL_FreeSurface(scoreTextSurface);
                SDL_DestroyTexture(scoreTextTexture);
                TTF_CloseFont(font);

                if (score == 30) {
                    showPortal = SDL_TRUE;
                } else {
                    showPortal = SDL_FALSE;
                }
                if (showPortal) {
                    SDL_RenderCopy(renderer, portal.texture, NULL, &portal.rect);
                }
            }

            //第3种游戏场景
            if(isNewGame2) {
                SDL_RenderCopy(renderer, NewGame_backgroundImage3, NULL, &newbackgroundRect3);
                SDL_RenderCopy(renderer, NewGame_backgroundImage4, NULL, &newbackgroundRect4);

                SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

                for (int i = 0; i < numBlocks; i++) {
                    SDL_Rect centeredBlockRect = {blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w,
                                                  blocks[i].rect.h};
                    SDL_RenderCopy(renderer, blocks[i].texture, NULL, &centeredBlockRect);
                }

                Uint32 currentTime = SDL_GetTicks();
                Uint32 elapsedTime = currentTime - scoreAnimationStartTime;
                Uint32 animationDuration = 1000;
                int fadeOutSpeed = 2;

                if (elapsedTime < animationDuration) {
                    int alpha = 255 - (fadeOutSpeed * elapsedTime) / (animationDuration);

                    TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                    SDL_Color textColor = {255, 255, 255, alpha};
                    char scoreAnimationText[20];
                    sprintf(scoreAnimationText, "+%d", scoreAnimationValue);

                    SDL_Surface *scoreAnimationSurface = TTF_RenderText_Solid(font, scoreAnimationText,
                                                                              textColor);
                    SDL_Texture *scoreAnimationTexture = SDL_CreateTextureFromSurface(renderer,
                                                                                      scoreAnimationSurface);

                    SDL_Rect scoreAnimationRect = {playerRect.x, playerRect.y - 30, scoreAnimationSurface->w+10,
                                                   scoreAnimationSurface->h+5};

                    SDL_RenderCopy(renderer, scoreAnimationTexture, NULL, &scoreAnimationRect);

                    SDL_FreeSurface(scoreAnimationSurface);
                    SDL_DestroyTexture(scoreAnimationTexture);
                    TTF_CloseFont(font);
                }

                if (triggerBackwardMovement) {
                    int animationSpeed = 5;
                    while (blocks[numBlocks - 2].rect.x < 520) {
                        playerRect.x += animationSpeed;
                        for (int i = 0; i < numBlocks; i++) {
                            blocks[i].rect.x += animationSpeed;
                        }
                        SDL_RenderCopy(renderer, NewGame_backgroundImage3, NULL, &newbackgroundRect3);
                        SDL_RenderCopy(renderer, NewGame_backgroundImage4, NULL, &newbackgroundRect4);

                        SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);

                        for (int i = 0; i < numBlocks; i++) {
                            SDL_Rect centeredBlockRect = {blocks[i].rect.x, blocks[i].rect.y, blocks[i].rect.w,
                                                          blocks[i].rect.h};
                            SDL_RenderCopy(renderer, blocks[i].texture, NULL, &centeredBlockRect);
                        }

                        SDL_RenderCopy(renderer, ScoreBoxTexture, NULL, &ScoreBoxRect);

                        sprintf(scoreText, "Score: %d", score);

                        TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                        if (!font) {
                            printf("Unable to load font: %s\n", TTF_GetError());
                            exit(1);
                        }

                        SDL_Color textColor = {101, 67, 33};
                        SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, scoreText, textColor);
                        if (!scoreTextSurface) {
                            printf("Unable to render score text surface: %s\n", TTF_GetError());
                            exit(1);
                        }

                        SDL_Texture *scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
                        if (!scoreTextTexture) {
                            printf("Unable to create score text texture: %s\n", SDL_GetError());
                            exit(1);
                        }

                        SDL_Rect scoreTextRect = {ScoreBoxRect.x + 43, ScoreBoxRect.y + 53, 120,
                                                  40};
                        SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);

                        SDL_FreeSurface(scoreTextSurface);
                        SDL_DestroyTexture(scoreTextTexture);
                        TTF_CloseFont(font);

                        Uint32 currentTime = SDL_GetTicks();
                        Uint32 elapsedTime = currentTime - scoreAnimationStartTime;
                        Uint32 animationDuration = 1000;
                        int fadeOutSpeed = 2;

                        if (elapsedTime < animationDuration) {
                            int alpha = 255 - (fadeOutSpeed * elapsedTime) / (animationDuration);

                            TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                            SDL_Color textColor = {255, 255, 255, alpha};
                            char scoreAnimationText[20];
                            sprintf(scoreAnimationText, "+%d", scoreAnimationValue);

                            SDL_Surface *scoreAnimationSurface = TTF_RenderText_Solid(font, scoreAnimationText,
                                                                                      textColor);
                            SDL_Texture *scoreAnimationTexture = SDL_CreateTextureFromSurface(renderer,
                                                                                              scoreAnimationSurface);

                            SDL_Rect scoreAnimationRect = {playerRect.x, playerRect.y - 30, scoreAnimationSurface->w+10,
                                                           scoreAnimationSurface->h+5};

                            SDL_RenderCopy(renderer, scoreAnimationTexture, NULL, &scoreAnimationRect);

                            SDL_FreeSurface(scoreAnimationSurface);
                            SDL_DestroyTexture(scoreAnimationTexture);
                            TTF_CloseFont(font);
                        }

                        newbackgroundRect3.x += (int) backgroundVelocityX;
                        newbackgroundRect4.x += (int) backgroundVelocityX;

                        if (newbackgroundRect3.x >= SCREEN_WIDTH) {
                            newbackgroundRect3.x = -SCREEN_WIDTH;
                        }

                        if (newbackgroundRect4.x >= SCREEN_WIDTH) {
                            newbackgroundRect4.x = -SCREEN_WIDTH;
                        }

                        SDL_RenderPresent(renderer);
                        SDL_Delay(16);
                    }

                    triggerBackwardMovement = SDL_FALSE;
                }

                SDL_RenderCopy(renderer, ScoreBoxTexture, NULL, &ScoreBoxRect);

                sprintf(scoreText, "Score: %d", score);

                TTF_Font *font = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
                if (!font) {
                    printf("Unable to load font: %s\n", TTF_GetError());
                    exit(1);
                }

                SDL_Color textColor = {101, 67, 33};
                SDL_Surface *scoreTextSurface = TTF_RenderText_Solid(font, scoreText, textColor);
                if (!scoreTextSurface) {
                    printf("Unable to render score text surface: %s\n", TTF_GetError());
                    exit(1);
                }

                SDL_Texture *scoreTextTexture = SDL_CreateTextureFromSurface(renderer, scoreTextSurface);
                if (!scoreTextTexture) {
                    printf("Unable to create score text texture: %s\n", SDL_GetError());
                    exit(1);
                }

                SDL_Rect scoreTextRect = {ScoreBoxRect.x + 43, ScoreBoxRect.y + 53, 120, 40};
                SDL_RenderCopy(renderer, scoreTextTexture, NULL, &scoreTextRect);

                SDL_FreeSurface(scoreTextSurface);
                SDL_DestroyTexture(scoreTextTexture);
                TTF_CloseFont(font);
            }
            break;

        case GAME_OVER:
            SDL_RenderCopy(renderer, GameOver_backgroundImage, NULL, NULL);

            float restartOffsetX = 7.5 * sin(SDL_GetTicks() * 0.002);
            float restartOffsetY = 7.5 * cos(SDL_GetTicks() * 0.002);

            SDL_Rect restartButtonImageRect = { restartButtonRect.x + (int)restartOffsetX, restartButtonRect.y + (int)restartOffsetY, restartButtonRect.w, restartButtonRect.h };
            SDL_RenderCopy(renderer, restartButtonImageTexture, NULL, &restartButtonImageRect);

            SDL_Rect restartTextRect = { restartButtonImageRect.x + 75, restartButtonImageRect.y + 24, restartButtonImageRect.w - 150, restartButtonImageRect.h - 35 };
            SDL_RenderCopy(renderer, restartButtonTextTexture, NULL, &restartTextRect);

            float exit2OffsetX = 7.5 * sin(SDL_GetTicks() * 0.002);
            float exit2OffsetY = 7.5 * cos(SDL_GetTicks() * 0.002);

            SDL_Rect exit2ButtonImageRect = { exit2ButtonRect.x + (int)exit2OffsetX, exit2ButtonRect.y + (int)exit2OffsetY, exit2ButtonRect.w, exit2ButtonRect.h };
            SDL_RenderCopy(renderer, exit2ButtonImageTexture, NULL, &exit2ButtonImageRect);

            SDL_Rect exit2TextRect = { exit2ButtonImageRect.x + 80, exit2ButtonImageRect.y + 24, exit2ButtonImageRect.w - 160, exit2ButtonImageRect.h - 35 };
            SDL_RenderCopy(renderer, exit2ButtonTextTexture, NULL, &exit2TextRect);

            SDL_RenderCopy(renderer, ScoreBoardTexture, NULL, &ScoreBoardRect);


            sprintf(scoreText, "Score: %d", score);

            TTF_Font* font2 = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
            if (!font2) {
                printf("Unable to load font: %s\n", TTF_GetError());
                exit(1);
            }

            SDL_Color textColor2 = { 101,67,33};  // Brown color
            SDL_Surface* scoreTextSurface2 = TTF_RenderText_Solid(font2, scoreText, textColor2);
            if (!scoreTextSurface2) {
                printf("Unable to render score text surface: %s\n", TTF_GetError());
                exit(1);
            }

            SDL_Texture* scoreTextTexture2 = SDL_CreateTextureFromSurface(renderer, scoreTextSurface2);
            if (!scoreTextTexture2) {
                printf("Unable to create score text texture: %s\n", SDL_GetError());
                exit(1);
            }

            int scoreTextWidth2 = 150;
            int scoreTextHeight2 = 50;

            SDL_Rect scoreTextRect2 = { SCREEN_WIDTH/2-scoreTextWidth2/2, SCREEN_HEIGHT-450, scoreTextWidth2, scoreTextHeight2 };
            SDL_RenderCopy(renderer, scoreTextTexture2, NULL, &scoreTextRect2);

            SDL_FreeSurface(scoreTextSurface2);
            SDL_DestroyTexture(scoreTextTexture2);
            TTF_CloseFont(font2);

            // 判断最高分
            if(score>max){
                max=score;
            }

            sprintf(maxText, "MAX: %d", max);

            TTF_Font* font3 = TTF_OpenFont("res/fond/fond_surface.ttf", 20);
            if (!font3) {
                printf("Unable to load font: %s\n", TTF_GetError());
                exit(1);
            }

            SDL_Color textColor3 = { 101,67,33 };
            SDL_Surface* scoreTextSurface3 = TTF_RenderText_Solid(font3, maxText, textColor3);
            if (!scoreTextSurface3) {
                printf("Unable to render score text surface: %s\n", TTF_GetError());
                exit(1);
            }

            SDL_Texture* scoreTextTexture3 = SDL_CreateTextureFromSurface(renderer, scoreTextSurface3);
            if (!scoreTextTexture3) {
                printf("Unable to create score text texture: %s\n", SDL_GetError());
                exit(1);
            }

            int scoreTextWidth3 = 150;
            int scoreTextHeight3 = 50;

            SDL_Rect scoreTextRect3 = { SCREEN_WIDTH/2-scoreTextWidth2/2, SCREEN_HEIGHT-400, scoreTextWidth3, scoreTextHeight3 };
            SDL_RenderCopy(renderer, scoreTextTexture3, NULL, &scoreTextRect3);

            SDL_FreeSurface(scoreTextSurface3);
            SDL_DestroyTexture(scoreTextTexture3);
            TTF_CloseFont(font3);

            break;
    }
    SDL_RenderPresent(renderer);
}

// 释放内存
void cleanup() {
    SDL_DestroyTexture(Menu_backgroundImage);
    SDL_DestroyTexture(Game_backgroundImage1);
    SDL_DestroyTexture(Game_backgroundImage2);
    SDL_DestroyTexture(NewGame_backgroundImage1);
    SDL_DestroyTexture(NewGame_backgroundImage2);
    SDL_DestroyTexture(NewGame_backgroundImage3);
    SDL_DestroyTexture(NewGame_backgroundImage4);
    SDL_DestroyTexture(GameOver_backgroundImage);
    SDL_DestroyTexture(startButtonTextTexture);
    SDL_DestroyTexture(exitButtonTextTexture);
    SDL_DestroyTexture(restartButtonTextTexture);
    SDL_DestroyTexture(exit2ButtonTextTexture);
    SDL_DestroyTexture(startButtonImageTexture);
    SDL_DestroyTexture(exitButtonImageTexture);
    SDL_DestroyTexture(restartButtonImageTexture);
    SDL_DestroyTexture(exit2ButtonImageTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(TitleTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyTexture(blocks[numBlocks].texture);

    Mix_FreeChunk(jumpSound);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(portalSound);
    Mix_CloseAudio();

    TTF_Quit();
    SDL_Quit();
}

// 主函数
int main() {
    initialize();
    SDL_Event event;

    while (!shouldQuitGame) {
        handleInput(&event);

        if (gameState == GAME) {
            updatePlayer();
        }

        render();

        SDL_Delay(16);
    }

    cleanup();
    return 0;
}