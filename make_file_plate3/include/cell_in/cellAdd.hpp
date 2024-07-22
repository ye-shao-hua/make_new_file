#pragma once
#include <cell_in/cell.hpp>

class cellAdd : public cell {
  // cellAdd class,have an additional property 'm_value' to record epsilon(e)
public:
  void show() {
    std::cout << m_name << std::endl;
    std::cout << m_PointNumber << std::endl;
    for (auto i : m_data) {
      i.show();
      std::cout << std::endl;
    }
    std::cout << m_value << std::endl;
  }
  void add_value(double va) { m_value = va; }
  double get_value() { return m_value; }
  void operator=(cellAdd &ce){
      m_name=ce.get_name();
      m_PointNumber=ce.get_number();
      for(auto i:ce){
          m_data.push_back(i);
      }
      m_value=ce.get_value();
  }
private:
  double m_value{};
};
