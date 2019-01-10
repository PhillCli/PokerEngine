#include "helpers/helpers.hpp"
//#include <sstream>

// helper class for reading CSV
std::string const& CSVRow::operator [](std::size_t index) const {
  return m_data[index];
}

std::size_t CSVRow::size() const {
  return m_data.size();
}

std::vector<std::string> CSVRow::getRow() {
  return m_data;
}

void CSVRow::readNextRow(std::istream& str) {
  std::string        line;
  std::getline(str, line);

  std::stringstream lineStream(line);
  std::string       cell;

  m_data.clear();
  while(std::getline(lineStream, cell, ',')) {
    m_data.push_back(cell);
  }
  // trailing comas
  if (!lineStream && cell.empty()) {
    m_data.push_back("");
  }
}

// helper class for test cases
std::size_t EvTestCases::size() const {
  return ev.size();
}

std::vector<std::string> EvTestCases::getHands(size_t index) {
  return hands[index];
}

std::vector<std::string> EvTestCases::getBoard(size_t index) {
  return board[index];
}

std::vector<std::string> EvTestCases::getDead(size_t index) {
  return dead[index];
}

std::vector<double> EvTestCases::getEv(size_t index) {
  return ev[index];
}

void EvTestCases::parseNewRow(std::vector<std::string> &row) {
  // [P1_HAND, P1_EV, ..., PN_HAND, PN_EV, ..., BOARD, DEAD]

  // FIXME: add support for BOARD & DEAD cards in the future.
  // Only Hands & EV for now
  //std::cout << "[DEBUG] ROW SIZE: " << row_size << std::endl;
  std::vector<std::string> f_hands;
  std::vector<std::string> f_board;
  std::vector<std::string> f_dead;
  std::vector<double>      f_ev;
  for (size_t idx=0; idx < row.size(); idx+=2) {
    //std::cout << "[DEBUG] IDX: " << idx << " IDX2: " << idx+1 << std::endl;
    f_hands.push_back(row[idx]);
    //std::cout << row[idx] << std::endl;
    //std::cout << row[idx+1] << std::endl;
    double d_ev = std::stod(row[idx+1]);
    f_ev.push_back(d_ev);
  }
  f_board.push_back("");
  f_dead.push_back("");

  board.push_back(f_board);
  dead.push_back(f_dead);
  hands.push_back(f_hands);
  ev.push_back(f_ev);
}

//std::istream& operator >> (std::istream& is, CSVRow& data) {
//  data.readNextRow(is);
//  return is;
//}

