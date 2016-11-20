//Game of Life
const byte xdim = 8;
const byte ydim = 5;
const byte iterations = 23;
char board[ydim][xdim] = { //Led Layout and current/initial state
  {-28,-99,-99,-99,-99,-99, 99,-1},
  {-27, 20,-99,-99,-99, 99,  9,-2},
  {-26, 21,-19,-99,-99,-10, -8, 3},
  {-25,-22,-18, 15,-14,-11, -7,-4},
  { 24,-23, 17,-16,-13,-12, -6,-5},
};
const byte reset[ydim] = { //reset board to this state at number of iterations
  0b00000010,
  0b01000110,
  0b01000001,
  0b00010000,
  0b10100000
};
byte isOn(char piece){
  return piece > 0 ? 1:0;
}
byte numNeighbors(byte x, byte y){
  byte count = 0;
  byte up    = y != 0 ? y-1 : ydim-1;
  byte down  = y+1 < ydim ? y+1 : 0;
  byte left  = x != 0 ? x-1 : xdim-1;
  byte right = x+1 < xdim ? x+1 : 0;
  count += isOn(board[up][x]) + isOn(board[down][x]);
  count += isOn(board[y][left]) + isOn(board[y][right]);
  count += isOn(board[up][left]) + isOn(board[up][right]);
  count += isOn(board[down][left]) + isOn(board[down][right]);
  return count;
}
void resetBoard(){
  for(int i = 0; i < ydim; i++){
    for(int j = 0; j < xdim; j++){
      board[i][j] = ((reset[i] >> (xdim-j-1)) & 1)? abs(board[i][j]) : -abs(board[i][j]);
    }
  }
}
void iterateGame(){
  static byte iter = 0;
  byte tempBoard[ydim][xdim];
  //iterate board
  for(byte i = 0; i < ydim; i++){
    for(byte j = 0; j < xdim; j++){
      byte num = numNeighbors(j,i);
      if(num < 2 || num > 3){
        tempBoard[i][j] = -abs(board[i][j]); //kill the cell
      }
      else if(num == 3){
        tempBoard[i][j] = abs(board[i][j]);
      }
      else if(num == 2){
        tempBoard[i][j] = board[i][j];
      }
    }
  }
  //copy board
  for(byte i = 0; i < ydim; i++){
    for(byte j = 0; j < xdim; j++){
      board[i][j] = tempBoard[i][j];
    }
  }
  iter = (iter+1) % iterations;
  if(iter == 0) resetBoard();
}
boolean isAlive(byte ledNum){
  for(byte i = 0; i < ydim; i++){
    for(byte j = 0; j < xdim; j++){
      if(ledNum == abs(board[i][j])-1) return isOn(board[i][j]);
    }
  }
}
