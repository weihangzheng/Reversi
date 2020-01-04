#include <iostream>
#include <vector>
using namespace std;

enum class Cell {
    empty,
    black,
    white,
    potential
};

typedef vector<vector<Cell>> Board;     //makes the empty board to start with

const string red        = "\033[0;31m";
const string green      = "\033[1;32m";
const string yellow     = "\033[1;33m";
const string cyan       = "\033[0;36m";
const string magenta    = "\033[0;35m";
const string reset_color = "\033[0m";

const string empty_str = yellow + "." + reset_color;
const string white_str = red + "X" + reset_color;
const string black_str = green + "O" + reset_color;
const string potential_str = cyan + "!" + reset_color;

string to_str(Cell c){
    switch (c){
        case Cell::empty: return empty_str;
        case Cell::white: return white_str;
        case Cell::black: return black_str;
        case Cell::potential: return potential_str;
    }
    return "";
}

struct Coord {
    int x;
    int y;
};

class Player{
public:
    virtual void piece_track() = 0;
    virtual void receive_board(Board b) = 0;
    virtual Board give_board() = 0;
    virtual void Cclear_legal_Flips_PlayCoords() = 0;
    virtual vector<Coord> left_legal() = 0;
    virtual vector<Coord> right_legal() = 0;
    virtual vector<Coord> up_legal() = 0;
    virtual vector<Coord> down_legal() = 0;
    virtual vector<Coord> diagupleft_legal() = 0;
    virtual vector<Coord> diagupright_legal() = 0;
    virtual vector<Coord> diagdownleft_legal() = 0;
    virtual vector<Coord> diagdownright_legal() = 0;
    virtual void potential_flush() = 0;
    virtual bool is_not_zero() = 0;
    virtual ~Player(){};
};


class Human_player : public Player {
//: public Player {

private:
    int shiftcount = 0;
    vector<Coord> playcoords;   //list of the coordinates that the player has already played
    vector<int> leftcounts;     //list of #flips that correspond to each possible legal move in this direction
    vector<int> rightcounts;     
    vector<int> upcounts;
    vector<int> downcounts;
    vector<int> diagupleftcounts;
    vector<int> diaguprightcounts;
    vector<int> diagdownleftcounts;
    vector<int> diagdownrightcounts;

    vector<Coord> legal_moves;  //list of all possible legal moves in all directions on a given turn

    Board board;

public:

    Human_player()
    {}

    void piece_track(){ //keeps track of the coordinates of existing player pieces (black), by updating playcoords
        Coord c = {0, 0};
        for(int row = 0; row < 8; row++){
            for(int column = 0; column < 8; column++){
                if(board[row][column] == Cell::black){
                    c.x = column;
                    c.y = row;
                    playcoords.push_back(c);
                }
            }
        }
    }

    void receive_board(Board b){
        board = b;
        piece_track();
    }

    Board give_board(){
        return board;
    }

    void Cclear_legal_Flips_PlayCoords(){   //we must clear these vectors as the legal moves, # corresponding flips, and existing player pieces will differ after every move
        legal_moves.clear();    
        playcoords.clear();

        leftcounts.clear();     //clear the list of #flips that correspond to each possible legal move in every direction
        rightcounts.clear();     
        upcounts.clear();
        downcounts.clear();
        diagupleftcounts.clear();
        diaguprightcounts.clear();
        diagdownleftcounts.clear();
        diagdownrightcounts.clear();
    }

    void potential_flush(){
        for(int row = 0; row < 8; row++){
            for(int column = 0; column < 8; column++){
                if(board[row][column] == Cell::potential){
                    board[row][column] = Cell::empty;
                }
            }
        }
    }

