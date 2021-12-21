#include <iostream>
#include <vector>
#include <array>
#include <memory>

using namespace std;

class Board {
 public:
  static const int rows = 6;
  static const int columns = 7;
  static const int toWin = 4;
  enum squareType {Empty, Red, Black};
  enum gameState {RedTurn, BlackTurn, RedWins, BlackWins, Tie};
 private:
  gameState currentGameState = RedTurn;
  bool blackWentFirst = true;
  array< array< squareType, columns >, rows > board;
 public:
  Board() {
    newGame();
  }
  squareType getSquare(int row, int column) const {
    return board.at(row).at(column);
  }
  gameState getGameState() const {
    return currentGameState;
  }
  bool move (int column) { // Returns True if it was a valid move
    if (currentGameState==RedWins
        || currentGameState == BlackWins
        || currentGameState == Tie)
      return false; // Can not move
    // Finds where the piece should go
    int row = 0;
    while (row<rows && getSquare(row, column) == Empty)
      row+=1;
    if (row==0)
      return false; // Row full
    board.at(row-1).at(column) =
      currentGameState == RedTurn ? Red : Black; //make move

    //This code checks to see if there are four in a row
    //Starting in every square and going in four different directions
    //(Horizonal, vertical, two diagonals)
    int x;
    for (int row=0; row<rows; row++) {
      for (int column=0; column<columns; column++) {
        if (getSquare(row, column)!=Empty) {
          squareType color = getSquare(row, column);

          for (x=1; x<toWin; x++)
            if (row+x>=rows || getSquare(row+x, column)!=color) break;
          if (x<toWin) {
            for (x=1; x<toWin; x++)
              if (column+x>=columns || getSquare(row, column+x)!=color) break;
          }
          if (x<toWin) {
            for (x=1; x<toWin; x++)
              if (row+x>=rows || column+x>=columns || getSquare(row+x, column+x)!=color) break;
          }
          if (x<toWin) {
            for (x=1; x<toWin; x++)
              if (row+x>=rows || column-x<0 || getSquare(row+x, column-x)!=color) break;
          }
          if (x==toWin) {
            currentGameState=(color==Red) ? RedWins: BlackWins;
            return true;
          }
        }
      }
    }
    // This checks for a tie (all top squares are occupoed)
    for (x=0; x<columns; x++)
      if (getSquare(0, x)==Empty) break;
    if (x==columns) {
      currentGameState = Tie;
      return true;
    }
    // Change whose turn it is
    currentGameState = currentGameState==RedTurn ? BlackTurn: RedTurn;
    return true;
  }
  void newGame() {
    for (auto &c: board) for (auto &x: c) x = Empty;
    blackWentFirst=!blackWentFirst;
    currentGameState = blackWentFirst ? BlackTurn: RedTurn;
  }
};


/*--------------------------------------------------
DispalyBoard class.
--------------------------------------------------*/


class DisplayBoard {
  shared_ptr<Board> board;
 public:
  DisplayBoard(shared_ptr<Board> board): board{board} {};
  void display() {
    cout<<" 1 2 3 4 5 6 7"<<endl;
    for (int x=0; x<Board::rows; ++x) {
      for (int y=0; y<Board::columns; ++y) {
        switch (board->getSquare(x, y)) {
          case Board::Red:
            cout<<"|R";
            break;
          case Board::Black:
            cout<<"|B";
            break;
          case Board::Empty:
            cout<<"| ";
            break;
        }
      }
      cout<<"|"<<endl<<"+-+-+-+-+-+-+-+"<<endl;
    }
    cout<<endl;
    switch (board->getGameState()) {
      case Board::RedTurn:
        cout<<"Red's move"<<endl;
        break;
      case Board::BlackTurn:
        cout<<"Black's move"<<endl;
        break;
      case Board::RedWins:
        cout<<"Red wins!"<<endl;
        break;
      case Board::BlackWins:
        cout<<"Black wins!"<<endl;
        break;
      case Board::Tie:
        cout<<"Tie!"<<endl;
        break;
    }
    cout<<"1-7: play in that column; n: new game; q: quit"<<endl;
  }
};

/*--------------------------------------------------
ControllBoard class.
--------------------------------------------------*/

class ControllBoard {
  shared_ptr<Board> board;
 public:
  ControllBoard(shared_ptr<Board> board): board{board} {};
  void processKey(char key) {
    switch (key) {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
        board->move(key-'1');
        break;
      case 'n':
        board->newGame();
        break;
      case 'q':
        exit(0);
    }
  }
};

int main() {
  auto b = make_shared<Board>();
  DisplayBoard db{b};
  ControllBoard controller{b};
  while (true) {
    system("clear");
    db.display();
    char c;
    cin.get(c);
    controller.processKey(c);
  }
}
