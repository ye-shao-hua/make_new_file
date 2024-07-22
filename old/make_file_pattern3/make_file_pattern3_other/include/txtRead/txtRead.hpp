#pragma once
#include <cstddef>
#include <fstream>
#include <iterator>
#include <point_in/point.hpp>
#include <string_view>
#include <vec_in/CellVec.hpp>
#include <vec_in/CellVecAdd.hpp>
#include <vector>
/*
 * usage:
 * 1.
 *
 */
class txtRead {
public:
  void Open(std::string_view filename); // 打开文件
  cell get_master() { return master; }
  CellVec get_mental() { return mental; }
  CellVecAdd get_die() { return die; }
  void Read();
  void test();
  void Write(std::string_view filename);
  std::vector<std::size_t> Search(Point<2, double> po); // 以顶点搜索单元
  std::vector<std::size_t> Searchy(double up,
                                   double down); // 以纵坐标范围搜索单元
  std::vector<std::size_t> Searchy_mental(double up,
                                          double down); // 以纵坐标范围搜索单元
  bool Searchy_master(double up,
                      double down); // 以纵坐标范围搜索单元
  void Replace_layer(std::size_t l1, std::size_t l2); // 交换层

private:
  // 用于Read()
  void ReadCell(cell &ce);
  void ReadMaster() { ReadCell(master); }
  void ReadMental();
  void ReadDie();
  void ReadCorner();
  // 用于Weite()
  void WriteCell(cell ce);
  void WriteCell(cellAdd ce);
  void WriteMaster();
  void WriteMental();
  void WriteDie();
  void WriteCorner();

private:
  std::ifstream ifs;
  cell master{};
  CellVec mental{};
  CellVecAdd die{};
  cell corner{};
  std::ofstream ofs;
};

// 类成员函数的实现
//
// 打开文件
void txtRead::Open(std::string_view filename) {
  ifs.open(filename.data());
  if (!ifs.is_open()) {
    throw std::runtime_error{"file not open"};
  }
}
// Read()系列
void txtRead::ReadCell(cell &ce) {
  std::string buffer;
  std::size_t n_point = 0;
  Point<2, double> point;
  ifs >> buffer;
  ce.add_name(buffer);
  ifs >> n_point;
  ce.add_number(n_point);
  for (auto i : std::ranges::iota_view{0uz, n_point}) {
    ifs >> point;
    ce += point;
  }
}
void txtRead::ReadMental() {
  std::size_t n_cell = 0;
  ifs >> n_cell;
  mental.set_number(n_cell);
  for (auto i : std::ranges::iota_view{0uz, n_cell}) {
    cell ce;
    ReadCell(ce);
    mental += ce;
  }
}
void txtRead::ReadDie() {
  std::size_t n_cell = 0;
  double value = 0;
  ifs >> n_cell;
  die.set_number(n_cell);
  for (auto i : std::ranges::iota_view{0uz, n_cell}) {
    cellAdd ce;
    ReadCell(ce);
    ifs >> value;
    ce.add_value(value);
    die += ce;
  }
}
void txtRead::ReadCorner() {
  double n = 0;
  Point<2, double> point;
  ifs >> n;
  corner.add_number(n);
  for (auto i : std::ranges::iota_view{0uz, n}) {
    ifs >> point;
    corner += point;
  }
}
void txtRead::Read() {
  ReadMaster();
  ReadMental();
  ReadDie();
  ReadCorner();
}

//
void txtRead::test() {
  ReadMaster();
  master.show();
  std::cout << std::endl;
  ReadMental();
  mental.show();
  ReadDie();
  die.show();
  ReadCorner();
  corner.show();
}
// Write（）系列
void txtRead::Write(std::string_view filename) {
  ofs.open(filename.data());
  WriteMaster();
  WriteMental();
  WriteDie();
  WriteCorner();
}
void txtRead::WriteCell(cell ce) {
  ofs << ce.get_name() << "\n";
  ofs << ce.get_number() << "\n";
  for (auto i : ce) {
    for (auto j : i) {
      ofs << j << " ";
    }
    ofs << "\n";
  }
  ofs << "\n";
}
void txtRead::WriteCell(cellAdd ce) {
  ofs << ce.get_name() << "\n";
  ofs << ce.get_number() << "\n";
  for (auto i : ce) {
    for (auto j : i) {
      ofs << j << " ";
    }
    ofs << "\n";
  }
  ofs << ce.get_value() << "\n\n";
}
void txtRead::WriteMaster() { WriteCell(master); }
void txtRead::WriteMental() {
  ofs << mental.get_number() << "\n";
  for (auto i : mental) {
    WriteCell(i);
  }
  ofs << "\n";
}
void txtRead::WriteDie() {
  ofs << die.get_number() << "\n";
  for (auto i : die) {
    WriteCell(i);
  }
}
void txtRead::WriteCorner() { WriteCell(corner); }

