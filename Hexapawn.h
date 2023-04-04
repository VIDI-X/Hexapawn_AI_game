#ifndef HEXAPAWN_H
#define HEXAPAWN_H

#include <Arduino.h>
#include <vector>
#include <unordered_map>

class Hexapawn {
public:
  Hexapawn();

  void train();
  int playGame();
  
  int gamesPlayed;

private:
  typedef std::vector<std::vector<int>> Board;
  typedef std::pair<Board, int> StateActionPair;

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1, T2>& p) const {
      auto h1 = board_hash(p.first);
      auto h2 = std::hash<T2>{}(p.second);
      return h1 ^ h2;
    }

    static std::size_t board_hash(const Board& board) {
      std::size_t seed = board.size();
      for (const auto& row : board) {
        for (const auto& elem : row) {
          seed ^= std::hash<int>{}(elem) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
      }
      return seed;
    }
  };

  Board initialBoard;
  float learningRate;
  float discountFactor;
  int trainEpisodes;

  std::unordered_map<StateActionPair, float, pair_hash> Q;

  std::vector<Board> getPossibleBoards(Board board, int player);
  Board getNewBoardAfterMove(Board board, int player, int move);
  bool isEndState(Board board, int player);
  int getWinner(Board board);
  bool boardsEqual(Board board1, Board board2);
  void printBoard(Board board);
  int boardHash(Board board);
  Board rotateBoard(Board board);
  Board mirrorBoard(Board board);
};

#endif // HEXAPAWN_H
