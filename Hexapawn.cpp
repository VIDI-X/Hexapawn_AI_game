#include "Hexapawn.h"

Hexapawn::Hexapawn() {
  initialBoard = {
    { 1,  1,  1},
    { 0,  0,  0},
    {-1, -1, -1}
  };

  learningRate = 0.5;
  discountFactor = 0.9;
  trainEpisodes = 10000;
}

void Hexapawn::train() {
  for (int episode = 0; episode < trainEpisodes; ++episode) {
    Board currentBoard = initialBoard;
    int currentPlayer = 1;

    while (!isEndState(currentBoard, currentPlayer)) {
      
      std::vector<Board> possibleBoards = getPossibleBoards(currentBoard, currentPlayer);

      float maxQ = -1e6;
      int bestMoveIdx = 0;

      for (int i = 0; i < possibleBoards.size(); ++i) {
        Board possibleBoard = possibleBoards[i];
        float q = Q[StateActionPair(currentBoard, i)];
        if (q > maxQ) {
          maxQ = q;
          bestMoveIdx = i;
        }
      }

      Board nextBoard = possibleBoards[bestMoveIdx];
      float reward = 0;

      if (isEndState(nextBoard, -currentPlayer)) {
        reward = 1;
      }

      float maxQNext = -1e6;
      std::vector<Board> nextPossibleBoards = getPossibleBoards(nextBoard, -currentPlayer);

      for (Board& possibleBoard : nextPossibleBoards) {
        float q = Q[StateActionPair(nextBoard, boardHash(possibleBoard))];
        if (q > maxQNext) {
          maxQNext = q;
        }
      }

      Q[StateActionPair(currentBoard, bestMoveIdx)] += learningRate * (reward + discountFactor * maxQNext - Q[StateActionPair(currentBoard, bestMoveIdx)]);

      currentBoard = nextBoard;
      currentPlayer = -currentPlayer;
    }
  }
}

int Hexapawn::playGame() {
  Board currentBoard = initialBoard;
  int currentPlayer = 1;

  while (!isEndState(currentBoard, currentPlayer)) {
    printBoard(currentBoard);

    std::vector<Board> possibleBoards = getPossibleBoards(currentBoard, currentPlayer);

    float maxQ = -1e6;
    int bestMoveIdx = 0;

    for (int i = 0; i < possibleBoards.size(); ++i) {
      Board possibleBoard = possibleBoards[i];
      float q = Q[StateActionPair(currentBoard, i)];
      if (q > maxQ) {
        maxQ = q;
        bestMoveIdx = i;
      }
    }

    Board nextBoard = possibleBoards[bestMoveIdx];

    currentBoard = nextBoard;
    currentPlayer = -currentPlayer;
  }

  return getWinner(currentBoard);
}

// Implementacija ostalih privatnih metoda dolazi ovdje.
// Dodajte ove metode u Hexapawn.cpp:

bool Hexapawn::isEndState(Board board, int player) {
  if (getPossibleBoards(board, player).empty()) {
    return true;
  }

  for (int i = 0; i < 3; ++i) {
    if (board[0][i] == -1 || board[2][i] == 1) {
      return true;
    }
  }

  return false;
}

int Hexapawn::getWinner(Board board) {
  for (int i = 0; i < 3; ++i) {
    if (board[0][i] == -1) {
      return -1;
    }
    if (board[2][i] == 1) {
      return 1;
    }
  }

  return 0;
}

void Hexapawn::printBoard(Board board) {
  for (const auto &row : board) {
    for (const auto &elem : row) {
      Serial.print(elem);
      Serial.print(" ");
    }
    Serial.println();
  }
  Serial.println("-----");
}

std::vector<Hexapawn::Board> Hexapawn::getPossibleBoards(Board board, int player) {
  // Ovdje dodajte logiku za generiranje mogućih poteza za trenutačnog igrača.
  // Povratite vektor svih mogućih ploča koje se mogu postići iz trenutne ploče.
  std::vector<Board> possibleBoards;
  // Implementirajte logiku za generiranje poteza.
    for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      if (board[row][col] == player) {
        // Potez naprijed
        int newRow = row + player;
        if (newRow >= 0 && newRow < 3 && board[newRow][col] == 0) {
          Board newBoard = board;
          newBoard[row][col] = 0;
          newBoard[newRow][col] = player;
          possibleBoards.push_back(newBoard);
        }

        // Potez dijagonalno lijevo
        int newCol = col - 1;
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3 &&
            board[newRow][newCol] == -player) {
          Board newBoard = board;
          newBoard[row][col] = 0;
          newBoard[newRow][newCol] = player;
          possibleBoards.push_back(newBoard);
        }

        // Potez dijagonalno desno
        newCol = col + 1;
        if (newRow >= 0 && newRow < 3 && newCol >= 0 && newCol < 3 &&
            board[newRow][newCol] == -player) {
          Board newBoard = board;
          newBoard[row][col] = 0;
          newBoard[newRow][newCol] = player;
          possibleBoards.push_back(newBoard);
        }
      }
    }
  }
  
  return possibleBoards;
}

int Hexapawn::boardHash(Board board) {
  // Ova funkcija bi trebala generirati jedinstveni identifikator za svaku ploču.
  // Možete koristiti kombinaciju vrijednosti elemenata na ploči, rotacija ili zrcaljenja ploče.
  // Implementirajte logiku za generiranje jedinstvenog identifikatora za ploču.
  int hash = 0;
  int base = 3;
  int exponent = 0;

  for (const auto &row : board) {
    for (const auto &elem : row) {
      // Ovdje dodajemo 1 kako bismo dobili pozitivan broj (jer su elementi -1, 0, 1).
      // Ovo je potrebno jer koristimo base kao osnovu za izračunavanje hash vrijednosti.
      hash += (elem + 1) * pow(base, exponent);
      exponent++;
    }
  }

  return hash;
  //return 0;
}

Hexapawn::Board Hexapawn::rotateBoard(Board board) {
  // Ovdje dodajte logiku za rotiranje ploče za 90 stupnjeva u smjeru kazaljke na satu.
  Board rotatedBoard(3, std::vector<int>(3, 0));
  // Implementirajte logiku za rotiranje ploče.
    for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      rotatedBoard[col][2 - row] = board[row][col];
    }
  }
  return rotatedBoard;
}

Hexapawn::Board Hexapawn::mirrorBoard(Board board) {
  // Ovdje dodajte logiku za zrcaljenje ploče oko vertikalne osi.
  Board mirroredBoard(3, std::vector<int>(3, 0));
  // Implementirajte logiku za zrcaljenje ploče.
    for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      mirroredBoard[row][2 - col] = board[row][col];
    }
  }
  return mirroredBoard;
}

