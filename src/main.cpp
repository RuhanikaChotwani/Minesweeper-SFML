#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

using namespace sf;

static const int GRID = 10;
static const int TILE = 50;
static const int BOMBS = 10;

int dr[8] = {-1,-1,-1,0,0,1,1,1};
int dc[8] = {-1,0,1,-1,1,-1,0,1};

int main() {

    RenderWindow window(VideoMode(GRID*TILE, GRID*TILE), "Minesweeper");

    Font font;
    if(!font.loadFromFile("arial.ttf")){
        std::cout << "Font missing\n";
        return -1;
    }

    int board[GRID][GRID] = {0};
    int state[GRID][GRID] = {0};

    srand(time(NULL));

    for(int i=0;i<BOMBS;i++){
        board[rand()%GRID][rand()%GRID] = 9;
    }

    for(int r=0;r<GRID;r++){
        for(int c=0;c<GRID;c++){
            if(board[r][c] == 9) continue;
            int cnt = 0;
            for(int k=0;k<8;k++){
                int nr=r+dr[k], nc=c+dc[k];
                if(nr>=0 && nr<GRID && nc>=0 && nc<GRID && board[nr][nc]==9)
                    cnt++;
            }
            board[r][c] = cnt;
        }
    }

    bool gameOver = false;

    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed)
                window.close();

            if(!gameOver && event.type == Event::MouseButtonPressed){
                int c = event.mouseButton.x / TILE;
                int r = event.mouseButton.y / TILE;

                if(r<0||c<0||r>=GRID||c>=GRID) continue;

                if(event.mouseButton.button == Mouse::Right){
                    state[r][c] = (state[r][c]==2 ? 0 : 2);
                }

                if(event.mouseButton.button == Mouse::Left){
                    if(board[r][c] == 9)
                        gameOver = true;
                    else
                        state[r][c] = 1;
                }
            }
        }

        window.clear(Color::White);
        RectangleShape tile(Vector2f(TILE-2, TILE-2));

        for(int r=0;r<GRID;r++){
            for(int c=0;c<GRID;c++){
                tile.setPosition(c*TILE, r*TILE);
                tile.setOutlineThickness(1);
                tile.setOutlineColor(Color::Black);
                tile.setFillColor(state[r][c]==0 ? Color(160,160,160) : Color(220,220,220));
                window.draw(tile);

                if(state[r][c]==2){
                    Text f("F", font, 28);
                    f.setFillColor(Color::Red);
                    f.setPosition(c*TILE+15, r*TILE+5);
                    window.draw(f);
                }

                if(state[r][c]==1 && board[r][c]>0 && board[r][c]<9){
                    Text t(std::to_string(board[r][c]), font, 28);
                    t.setFillColor(Color::Blue);
                    t.setPosition(c*TILE+15, r*TILE+5);
                    window.draw(t);
                }

                if(gameOver && board[r][c]==9){
                    CircleShape b(12);
                    b.setFillColor(Color::Black);
                    b.setPosition(c*TILE+14, r*TILE+14);
                    window.draw(b);
                }
            }
        }

        window.display();
    }

    return 0;
}
