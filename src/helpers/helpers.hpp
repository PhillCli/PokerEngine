#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#ifndef HELPERS_H
#define HELPERS_H

class CSVRow {
  private:
    std::vector<std::string> m_data;
	public:
		//CSVRow();
		//~CSVRow();
    std::string const& operator [](std::size_t index) const;
    std::size_t size() const;
    std::vector<std::string> getRow();
    void readNextRow(std::istream& str);

};

class EvTestCases {
  private:
    std::vector<std::vector<std::string> > hands;
    std::vector<std::vector<std::string> > board;
    std::vector<std::vector<std::string> > dead;
    std::vector<std::vector<double> >      ev;
  public:
    std::size_t size() const;
    std::vector<std::string> getHands(size_t index);
    std::vector<std::string> getBoard(size_t index);
    std::vector<std::string> getDead(size_t index);
    std::vector<double> getEv(size_t index);
    void parseNewRow(std::vector<std::string> &row);

};

#endif

