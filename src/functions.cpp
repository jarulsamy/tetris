#include "../include/tetris.hpp"
#include <fstream>
#include <ncurses.h>
#include <cstring>
#include <random>

using namespace std;

string readLine(string str, int n) {
   stringstream f(str);
   string s;
   for (int i = 0; i < n; i++) {
       getline(f, s);
   }

   getline(f,s);
   return s; 
}

Screen::Screen(string screenstr) {
    string line;
    stringstream f(screenstr);

    int lineNum = 0;

    while (getline(f, line)) {
        lineNum++;
    }

    for ( int x = 0; x < lineNum; x++ ) {
        vector<string> rowVec;
        string line = readLine(screenstr, x);
        for ( int i = 0; i < line.length(); i++ ) {
            rowVec.push_back(line.substr(i,1));
        }
        window.push_back(rowVec);
    }
}

void Screen::draw() {
    for ( int i = 0; i < window.size(); i++ ) {
        for ( int j = 0; j < window[i].size(); j++) {
            printw(window[i][j].c_str());
        }
        printw(string("\n").c_str());
    }
}

void Screen::updateScore(int score) {
    int x = 13;
    int y = 36;

    window[x][y] = to_string(score);

    for ( int z = 37; z < 42; z++ ) {
        window[x][z] = " ";
    }

    for ( int i = 1; i < to_string(score).length(); i++ ) {
        window[x][y + i] = "";
    }
}


void Screen::gameOver() {
    gameover = true;
    // window[9][12] = "die";
}


// basically a constructor to generate a new
// random shape and fill attributes:
void Shape::generate() {

    random_device rd; 
    mt19937 eng(rd()); 
    uniform_int_distribution<> distr(1, 7);

    int rand = distr(eng) - 1; 
    selected = shapecoords[rand];
    color = colors[rand];        

    // find the height of the shape

    for ( int row = 0; row < selected.size(); row++ ) {
        bool found = false;
        for ( int cell = 0; cell < selected[row].size(); cell++ ) {
            if (selected[row][cell] == 1) {
                found = true;
            }
        }
        if ( found ) {
            shapeHeight++;
        }
    }

    // can prob add something here to center shape; ie shapeWidth;
    defaultPos[1] -= shapeHeight;
    isdropping = shapeHeight;
}

string Screen::getChar(int x, int y) {
    x += defaultPos[0];
    y += defaultPos[1];

    return window[x][y];
}

void Shape::drop() {
    // bool cannotDrop;

    // // if any one of the shape's coordinates cannot move down the full amount, die:
    // for ( int i = 3; i > 0; i-- ) {
    //     for ( int x = 0; x < 4; x++ ) {
    //         // going up rows of shape, over cells
    //         if ( selected[i][x] ) {
    //             // this block needs to descend by 1; check coord below;
    //             int position[2] = { trCoord[0] + i - 1, trCoord[1] + x };
    //         }
    //     }
    // }
    
    // if ( cannotDrop ) {
    //     // gameOver();
    // }
    // else {
    //     trCoord[0] += 1;
    // }

    trCoord[0]++;

    isdropping--;
    mvprintw(0,0,to_string(isdropping).c_str());
}

void Shape::rotate() {
    // function to rotate the matrix
}

void Shape::draw() {
    if ( isdropping > 0 ) {
        // need to cut off something;
        // left = isdropping
        int draw = shapeHeight - isdropping;
        vector<vector<bool> > relevant;

        for ( int i = isdropping; i <= draw; i++ ) {
            relevant.push_back(selected[i]);
        }

        int currentPos[2] = { trCoord[0] + defaultPos[1] + 1, trCoord[1] + defaultPos[0]};

        init_pair(2, color, -1);
        attrset(COLOR_PAIR(2));

        for ( int i = 1; i < draw; i++  ) {
            // for each line;
            vector<bool> line = relevant[i];
            for ( int i = 0; i < 4; i++ ) {
                // for each el in line;
                if ( line[i] ) {
                    // need to draw two side by side fullblocks;
                    mvprintw(currentPos[0], currentPos[1], string("██").c_str());
                }
                currentPos[1] += 2;
            }
            currentPos[0] += 1;
            currentPos[1] = trCoord[1] + defaultPos[0];
        }
    }
    else {
        int currentPos[2] = { trCoord[0] + defaultPos[1], trCoord[1] + defaultPos[0]};

        init_pair(2, color, -1);
        attrset(COLOR_PAIR(2));

        for ( int i = 0; i < shapeHeight; i++  ) {
            // for each line;
            vector<bool> line = selected[i];
            for ( int i = 0; i < 4; i++ ) {
                // for each el in line;
                if ( line[i] ) {
                    // need to draw two side by side fullblocks;
                    mvprintw(currentPos[0], currentPos[1], string("██").c_str());
                }
                currentPos[1] += 2;
            }
            currentPos[0] += 1;
            currentPos[1] = trCoord[1] + defaultPos[0];
        }
    }
    init_pair(1, COLOR_WHITE, -1);
    attrset(COLOR_PAIR(1));
}

void Shape::fall() {

    // psuedocode:

     trCoord[0]++;   // just move down the shape, no need to draw here
    // if !( spaceBelowAfterMoveDown ) {
    //  //cannot move down any more, incorporate this one into the board
    //  screen.drawshape(selected, trcoord[0], trcoord[1]);
    //}

}

void Shape::move(int movetype) {

}

void Shape::ground(int framerate) {

}