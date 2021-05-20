//Bevan Eckman
//CS 252
//Mouse Island
//Google (for simple syntax) / Chase / Brandon

#include <iostream> //for cin, cout
#include <fstream>  //for file i/o
#include <string>   //for string data type
#include <random>   //for randomness
#include <chrono>   //for time
#include <thread>   //for pausing

using namespace std;

int getRandomNumber(int, int);
void loadMap(ifstream&, int[][90], int, int);
void printMap(int[][90], int, int);
string simulate(int[][90], int, int, int[][90]);

int main()
{
    //Create the map array
    int theMap[40][90];
    int Omap[40][90];
    int heatMap[40][90];


    //Open the file
    //cout << "Enter a file name: " << endl;
    string fileName = "map.txt";
    //cin >> fileName;

    std::ifstream inData;
    inData.open(fileName);

    //Get map attributes from the header of the file
    string mapName = "";
    int numSims = 0;
    int rows = 0;
    int cols = 0;

    if(inData.is_open()){
        getline(inData, mapName); //name of map
        inData >> numSims;        //times to run the simulation
        inData >> rows;           //number of rows in map
        inData >> cols;           //number of columns in map

        /*
         When reading with >>, it leaves \n in the buffer.
         If we want to read with getline, we have to clear that out
        */
        string clear = "";
        getline(inData,clear); //read \n into an empty string

        //read the map into the array, close the file
        loadMap(inData, Omap, rows, cols);
        inData.close();

        for(int j = 0; j< rows; j++){
            for(int k = 0; k< cols; k++){
                heatMap[j][k] = 0;
            }
        }

        string OutFileName = "results.txt";

        std::ofstream outData;
        outData.open(OutFileName);
        //string result = simulate(theMap, rows, cols);

        int drownCount = 0;
        int escapeCount = 0;
        int CatGotMouseCount = 0;
        int MouseStarve = 0;
        for(int i = 0; numSims > i;  i++){
            for(int j = 0; j< rows; j++){
                for(int k = 0; k< cols; k++){
                    theMap[j][k] = Omap[j][k];

                }
            }
            string result = simulate(theMap, rows, cols, heatMap);
            if(result == "drown"){
                drownCount++; }
            if(result == "CatGotMouse"){
                CatGotMouseCount++; }
            if(result == "Escaped"){
                escapeCount++; }
            if(result == "starve"){
                MouseStarve++;
            }
        cout << i <<  " " << result << endl;
        }
        cout << "hello" << endl;
        if(outData.is_open()){
        outData <<"The number of times the mouse drowned was "<< drownCount << endl;
        outData <<"The number of times the mouse escaped the island was " << escapeCount << endl;
        outData <<"The number of times the mouse was caught by the mouse was " << CatGotMouseCount << endl;
        outData <<"The number of times the mosuse starved was " << MouseStarve << endl;
        for(int j = 0; j< rows; j++){
            for(int k = 0; k< cols; k++){
                outData << heatMap[j][k] << " " ;

            }
            outData << "\n";
        }

        outData.close();
        }
    else{
        cout << "Invalid Input File!" << endl;
    }
    return 0;
    }
}
/*
getRandomNumber takes a minimum value and a maximum value
it returns a random number in that range.

This is a modification of the function I gave you before
from Bjarne Stroustrup. There was a compiler implementation
bug with the random device part of the previous function that
set the same seed every time, making things not very random.

This one appropriately gives randomness.
*/
int getRandomNumber(int min, int max)
{
    static std::mt19937 ran(time(nullptr));
    std::uniform_int_distribution<int> dist(min,max);
    return dist(ran);
}

/*
loadMap takes an input file object (not a string), a 2D array of integers
and the length and width of the array. The maximum number of columns is 80.

It reads the map from the file and puts it in the 2D array.
*/
void loadMap(std::ifstream& inData, int theMap[][90],int length, int width){
    string line = "";
    int row = 0;
    int col = 0;
    if(inData.is_open()){
        while(getline(inData, line)){
            for(char thing: line){
                theMap[row][col] = thing-'0'; //convert character to integer
                col++;
            }
            row++;

            if(row > length or col > width){
                cout << "ERROR: Array Overflow" << endl;
                exit(1);
            }
            col = 0;
        }
    }
    else{
        cout << "Invalid Input File!" << endl;
    }
    inData.close();
}

