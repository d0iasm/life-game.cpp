#include <cstdlib>
#include <thread>
#include <string>
#include <iostream>
#include <vector>


int main(){
  using table_t = std::vector<std::vector<bool>>;
  table_t table = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  };

  auto width = [](auto const& table){
    return table[0].size();
  };

  auto height = [](auto const& table){
    return table.size();
  };

  auto at_cell = [&](auto const& table, auto x, auto y){
    return x < 0 || width(table)  <= x ? false
      : y < 0 || height(table) <= y ? false
      : table[y][x];
  };

  auto dead_or_alive = [&](auto table, auto x, auto y){
    auto cell  = at_cell(table, x, y);
    auto count = at_cell(table, x - 1, y - 1)
      + at_cell(table, x - 0, y - 1)
      + at_cell(table, x + 1, y - 1)
      + at_cell(table, x + 1, y - 0)
      + at_cell(table, x + 1, y + 1)
      + at_cell(table, x - 0, y + 1)
      + at_cell(table, x - 1, y + 1)
      + at_cell(table, x - 1, y - 0);
    return cell ? count == 3 || count == 2 : count == 3;
  };

  auto update = [&](auto const& table){
    table_t result = table;
    for(int y = 0 ; y < table.size() ; ++y){
      for(int x = 0 ; x < table[y].size() ; ++x){
        result[y][x] = dead_or_alive(table, x, y);
      }
    }
    return result;
  };

  auto print = [&](auto const& table){
    std::system("clear");
    // or
    //      std::system("cls");

    std::cout << std::string(width(table) + 2, '-') << "\n";
    for(auto&& row : table){
      std::cout << "|";
      for(auto&& cell : row){
        std::cout << (cell ? "*" : " ");
      }
      std::cout << "|\n";
    }
    std::cout << std::string(width(table) + 2, '-') << "\n";
    std::cout << std::endl;
  };

  while(1){
    print(table);
    table = update(table);
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  return 0;
}
