#pragma once
#include <cstddef>
#include <fstream>
#include <iterator>
#include <point_in/point.hpp>
#include <string_view>
#include <vec_in/CellVec.hpp>
#include <vec_in/CellVecAdd.hpp>
#include <vector>
#include <iostream>
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
  std::vector<std::size_t> Search(const Point<2, double> &po); // 以顶点搜索单元
  std::vector<std::size_t> Searchy(double up,
                                   double down); // 以纵坐标范围搜索单元
  std::vector<std::size_t> Searchx(double left,
                                   double right); // 以纵坐标范围搜索单元
  void Replace_layer(std::size_t la);             // 交换层
  void Replace_layer1(int layer1,int layer2);             // 交换层
  std::size_t Same_number(std::vector<std::size_t> a,
                          std::vector<std::size_t> b);

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
std::vector<std::size_t> txtRead::Search(const Point<2, double> &po) {
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
std::vector<std::size_t> txtRead::Searchx(double left, double right) {
  std::vector<std::size_t> number{};
  for (auto k = 0; auto i : die) {
    int number_of_point = 0;
    for (auto j : i) {
      if (j.data()[0] <= right + 0.001 && j.data()[0] >= left - 0.001) {
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
void txtRead::Replace_layer(std::size_t layer) {
    std::vector<double> hight;
    std::vector<double> width;
    hight.push_back(0.);
    hight.push_back(0.687565);
    hight.push_back(0.932565);
    hight.push_back(1.17756);
    hight.push_back(1.42257);
    hight.push_back(1.66756);
    hight.push_back(1.9125);
    
    width.push_back(0.);
    width.push_back(0.032005);
    width.push_back(0.031985);
    width.push_back(0.032);
    width.push_back(0.032);
    width.push_back(0.032);
    width.push_back(0.07);

    std::vector<std::size_t> a;
    std::vector<std::size_t> b;

    if(layer>6||layer<2){
        goto Error;
    }else{
        goto Other;
    }

Other:
    //先扩充范围
    //上移
    for(auto i=0;i<layer-2;++i){
        a=Searchy(7,hight[2]);
        for(auto j:a){
            for(auto &k:*(die.begin()+j)){
                k.data()[1]=k.data()[1]+(hight[3]-hight[2]);
            }
        }
        
        
        for(auto &j:mental){
            if(j.get_name()=="topleft"||j.get_name()=="topright"){
                for(auto &k:j){
                    k.data()[1]=k.data()[1]+(hight[3]-hight[2]);
                }
            }
        }
        //复制
        a=Searchy(hight[3]+(hight[3]-hight[2]),hight[3]);
        for(auto i:a){
            cellAdd ce;
            int number;
            ce=*(die.begin()+i);
            die+=ce;
            number=die.get_number();
            die.set_number(number+1);
            for(auto &j:*(die.begin()+i)){
                j.data()[1]=j.data()[1]-(hight[3]-hight[2]);
            }
        }
        //修改cornel
        for(auto &i:corner){
            if(i.data()[1]>1){
                i.data()[1]=i.data()[1]+(hight[3]-hight[2]);
            }
        }
    }
    //交换
    //主导体
    for(auto &i:master){
        i.data()[1]=i.data()[1]+(layer-2)*(hight[3]-hight[2]);
    }
    //其余导体
    for(auto &i:mental){
        if(i.get_name()=="c3"||i.get_name()=="c2"||i.get_name()=="c2e"){
            for(auto &j:i){
                j.data()[1]=j.data()[1]+(layer-2)*(hight[3]-hight[2]);
            }
        }
    }
    //介质
    a=Searchy(hight[2]+(layer-2)*(hight[3]-hight[2]),hight[2]+(layer-3)*(hight[3]-hight[2]));
    b=Searchy(hight[2],hight[1]);
    for(auto i:a){
        for(auto &j:*(die.begin()+i)){
            j.data()[1]=j.data()[1]-(layer-2)*(hight[3]-hight[2]);
        }
    }
    for(auto i:b){
        for(auto &j:*(die.begin()+i)){
            j.data()[1]=j.data()[1]+(layer-2)*(hight[3]-hight[2]);
        }
    }
    
    goto End;
Error:
    std::cout<<"file generate fail\n";
    goto End;
End:
    return ;
}