// Search()系列
std::vector<std::size_t> txtRead::Search(Point<2, double> po) {
  std::vector<std::size_t> number{};
  for (auto k = 0; auto i : die) {
    for (auto j : i) {
      if (po == j) {
        number.push_back(k);
      }
    }
    k++;
  }
  return number;
}
std::vector<std::size_t> txtRead::Searchy(double up, double down) {
  std::vector<std::size_t> number{};
  for (auto k = 0; auto i : die) {
    int number_of_point = 0;
    for (auto j : i) {
      // if (*(j.begin() + 1) <= up && *(j.begin() + 1) >= down) {
      if (j.data()[1] <= up + 0.001 && j.data()[1] >= down - 0.001) {
        number_of_point++;
      }
      // point j
      if (number_of_point == i.size()) {
        number.push_back(k);
      }
    }
    k++;
  }
  return number;
}

// Replace系列
void txtRead::Replace_layer(std::size_t l1, std::size_t l2) {
  const double high_base = 0.4875;
  const double thick_layer = 0.187;
  bool a;                         // 记录a是否在第二层
  std::vector<std::size_t> b, c;  // 记录第二，三层mental序号
  std::vector<std::size_t> d, e;  // 记录第二，三层die序号
  std::vector<std::size_t> f, g;  // 第三，六层顶上die序号
  std::vector<std::size_t> h, hh; // 第二，三层对应交换层die序号
  a = Searchy_master(0.6745, 0.4875);
  b = Searchy_mental(0.6535, 0.4875);
  c = Searchy_mental(0.6535 + thick_layer, 0.4875 + thick_layer);
  d = Searchy(high_base + thick_layer, high_base);
  e = Searchy(high_base + 2 * thick_layer, high_base + thick_layer);
  f = Searchy(0.8651, 0.8405);
  g = Searchy(1.4765, 1.4015);
  hh = Searchy(high_base + (l2 - 1) * thick_layer,
               high_base + (l2 - 2) * thick_layer);
  // 第三层操作
  if (l2 != 6) {
    for (auto i : c) {
      for (auto &j : mental[i]) {
        j[1] += (l2 - 3) * thick_layer;
      }
    }
    for (auto i : e) {
      for (auto &j : die[i]) {
        j[1] += (l2 - 3) * thick_layer;
      }
    }
    for (auto i : hh) {
      for (auto &j : die[i]) {
        j[1] -= (l2 - 3) * thick_layer;
      }
    }
  } else {
    hh = Searchy(high_base + (l2 - 1) * thick_layer + 0.054,
                 high_base + (l2 - 2) * thick_layer);
    for (auto i : c) {
      for (auto &j : mental[i]) {
        j[1] += (l2 - 3) * thick_layer;
      }
    }
    for (auto i : e) {
      for (auto &j : die[i]) {
        j[1] += (l2 - 3) * thick_layer;
      }
    }
    for (auto i : hh) {
      for (auto &j : die[i]) {
        j[1] -= (l2 - 3) * thick_layer;
      }
    }
    for (auto i : f) {
      std::size_t si;
      si = die[i].get_number();
      die[i][si - 1] += Point<2>{0, 0.054};
      die[i][si - 2] += Point<2>{0, 0.054};
    }
    for (auto i : g) {
      die[i][2] += Point<2>{0, -0.054};
      die[i][3] += Point<2>{0, -0.054};
    }
  }
  // 第二层操作
  h = Searchy(high_base + (l1 - 1) * thick_layer,
              high_base + (l1 - 2) * thick_layer);
  if (a) {
    for (auto &i : master) {
      i[1] += (l1 - 2) * thick_layer;
    }
  } else {
    for (auto &i : master) {
      i[1] += (l2 - 3) * thick_layer;
    }
  }

  if (l1 != 3) {
    for (auto i : b) {
      for (auto &j : mental[i]) {
        j[1] += (l1 - 2) * thick_layer;
      }
    }
    for (auto i : d) {
      for (auto &j : die[i]) {
        j[1] += (l1 - 2) * thick_layer;
      }
    }
    for (auto i : h) {
      for (auto &j : die[i]) {
        j[1] -= (l1 - 2) * thick_layer;
      }
    }
  } else {
    for (auto i : b) {
      for (auto &j : mental[i]) {
        j[1] += (l1 - 2) * thick_layer;
      }
    }
    for (auto i : d) {
      for (auto &j : die[i]) {
        j[1] += (l1 - 2) * thick_layer;
      }
    }
    for (auto i : hh) {
      for (auto &j : die[i]) {
        j[1] -= (l1 - 2) * thick_layer;
      }
    }
  }
}

std::vector<std::size_t> txtRead::Searchy_mental(double up, double down) {
  std::vector<std::size_t> number{};
  for (auto k = 0; auto i : mental) {
    int number_of_point = 0;
    for (auto j : i) {
      // if (*(j.begin() + 1) <= up && *(j.begin() + 1) >= down) {
      if (j.data()[1] <= up + 0.001 && j.data()[1] >= down - 0.001) {
        number_of_point++;
      }
      // point j
      if (number_of_point == i.size()) {
        number.push_back(k);
      }
    }
    k++;
  }
  return number;
}

bool txtRead::Searchy_master(double up, double down) {
  int number_of_point = 0;
  for (auto j : master) {
    // if (*(j.begin() + 1) <= up && *(j.begin() + 1) >= down) {
    if (j.data()[1] <= up + 0.001 && j.data()[1] >= down - 0.001) {
      number_of_point++;
    }
    // point j
    if (number_of_point == master.size()) {
      return true;
    }
  }
  return false;
}