#include<iostream>
#include<raylib.h>
#include<deque>
#include<raymath.h>

using namespace std;


Color Yellow = {173 , 204 , 96 ,255};
Color DarkGreen = {43 , 51 ,24 , 255};

int cellSize = 30;
int cellCount = 25;


double lastUpdateTime = 0;

bool eventTriggered(double interval){
    double currentTime = GetTime();
    
    if(currentTime - lastUpdateTime >= interval)
    {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 element , deque<Vector2> dq){

    for(unsigned int i=0;i<dq.size();i++){
        if(Vector2Equals(dq[i] , element)) return true;
    }
    return false;
}

class Snake{
    public:
    deque<Vector2> body = {Vector2{6, 9} , Vector2{5 , 9} , Vector2{4 , 9}};
    // Snake(){
    bool AddSegment = false;
    Vector2 direction = {1,0};

    // }
    void Draw(){
        for(unsigned int i=0;i<body.size();i++){
            int x = body[i].x;
            int y = body[i].y;
            Rectangle Segment = Rectangle{(float)x*cellSize , (float)y*cellSize , (float)cellSize , (float)cellSize};
            DrawRectangleRounded(Segment , 0.5 , 6 , DarkGreen);
        }
    }

    void Update(){
        body.push_front(Vector2Add(body[0] , direction));
        if(AddSegment == true){
            AddSegment = false;
        }
        else{
            body.pop_back();   
        }
    }

    void Reset(){
        body = {Vector2{6, 9} , Vector2{5 , 9} , Vector2{4 , 9}};
        direction = {1,0};
    }
};

class Food{
    public:
    Vector2 position;
    Texture2D texture;

    Food(deque<Vector2> snakeBody){
        Image image = LoadImage("./Apple.png"); 
        ImageResize(&image, cellSize, cellSize);
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
        position = GenerateRandomPos(snakeBody);

    }
    ~Food(){
        UnloadTexture(texture);
    }

    void Draw()
    {
        DrawTexture(texture , position.x*cellSize , position.y*cellSize , WHITE);
    }

    Vector2 GetRandomCell(){
        float x = GetRandomValue(0 , cellCount-1);
        float y = GetRandomValue(0 , cellCount-1);
        return Vector2{x , y};
    }
    Vector2 GenerateRandomPos(deque<Vector2> snakeBody){
        
        Vector2 position = GetRandomCell();
        while(ElementInDeque(position , snakeBody)){
            position = GetRandomCell();
        }
        return position;
    }
};

class Game{
    public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    bool running = true;

    void Draw(){
        food.Draw();
        snake.Draw();
    }

    void Update(){
        if(running){
            snake.Update();
            checkCollisionWithFood();
            checkGameOver();
            CheckCollisionWithTail();
        }
    }

    void checkCollisionWithFood(){
        if(Vector2Equals(snake.body[0] , food.position)){
            food.position = food.GenerateRandomPos(snake.body);
            snake.AddSegment = true;
        }
    }

    void checkGameOver(){
        if(snake.body[0].x == cellCount || snake.body[0].x == -1 ){
            GameOver();
        }
        if(snake.body[0].y == cellCount || snake.body[0].y == -1 ){
            GameOver();
        }

    }
    void GameOver(){
        snake.Reset();
        food.position = food.GenerateRandomPos(snake.body);
        running = false;
    }

    void CheckCollisionWithTail(){
        deque<Vector2> HeadLessBody = snake.body;
        HeadLessBody.pop_front();

        if(ElementInDeque(snake.body[0] , HeadLessBody)){
            GameOver();
        }
    }

};

int main(){
    
    cout<<"Starting the game....\n";
    InitWindow(cellSize*cellCount , cellSize*cellCount ,"Retro Snake");
    SetTargetFPS(60);

    Game game = Game();
    // Game Loop

    while(WindowShouldClose()==false){
        BeginDrawing();
       
        if(eventTriggered(0.2)){
             game.Update();
        }
        if(IsKeyPressed(KEY_UP) && game.snake.direction!=Vector2{0 , 1}){
            game.snake.direction = {0 , -1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_DOWN) && game.snake.direction!=Vector2{0 , -1}){
            game.snake.direction = {0 , 1};
            game.running = true;
        }
        if(IsKeyPressed(KEY_LEFT) && game.snake.direction!=Vector2{1 , 0}){
            game.snake.direction = {-1 , 0};
            game.running = true;
        }
        if(IsKeyPressed(KEY_RIGHT) && game.snake.direction!=Vector2{-1 , 0}){
            game.snake.direction = {1 , 0};
            game.running = true;
        }
        // Drawing
        ClearBackground(Yellow);
        game.Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;

}