    vector<Coord> left_legal(){
        vector<Coord> leftmoves;    //list of legal moves to the left of all your current pieces
        Coord leftmove;

        //iterates through left direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftleft = playcoords[i].x - 1;
            while(shiftleft > 0){
                if(board[playcoords[i].y][shiftleft] == Cell::empty || board[playcoords[i].y][shiftleft] == Cell::potential){
                    break;
                }
                else if(board[playcoords[i].y][shiftleft] == Cell::black){

                }
                else if(board[playcoords[i].y][shiftleft] == Cell::white){
                    shiftcount++;
                    if(board[playcoords[i].y][shiftleft-1] == Cell::empty || board[playcoords[i].y][shiftleft-1] == Cell::potential){
                        board[playcoords[i].y][shiftleft-1] = Cell::potential;
                        leftmove.x = shiftleft - 1;
                        leftmove.y = playcoords[i].y;
                        leftmoves.push_back(leftmove);

                        legal_moves.push_back(leftmove);

                        leftcounts.push_back(shiftcount);
                        break;
                    }
                }
                shiftleft--;
            }
        }
        return leftmoves;
    }

    vector<Coord> right_legal(){
        vector<Coord> rightmoves;
        Coord rightmove;

        //iterates through right direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftright = playcoords[i].x + 1;
            while(shiftright < 7){
                if(board[playcoords[i].y][shiftright] == Cell::empty || board[playcoords[i].y][shiftright] == Cell::potential){
                    break;
                }
                else if(board[playcoords[i].y][shiftright] == Cell::black){
                 
                }
                else if(board[playcoords[i].y][shiftright] == Cell::white){
                    shiftcount++;
                    if(board[playcoords[i].y][shiftright+1] == Cell::empty || board[playcoords[i].y][shiftright+1] == Cell::potential){
                        board[playcoords[i].y][shiftright+1] = Cell::potential;
                        rightmove.x = shiftright + 1;
                        rightmove.y = playcoords[i].y;
                        rightmoves.push_back(rightmove);

                        legal_moves.push_back(rightmove);

                        rightcounts.push_back(shiftcount);
                        break;
                    }
                }
                shiftright++;
            }
        }
        return rightmoves;
    }

    vector<Coord> up_legal(){
        vector<Coord> upmoves;
        Coord upmove;

        //iterates through up direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftup = playcoords[i].y - 1;
            while(shiftup > 0){
                if(board[shiftup][playcoords[i].x] == Cell::empty || board[shiftup][playcoords[i].x] == Cell::potential){
                    break;
                }
                else if(board[shiftup][playcoords[i].x] == Cell::black){
                 
                }
                else if(board[shiftup][playcoords[i].x] == Cell::white){
                    shiftcount++;
                    if(board[shiftup-1][playcoords[i].x] == Cell::empty || board[shiftup-1][playcoords[i].x] == Cell::potential){
                        board[shiftup-1][playcoords[i].x] = Cell::potential;
                        upmove.x = playcoords[i].x;
                        upmove.y = shiftup - 1;
                        upmoves.push_back(upmove);

                        legal_moves.push_back(upmove);

                        upcounts.push_back(shiftcount);
                        break;
                    }
                }
                shiftup--;
            }
        }
        return upmoves;
    }

    vector<Coord> down_legal(){
        vector<Coord> downmoves;
        Coord downmove;

        //iterates through down direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftdown = playcoords[i].y + 1;
            while(shiftdown < 7){
                if(board[shiftdown][playcoords[i].x] == Cell::empty || board[shiftdown][playcoords[i].x] == Cell::potential){
                    break;
                }
                else if(board[shiftdown][playcoords[i].x] == Cell::black){
                 
                }
                else if(board[shiftdown][playcoords[i].x] == Cell::white){
                    shiftcount++;
                    if(board[shiftdown+1][playcoords[i].x] == Cell::empty || board[shiftdown+1][playcoords[i].x] == Cell::potential){
                        board[shiftdown+1][playcoords[i].x] = Cell::potential;
                        downmove.x = playcoords[i].x;
                        downmove.y = shiftdown + 1;
                        downmoves.push_back(downmove);

                        legal_moves.push_back(downmove);

                        downcounts.push_back(shiftcount);   //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftdown++;
            }
        }
        return downmoves;
    }

    vector<Coord> diagupleft_legal(){
        vector<Coord> diagupleftmoves;
        Coord diagupleftmove;

        //iterates through upleft direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftup1 = playcoords[i].y - 1;
            int shiftleft1 = playcoords[i].x - 1;
            while(shiftup1 > 0 && shiftleft1 > 0){
                if(board[shiftup1][shiftleft1] == Cell::empty || board[shiftup1][shiftleft1] == Cell::potential){
                    break;
                }
                else if(board[shiftup1][shiftleft1] == Cell::black){
                 
                }
                else if(board[shiftup1][shiftleft1] == Cell::white){
                    shiftcount++;
                    if(board[shiftup1-1][shiftleft1-1] == Cell::empty || board[shiftup1-1][shiftleft1-1] == Cell::potential){   //change to Cell:empty and then manually turn that cell into Cell::potential
                        board[shiftup1-1][shiftleft1-1] = Cell::potential;
                        diagupleftmove.x = shiftleft1-1;
                        diagupleftmove.y = shiftup1 - 1;
                        diagupleftmoves.push_back(diagupleftmove);

                        legal_moves.push_back(diagupleftmove);

                        diagupleftcounts.push_back(shiftcount); //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftup1--;
                shiftleft1--;
            }
        }
        return diagupleftmoves;
    }

    vector<Coord> diagupright_legal(){
        vector<Coord> diaguprightmoves;
        Coord diaguprightmove;

        //iterates through upright direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftup2 = playcoords[i].y - 1;
            int shiftright1 = playcoords[i].x + 1;
            while(shiftup2 > 0 && shiftright1 < 7){
                if(board[shiftup2][shiftright1] == Cell::empty || board[shiftup2][shiftright1] == Cell::potential){
                    break;
                }
                else if(board[shiftup2][shiftright1] == Cell::black){
                 
                }
                else if(board[shiftup2][shiftright1] == Cell::white){
                    shiftcount++;
                    if(board[shiftup2-1][shiftright1+1] == Cell::empty || board[shiftup2-1][shiftright1+1] == Cell::potential){ //change to Cell:empty and then manually turn that cell into Cell::potential
                        board[shiftup2-1][shiftright1+1] = Cell::potential;
                        diaguprightmove.x = shiftright1+1;
                        diaguprightmove.y = shiftup2 - 1;

                        legal_moves.push_back(diaguprightmove);

                        diaguprightmoves.push_back(diaguprightmove);
                        diaguprightcounts.push_back(shiftcount);    //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftup2--;
                shiftright1++;
            }
        }
        return diaguprightmoves;
    }

    vector<Coord> diagdownleft_legal(){
        vector<Coord> diagdownleftmoves;
        Coord diagdownleftmove;

        //iterates through downleft direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftdown1 = playcoords[i].y + 1;
            int shiftleft2 = playcoords[i].x - 1;
            while(shiftdown1 < 7 && shiftleft2 > 0){
                if(board[shiftdown1][shiftleft2] == Cell::empty || board[shiftdown1][shiftleft2] == Cell::potential){
                    break;
                }
                else if(board[shiftdown1][shiftleft2] == Cell::black){
                 
                }
                else if(board[shiftdown1][shiftleft2] == Cell::white){
                    shiftcount++;
                    if(board[shiftdown1+1][shiftleft2-1] == Cell::empty || board[shiftdown1+1][shiftleft2-1] == Cell::potential){   //change to Cell:empty and then manually turn that cell into Cell::potential
                        board[shiftdown1+1][shiftleft2-1] = Cell::potential;
                        diagdownleftmove.x = shiftleft2-1;
                        diagdownleftmove.y = shiftdown1 + 1;

                        legal_moves.push_back(diagdownleftmove);

                        diagdownleftmoves.push_back(diagdownleftmove);
                        diagdownleftcounts.push_back(shiftcount);   //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftdown1++;
                shiftleft2--;
            }
        }
        return diagdownleftmoves;
    }

    vector<Coord> diagdownright_legal(){
        vector<Coord> diagdownrightmoves;
        Coord diagdownrightmove;

        //iterates through downright direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftdown2 = playcoords[i].y + 1;
            int shiftright2 = playcoords[i].x + 1;
            while(shiftdown2 < 7 && shiftright2 < 7){
                if(board[shiftdown2][shiftright2] == Cell::empty || board[shiftdown2][shiftright2] == Cell::potential){
                    break;
                }
                else if(board[shiftdown2][shiftright2] == Cell::black){
                 
                }
                else if(board[shiftdown2][shiftright2] == Cell::white){
                    shiftcount++;
                    if(board[shiftdown2+1][shiftright2+1] == Cell::empty || board[shiftdown2+1][shiftright2+1] == Cell::potential){ //change to Cell:empty and then manually turn that cell into Cell::potential
                        board[shiftdown2+1][shiftright2+1] = Cell::potential;
                        diagdownrightmove.x = shiftright2+1;
                        diagdownrightmove.y = shiftdown2+1;

                        legal_moves.push_back(diagdownrightmove);

                        diagdownrightmoves.push_back(diagdownrightmove);
                        diagdownrightcounts.push_back(shiftcount);  //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftdown2++;
                shiftright2++;
            }
        }
        return diagdownrightmoves;
    }

    void makemove(int x , int y){
        Coord a;
        bool flag = false;
        while(!flag){
            a = {x - 1, y - 1};
            vector<Coord> lefttrial = left_legal();
            for(int o = 0; o < lefttrial.size(); o++){   
                if(a.x == lefttrial[o].x && a.y == lefttrial[o].y){
                    for(int v = a.x+1; v <= a.x + leftcounts[o]; v++){    
                        board[a.y][v] = Cell::black;
                    }
                    playcoords.push_back(a);
                    break;
                }
            }
         
            vector<Coord> righttrial = right_legal();
            for(int p = 0; p < righttrial.size(); p++){  
                if(a.x == righttrial[p].x && a.y == righttrial[p].y){
                    for(int d = a.x-1; d >= a.x - rightcounts[p]; d--){
                        board[a.y][d] = Cell::black;
                    }
                    playcoords.push_back(a);                   
                    break;
                }
            }

            vector<Coord> uptrial = up_legal();
            for(int j = 0; j < uptrial.size(); j++){     
                if(a.x == uptrial[j].x && a.y == uptrial[j].y){
                    for(int h = a.y+1; h <= a.y+upcounts[j]; h++){
                        board[h][x-1] = Cell::black;
                    }
                    playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                    break;
                }
            }

            vector<Coord> downtrial = down_legal();     //assigns list of legal downward moves
            for(int t = 0; t < downtrial.size(); t++){   
                if(a.x == downtrial[t].x && a.y == downtrial[t].y){     //if inputted player coordinate is legal
                    for(int k = a.y-1; k >= a.y-downcounts[t]; k--){
                        board[k][x-1] = Cell::black;
                    }
                    playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                    break;
                }
            }
         
            vector<Coord> diaguplefttrial = diagupleft_legal();     //assigns list of legal downward moves
            for(int w = 0; w < diaguplefttrial.size(); w++){     
                if(a.x == diaguplefttrial[w].x && a.y == diaguplefttrial[w].y){     //if inputted player coordinate is legal
                    for(int f = a.x+1, q = a.y+1; f <= a.x + diagupleftcounts[w], q <= a.y + diagupleftcounts[w]; f++, q++){
                        board[q][f] = Cell::black;
                    }
                    playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                    break;
                }
            }
        
            vector<Coord> diaguprighttrial = diagupright_legal();       //assigns list of legal downward moves
            for(int r = 0; r < diaguprighttrial.size(); r++){    
                if(a.x == diaguprighttrial[r].x && a.y == diaguprighttrial[r].y){       //if inputted player coordinate is legal
                    for(int u = a.x-1, w = a.y+1; u >= a.x - diaguprightcounts[r], w <= a.y + diaguprightcounts[r]; u--, w++){
                        board[w][u] = Cell::black;
                    }
                    playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                    break;
                }
            }

            vector<Coord> diagdownlefttrial = diagdownleft_legal();     //assigns list of legal downward moves
            for(int r1 = 0; r1 < diagdownlefttrial.size(); r1++){    
                if(a.x == diagdownlefttrial[r1].x && a.y == diagdownlefttrial[r1].y){       //if inputted player coordinate is legal
                    for(int u1 = a.x+1, w1 = a.y-1; u1 <= a.x + diagdownleftcounts[r1], w1 >= a.y - diagdownleftcounts[r1]; u1++, w1--){
                        board[w1][u1] = Cell::black;
                    }
                    playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                    break;
                }
            }

            vector<Coord> diagdownrighttrial = diagdownright_legal();       //assigns list of legal downward moves
            for(int r2 = 0; r2 < diagdownrighttrial.size(); r2++){   
                if(a.x == diagdownrighttrial[r2].x && a.y == diagdownrighttrial[r2].y){     //if inputted player coordinate is legal
                    for(int u2 = a.x-1, w2 = a.y-1; u2 >= a.x - diagdownrightcounts[r2], w2 >= a.y - diagdownrightcounts[r2]; u2--, w2--){
                        board[w2][u2] = Cell::black;
                    }
                    playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                    break;
                }
            }

            bool flag1 = true;
            for(int h = 0; h < legal_moves.size(); h++){
                if(legal_moves[h].x == a.x && legal_moves[h].y == a.y){
                    flag1 = false;		//if input is legal, don't ask user for input again
                    flag = true;	//end the while loop and implement the move
                    break;
                }
            }
            string first1;
            string second1;
            if(flag1){
                bool FLAG = true;
                bool FLAG1 = true;
                while(FLAG){
                    FLAG1 = true;
                    cout << "Please enter a valid x coordinate: " << "\n";      //guards against invalid input by making input a string and then stoi to int if it is a valid coordinate
                    cin >> first1;

                    for(int i = 0; i < first1.size(); i++){
                        if(first1[i] < 48 || first1[i] > 56){   //ASCII code for 0 to 8
                            FLAG1 = false;
                            break;
                        }
                    }
                    if(FLAG1 == true){
                        x = stoi(first1);
                        break;
                    }
                }

                FLAG = true;
                FLAG1 = true;
                while(FLAG){
                    FLAG1 = true;
                    cout << "Please enter a valid y coordinate: " << "\n";
                    cin >> second1;

                    for(int i = 0; i < second1.size(); i++){
                        if(second1[i] < 48 || second1[i] > 56){   //ASCII code for 0 to 8
                            FLAG1 = false;
                            break;
                        }
                    }

                    if(FLAG1 == true){
                        y = stoi(second1);
                        break;
                    }
                }  
            }
        }
        board[a.y][a.x] = Cell::black;
    }

    bool is_not_zero(){
        if(legal_moves.empty()){
            return false;
        }
        return true;
    }

    ~Human_player(){}
};

class Computer_player : public Player{
//: public Player {

private:
    int shiftcount = 0;
    vector<Coord> playcoords;   //list of the coordinates that the computer has already played

    vector<int> leftcounts;     //list of #flips that correspond to each possible legal move in this direction
    vector<int> rightcounts;     
    vector<int> upcounts;
    vector<int> downcounts;
    vector<int> diagupleftcounts;
    vector<int> diaguprightcounts;
    vector<int> diagdownleftcounts;
    vector<int> diagdownrightcounts;

    vector<Coord> legal_moves;  //list of all possible legal moves in all directions on a given turn
    vector<int> list_flips;     //list of all #flips that correspond to all the legal moves in all directions on a given turn
    //go through listflips to find the biggest flip, then use the same index and retrieve the corresponding legal move in legalmoves (the two vectors are aligned)

    Board board;

public:

    Computer_player()
    {}

    void piece_track(){ //keeps track of the coordinates of existing player pieces (white), by updating playcoords
        Coord d = {0,0};
        for(int row = 0; row < 8; row++){
            for(int column = 0; column < 8; column++){
                if(board[row][column] == Cell::white){
                    d.x = column;
                    d.y = row;
                    playcoords.push_back(d);
                }
            }
        }
    }

    void receive_board(Board b){
        board = b;
        piece_track();
    }

    Board give_board(){
        return board;
    }

    void Cclear_legal_Flips_PlayCoords(){   //we must clear these vectors as the legal moves, # corresponding flips, and existing player pieces will differ after every move
        legal_moves.clear();    
        list_flips.clear();
        playcoords.clear();

        leftcounts.clear();     //clear the list of #flips that correspond to each possible legal move in every direction
        rightcounts.clear();     
        upcounts.clear();
        downcounts.clear();
        diagupleftcounts.clear();
        diaguprightcounts.clear();
        diagdownleftcounts.clear();
        diagdownrightcounts.clear();
    }

    void potential_flush(){		//clear the board of any potential symbols
        for(int row = 0; row < 8; row++){
            for(int column = 0; column < 8; column++){
                if(board[row][column] == Cell::potential){
                    board[row][column] = Cell::empty;
                }
            }
        }
    }

    vector<Coord> left_legal(){
        vector<Coord> leftmoves;    //list of legal moves to the left of all your current pieces
        Coord leftmove;

        //iterates through left direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftleft = playcoords[i].x - 1;
            while(shiftleft > 0){
                if(board[playcoords[i].y][shiftleft] == Cell::empty || board[playcoords[i].y][shiftleft] == Cell::potential){
                    break;
                }
                else if(board[playcoords[i].y][shiftleft] == Cell::white){

                }
                else if(board[playcoords[i].y][shiftleft] == Cell::black){
                    shiftcount++;
                    if(board[playcoords[i].y][shiftleft-1] == Cell::empty || board[playcoords[i].y][shiftleft-1] == Cell::potential){
                        leftmove.x = shiftleft - 1;
                        leftmove.y = playcoords[i].y;
                        leftmoves.push_back(leftmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(leftmove);

                        leftcounts.push_back(shiftcount);
                        break;
                    }
                }
                shiftleft--;
            }
        }
        return leftmoves;
    }

    vector<Coord> right_legal(){
        vector<Coord> rightmoves;
        Coord rightmove;

        //iterates through right direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftright = playcoords[i].x + 1;
            while(shiftright < 7){
                if(board[playcoords[i].y][shiftright] == Cell::empty || board[playcoords[i].y][shiftright] == Cell::potential){
                    break;
                }
                else if(board[playcoords[i].y][shiftright] == Cell::white){
                 
                }
                else if(board[playcoords[i].y][shiftright] == Cell::black){
                    shiftcount++;
                    if(board[playcoords[i].y][shiftright+1] == Cell::empty || board[playcoords[i].y][shiftright+1] == Cell::potential){
                        rightmove.x = shiftright + 1;
                        rightmove.y = playcoords[i].y;
                        rightmoves.push_back(rightmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(rightmove);

                        rightcounts.push_back(shiftcount);
                        break;
                    }
                }
                shiftright++;
            }
        }
        return rightmoves;
    }

    vector<Coord> up_legal(){
        vector<Coord> upmoves;
        Coord upmove;

        //iterates through up direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftup = playcoords[i].y - 1;
            while(shiftup > 0){
                if(board[shiftup][playcoords[i].x] == Cell::empty || board[shiftup][playcoords[i].x] == Cell::potential){
                    break;
                }
                else if(board[shiftup][playcoords[i].x] == Cell::white){
                 
                }
                else if(board[shiftup][playcoords[i].x] == Cell::black){
                    shiftcount++;
                    if(board[shiftup-1][playcoords[i].x] == Cell::empty || board[shiftup-1][playcoords[i].x] == Cell::potential){
                        upmove.x = playcoords[i].x;
                        upmove.y = shiftup - 1;
                        upmoves.push_back(upmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(upmove);

                        upcounts.push_back(shiftcount);
                        break;
                    }
                }
                shiftup--;
            }
        }
        return upmoves;
    }

    vector<Coord> down_legal(){
        vector<Coord> downmoves;
        Coord downmove;

        //iterates through down direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftdown = playcoords[i].y + 1;
            while(shiftdown < 7){
                if(board[shiftdown][playcoords[i].x] == Cell::empty || board[shiftdown][playcoords[i].x] == Cell::potential){
                    break;
                }
                else if(board[shiftdown][playcoords[i].x] == Cell::white){
                 
                }
                else if(board[shiftdown][playcoords[i].x] == Cell::black){
                    shiftcount++;
                    if(board[shiftdown+1][playcoords[i].x] == Cell::empty || board[shiftdown+1][playcoords[i].x] == Cell::potential){
                        downmove.x = playcoords[i].x;
                        downmove.y = shiftdown + 1;
                        downmoves.push_back(downmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(downmove);

                        downcounts.push_back(shiftcount);   //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftdown++;
            }
        }
        return downmoves;
    }

    vector<Coord> diagupleft_legal(){
        vector<Coord> diagupleftmoves;
        Coord diagupleftmove;

        //iterates through upleft direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftup1 = playcoords[i].y - 1;
            int shiftleft1 = playcoords[i].x - 1;
            while(shiftup1 > 0 && shiftleft1 > 0){
                if(board[shiftup1][shiftleft1] == Cell::empty || board[shiftup1][shiftleft1] == Cell::potential){
                    break;
                }
                else if(board[shiftup1][shiftleft1] == Cell::white){
                 
                }
                else if(board[shiftup1][shiftleft1] == Cell::black){
                    shiftcount++;
                    if(board[shiftup1-1][shiftleft1-1] == Cell::empty || board[shiftup1-1][shiftleft1-1] == Cell::potential){   //change to Cell:empty and then manually turn that cell into Cell::potential
                        diagupleftmove.x = shiftleft1-1;
                        diagupleftmove.y = shiftup1 - 1;
                        diagupleftmoves.push_back(diagupleftmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(diagupleftmove);

                        diagupleftcounts.push_back(shiftcount); //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftup1--;
                shiftleft1--;
            }
        }
        return diagupleftmoves;
    }

    vector<Coord> diagupright_legal(){
        vector<Coord> diaguprightmoves;
        Coord diaguprightmove;

        //iterates through upright direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftup2 = playcoords[i].y - 1;
            int shiftright1 = playcoords[i].x + 1;
            while(shiftup2 > 0 && shiftright1 < 7){
                if(board[shiftup2][shiftright1] == Cell::empty || board[shiftup2][shiftright1] == Cell::potential){
                    break;
                }
                else if(board[shiftup2][shiftright1] == Cell::white){
                 
                }
                else if(board[shiftup2][shiftright1] == Cell::black){
                    shiftcount++;
                    if(board[shiftup2-1][shiftright1+1] == Cell::empty || board[shiftup2-1][shiftright1+1] == Cell::potential){ //change to Cell:empty and then manually turn that cell into Cell::potential
                        diaguprightmove.x = shiftright1+1;
                        diaguprightmove.y = shiftup2 - 1;
                        diaguprightmoves.push_back(diaguprightmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(diaguprightmove);

                        diaguprightcounts.push_back(shiftcount);    //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftup2--;
                shiftright1++;
            }
        }
        return diaguprightmoves;
    }

    vector<Coord> diagdownleft_legal(){
        vector<Coord> diagdownleftmoves;
        Coord diagdownleftmove;

        //iterates through downleft direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftdown1 = playcoords[i].y + 1;
            int shiftleft2 = playcoords[i].x - 1;
            while(shiftdown1 < 7 && shiftleft2 > 0){
                if(board[shiftdown1][shiftleft2] == Cell::empty || board[shiftdown1][shiftleft2] == Cell::potential){
                    break;
                }
                else if(board[shiftdown1][shiftleft2] == Cell::white){
                 
                }
                else if(board[shiftdown1][shiftleft2] == Cell::black){
                    shiftcount++;
                    if(board[shiftdown1+1][shiftleft2-1] == Cell::empty || board[shiftdown1+1][shiftleft2-1] == Cell::potential){   //change to Cell:empty and then manually turn that cell into Cell::potential
                        diagdownleftmove.x = shiftleft2-1;
                        diagdownleftmove.y = shiftdown1 + 1;
                        diagdownleftmoves.push_back(diagdownleftmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(diagdownleftmove);

                        diagdownleftcounts.push_back(shiftcount);   //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftdown1++;
                shiftleft2--;
            }
        }
        return diagdownleftmoves;
    }

    vector<Coord> diagdownright_legal(){
        vector<Coord> diagdownrightmoves;
        Coord diagdownrightmove;

        //iterates through downright direction
        for(int i = 0; i < playcoords.size(); i++){
            shiftcount = 0;
            int shiftdown2 = playcoords[i].y + 1;
            int shiftright2 = playcoords[i].x + 1;
            while(shiftdown2 < 7 && shiftright2 < 7){
                if(board[shiftdown2][shiftright2] == Cell::empty || board[shiftdown2][shiftright2] == Cell::potential){
                    break;
                }
                else if(board[shiftdown2][shiftright2] == Cell::white){
                 
                }
                else if(board[shiftdown2][shiftright2] == Cell::black){
                    shiftcount++;
                    if(board[shiftdown2+1][shiftright2+1] == Cell::empty || board[shiftdown2+1][shiftright2+1] == Cell::potential){ //change to Cell:empty and then manually turn that cell into Cell::potential
                        diagdownrightmove.x = shiftright2+1;
                        diagdownrightmove.y = shiftdown2+1;
                        diagdownrightmoves.push_back(diagdownrightmove);

                        list_flips.push_back(shiftcount);
                        legal_moves.push_back(diagdownrightmove);

                        diagdownrightcounts.push_back(shiftcount);  //the legal downmoves and their corresponding number of opponent pieces flipped are aligned in two vectors
                        break;
                    }
                }
                shiftdown2++;
                shiftright2++;
            }
        }
        return diagdownrightmoves;
    }


    Coord best_move(){
       Coord b = {0,0};

        //1st priority: take corners if legal
        bool flag = true;
        for(int i = 0; i < legal_moves.size(); i++){
            if((legal_moves[i].x == 0 && legal_moves[i].y == 0) || (legal_moves[i].x == 7 && legal_moves[i].y == 0) || (legal_moves[i].x == 0 && legal_moves[i].y == 7) || (legal_moves[i].x == 7 && legal_moves[i].y == 7)){
                b = legal_moves[i];
                flag = false;
                break;
            }
        }

        //2nd priority: make the move that flips the most pieces
        if(flag){
            int maxflips = 0;
            int maxindex = 0;
            int flips = 0;

            for(int n = 0; n < legal_moves.size(); n++){
                flips = list_flips[n];
                if(flips > maxflips){
                    maxflips = flips;
                    maxindex = n;
                }
            }
            b = legal_moves[maxindex];
        }
        return b;
    }

    void makemove(){
        vector<Coord> lefttrial = left_legal();
        vector<Coord> righttrial = right_legal();
        vector<Coord> uptrial = up_legal();\
        vector<Coord> downtrial = down_legal();
        vector<Coord> diaguplefttrial = diagupleft_legal();
        vector<Coord> diaguprighttrial = diagupright_legal();       
        vector<Coord> diagdownlefttrial = diagdownleft_legal();     
        vector<Coord> diagdownrighttrial = diagdownright_legal();       

        Coord a = best_move();
        int x = a.x;
        int y = a.y;

        for(int o = 0; o < lefttrial.size(); o++){   
            if(x == lefttrial[o].x && y == lefttrial[o].y){
                board[y][x] = Cell::white;
                for(int v = x; v <= x + leftcounts[o]; v++){    
                    board[y][v] = Cell::white;
                }
                playcoords.push_back(a);
                break;
            }
        }
     
        for(int p = 0; p < righttrial.size(); p++){  
            if(x == righttrial[p].x && y == righttrial[p].y){
                board[y][x] = Cell::white;
                for(int d = x; d >= x-rightcounts[p]; d--){
                board[y][d] = Cell::white;
                }
                playcoords.push_back(a);
                break;
            }
        }

        for(int j = 0; j < uptrial.size(); j++){     
            if(a.x == uptrial[j].x && a.y == uptrial[j].y){
                board[y][x] = Cell::white;
                for(int h = y; h <= y+upcounts[j]; h++){
                    board[h][x] = Cell::white;
                }
                playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                break;
            }
        }
    

        for(int t = 0; t < downtrial.size(); t++){   
            if(x == downtrial[t].x && y == downtrial[t].y){     //if inputted player coordinate is legal
                board[y][x] = Cell::white;
                for(int k = y; k >= y-downcounts[t]; k--){
                    board[k][x] = Cell::white;
                }
                playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                break;
            }
        }
     
        for(int w = 0; w < diaguplefttrial.size(); w++){     
            if(x == diaguplefttrial[w].x && y == diaguplefttrial[w].y){     //if inputted player coordinate is legal
                board[y][x] = Cell::white;
                for(int f = x, q = y; f <= x + diagupleftcounts[w], q <= y + diagupleftcounts[w]; f++, q++){
                    board[q][f] = Cell::white;
                }
                playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                break;
            }
        }

        for(int r = 0; r < diaguprighttrial.size(); r++){    
            if(x == diaguprighttrial[r].x && y == diaguprighttrial[r].y){       //if inputted player coordinate is legal
                board[y][x] = Cell::white;
                for(int u = x, w = y; u >= x - diaguprightcounts[w], w <= y + diaguprightcounts[w]; u--, w++){
                    board[w][u] = Cell::white;
                }
                playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                break;
            }
        }

        for(int r1 = 0; r1 < diagdownlefttrial.size(); r1++){    
            if(x == diagdownlefttrial[r1].x && y == diagdownlefttrial[r1].y){       //if inputted player coordinate is legal
                board[y][x] = Cell::white;
                for(int u1 = x, w1 = y; u1 <= x + diagdownleftcounts[r1], w1 >= y - diagdownleftcounts[r1]; u1++, w1--){
                    board[w1][u1] = Cell::white;
                }
                playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                break;
            }
        }

        for(int r2 = 0; r2 < diagdownrighttrial.size(); r2++){   
            if(x == diagdownrighttrial[r2].x && y == diagdownrighttrial[r2].y){     //if inputted player coordinate is legal
                board[y][x] = Cell::white;
                for(int u2 = x, w2 = y; u2 >= x - diagdownrightcounts[r2], w2 >= y - diagdownrightcounts[r2]; u2--, w2--){
                    board[w2][u2] = Cell::white;
                }
                playcoords.push_back(a);    //place this coordinate into the list of player piece coordinates on the board
                break;
            }
        }
    }

    bool is_not_zero(){
        if(legal_moves.empty())
            return false;
        return true;
    }

    ~Computer_player(){}
};



Board make_empty_board(){
    Board result = {
        {Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::empty, Cell::potential, Cell::empty, Cell::empty, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::potential, Cell::white, Cell::black, Cell::empty, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::empty, Cell::black, Cell::white, Cell::potential, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::potential, Cell::empty, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},    
        {Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty, Cell::empty},    
    };
    return result;
}

int count(const Board& board, Cell a){  //counts the number of pieces (black, white) on the board of that type
    int result = 0;
    for(const vector<Cell>& row : board){
        for(Cell c : row){
            if(c == a){
                result++;
            }
        }
    }
    return result;
}

void display(const Board& board){   //prints the board set up
    cout << "    ";
    for(int i = 0; i < 8; i++){
        cout << (i+1) << " ";
    }
    cout << "\n";

    int r = 1;
    for(const vector<Cell>& row : board){
        cout << "  " << r << " ";
        r++;
        for(Cell c : row){
            cout << to_str(c) << " ";
        }
        cout << "\n";
    }

    cout << "\n"
    << white_str << " " << count(board, Cell::white)
    << "    "
    << black_str << " " << count(board, Cell::black)
    << "\n";
}



int main(){ 
    string first1;
    string second1;   
    int first;
    int second;
    Board board = make_empty_board();
    Human_player a;
    Computer_player b;
    display(board);
    vector<Coord> temp;

    a.receive_board(board);     //player gets the board

    bool flag = true;
    bool flag1 = true;
    while(flag){
        flag1 = true;
        cout << "Please enter a valid x coordinate: " << "\n";
        cin >> first1;

        for(int i = 0; i < first1.size(); i++){
            if(first1[i] < 48 || first1[i] > 56){   //ASCII code for 0 to 8
                flag1 = false;
                break;
            }
        }
        if(flag1 == true){
            first = stoi(first1);
            break;
        }
    }

    flag = true;
    flag1 = true;
    while(flag){
        flag1 = true;
        cout << "Please enter a valid y coordinate: " << "\n";
        cin >> second1;

        for(int i = 0; i < second1.size(); i++){
            if(second1[i] < 48 || second1[i] > 56){   //ASCII code for 0 to 8
                flag1 = false;
                break;
            }
        }

        if(flag1 == true){
            second = stoi(second1);
            break;
        }
    }  

    a.makemove(first, second);      //implement the player move
    a.potential_flush();            //get rid of the lingering potential symbols
    display(a.give_board());        //print the current board
    board = a.give_board();
////////////////////////////////////////////////////////////////////////////////////////////////////////
    b.receive_board(a.give_board());    //computer gets the board
    temp = b.left_legal();
    temp = b.right_legal();
    temp = b.up_legal();
    temp = b.down_legal();
    temp = b.diagupleft_legal();
    temp = b.diagupright_legal();
    temp = b.diagdownleft_legal();
    temp = b.diagdownright_legal();
    b.makemove();                       //implement the computer move
    board = b.give_board();         

////////////////////////////////////////////////////////////////////////////////////////////////////////
    while(a.is_not_zero() && b.is_not_zero()){
        if(a.is_not_zero()){
            a.Cclear_legal_Flips_PlayCoords();      //clear all vectors in player
            a.receive_board(b.give_board());     //player gets the board

            temp = a.left_legal();
            temp = a.right_legal();
            temp = a.up_legal();
            temp = a.down_legal();
            temp = a.diagupleft_legal();
            temp = a.diagupright_legal();
            temp = a.diagdownleft_legal();
            temp = a.diagdownright_legal();

            display(a.give_board());
            if(a.is_not_zero()){
                flag = true;
                flag1= true;
                while(flag){
                    flag1 = true;
                    cout << "Please enter a valid x coordinate: " << "\n";
                    cin >> first1;

                    for(int i = 0; i < first1.size(); i++){
                        if(first1[i] < 48 || first1[i] > 56){   //ASCII code for 0 to 8
                            flag1 = false;
                            break;
                        }
                    }
                    if(flag1 == true){
                        first = stoi(first1);
                        break;
                    }
                }

                flag = true;
                flag1 = true;
                while(flag){
                    flag1 = true;
                    cout << "Please enter a valid y coordinate: " << "\n";
                    cin >> second1;

                    for(int i = 0; i < second1.size(); i++){
                        if(second1[i] < 48 || second1[i] > 56){   //ASCII code for 0 to 8
                            flag1 = false;
                            break;
                        }
                    }

                    if(flag1 == true){
                        second = stoi(second1);
                        break;
                    }
                }                

                a.makemove(first, second);      //implement the player move
                a.potential_flush();            //get rid of the lingering potential symbols
                display(a.give_board());        //print the current board
                board = a.give_board();
            }
        }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(b.is_not_zero()){
            b.Cclear_legal_Flips_PlayCoords();  //clear all vectors in computer   
            b.receive_board(a.give_board());    //computer gets the board
            b.makemove();                       //implement the computer move
            board = b.give_board();         
        }

    }
    int p_count = count(board, Cell::black);
    int c_count = count(board, Cell::white);
    if(p_count > c_count){
        cout << "Player wins! Good job you beat the computer." << "\n";
    }
    else if(c_count > p_count){
        cout << "Computer wins! Oh no! You lost to the computer." << "\n";
    }
    else{
        cout << "You tied with the computer! Almost but not quite." << "\n";
    }
    char play;
    cout << "Do you want to play again? (Y/N)" << "\n";
    cin >> play;
    if(play == 'Y' || play == 'y'){
        main();
    }
    else{
        return 0;
    }
}   // main