/*
printMap takes a 2D array of integers
and the length and width of the array. The maximum number of columns is 80.

It prints the map to the screen.
*/
void printMap(int theMap[][90], int length, int width){
    for(int i = 0; i < length; i++){
       for(int j = 0; j < width; j++){
           if(theMap[i][j] == 1){
               cout << 'w';
           }
           else if(theMap[i][j] == 0){
               cout << ' ';
           }
           else if(theMap[i][j] == 2){
               cout << 'B';
           }
           else if(theMap[i][j] == 3){
               cout << 'M';
           }
           else if(theMap[i][j] == 4){
               cout << 'C';
           }
       }
       cout << endl;
    }
    cout << endl;
}


/*
simulate takes a 2D array of integers
and the length and width of the array. The maximum number of columns is 80.

It returns a string containing the result of the simulation.

It should contain the logic for one full simulation. A simulation should
end when the mouse runs into water, escapes the island, is eaten by the cat,
or dies of starvation.


*/

string simulate(int theMap[][90], int length, int width,int heatmap[][90]){
    //Since the mouse location is in the file, we need to find it
    int mouseRow = 0;
    int mouseCol = 0;
    int catCol = 0;
    int catRow = 0;

    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(theMap[i][j] == 3){
                mouseRow = i;
                mouseCol = j;
                break;
            }
        }
    }

    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){
            if(theMap[i][j] == 4){
                catRow = i;
                catCol = j;
                break;
            }
        }
    }
    //print the initial map and delay
    //printMap(theMap, length, width);
    //std::this_thread::sleep_for(std::chrono::milliseconds(300));

    //simulation loop, currently goes forever. You should consider changing it.
    int counter = 0;
    while(counter < 100){
        //blank out the mouse's current spot
        theMap[mouseRow][mouseCol] = 0;
        theMap[catRow][catCol] = 0;
        int numH = getRandomNumber(-1,1);
        int numV = getRandomNumber(-1,1);
        int catH = getRandomNumber(-1,1);
        int catV = getRandomNumber(-1,1);

         if(numH == 1){
            mouseCol++;}
        if(numH == -1){
            mouseCol--;}
        if(numH == 0){
            ;
        }
        if(numV == 1){
            mouseRow++;}
        if(numV == -1){
            mouseRow--;}
        if(numV == 0){
            ;
        }
        int oldcatrow = catRow;
        int oldcatcol = catCol;

        if(catH == 1){
            catCol++;}
        if(catH == -1){
            catCol--;}
        if(catH == 0){
            ;
        }
        if(catV == 1){
            catRow++;}
        if(catV == -1){
            catRow--;}
        if(catV == 0){
            ;
        }


        //if the mouse hits water, return the result
        if(theMap[mouseRow][mouseCol] == 1){
            return "drown";
        }

        if(theMap[catRow][catCol]== 1){
            catRow = oldcatrow;
            catCol = oldcatcol;
        }
        if(theMap[mouseRow][mouseCol]== 2){
            return "Escaped";
        }

        if(catRow == catCol and mouseRow == mouseCol){
            return "CatGotMouse";
        }

        if(theMap[catRow][catCol] == 2){
            catRow = oldcatrow;
            catCol = oldcatcol;
        }

        //place the mouse in the new position
        theMap[mouseRow][mouseCol] = 3;

        theMap[catRow][catCol] = 4;

        heatmap[mouseRow][mouseCol] = heatmap[mouseRow][mouseCol] +1;

        if(mouseRow >= length or mouseCol >= width){
            cout << "MOUSE OVERFLOW" << endl;
        }

        if(catRow >= length or catCol >= width){
            cout << "CAT OVERFLOW";
        }

         //cout << "mouse row " << mouseRow << "mouse col " << mouseCol << endl;
         //cout << "cat row " << catRow << "cat col " << catCol << endl;
         //print the map and wait for a delay
        //printMap(theMap, length, width);
        //std::this_thread::sleep_for(std::chrono::milliseconds(17));
        counter++;
    }
    return "starve";
}
