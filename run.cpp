
#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include "kmeans.h"
using namespace std;

std::vector<double> uniform_sample(size_t n, double low=-1.0, double high=1.0){ 
  // std::random_device rd;  //Will be used to obtain a seed for the random number engine
  std::mt19937 generator(2333); //Standard mersenne_twister_engine seeded with rd()
  std::uniform_real_distribution<> dis(low, high);
  std::vector<double> new_points(n, 0.0);
  //std::cout << std::setprecision(10);
  for(size_t i=0;i < n; ++i){
    new_points[i] = dis(generator);
  }
  std::sort(new_points.begin(), new_points.end());
  return new_points;
}
template <typename T>
void print_vector(const std::vector<T> & vec, std::string sep=","){
    for(auto elem : vec)
    {
        std::cout << elem << sep;
    }
    std::cout<<std::endl;
}

int main(int argc, char *argv[])
{
  float max_error;
  std::vector<double> points;
  if (argc == 1 or argv[1]==std::string("u") ){
    points = uniform_sample(20, 1, 10);
    max_error = 3;
  }
  if (argc == 2 and argv[1]==std::string("m")){
    points = {
      // 1,2,3,4,5, 11,12,13,14,15, 21,22,23,24,25, 31,32,33,34,35
      // 1,2,3,6,7,8,11,12,13
       1,2,3,6,7,8
    };
    max_error = 0.005;
  }
  if (argc == 2 and argv[1]==std::string("r")){
    points = {
      66,68,66,55,64,56,52,74,81,88,81,96,85,54,56,61,70,64,77,66
    };
    std::sort(points.begin(), points.end());
    max_error = 3;

  }
  float min_cost = 1e7;
  std::cout<<"[data points]"<<std::endl;
  print_vector(points);
	if(!std::is_sorted(std::begin(points), std::end(points))){
	  std::cout << "input not sorted " << std::endl;
	  return -1;
	}
    std::string prefix_template = "[data points]";
    unique_ptr<kmeans_wilber> f(new kmeans_wilber(points));
    f->set_search_strategy(search_strategy::INTERPOLATION);
    for (size_t k=1; k<points.size(); ++k){
    std::cout << "[k = "<< k <<"] start..."<< std::endl;
    unique_ptr<kmeans_result> res = f->compute(k);
    std::cout << "[" << f->name() << "] "
              << "[k = " << k << "] [n = " << points.size() << "] "
              << "[cost = " << res->cost << "]" << std::endl;
    std::cout<<"[centers]"<<std::endl;
    print_vector(res->centers);
    std::cout<<"[path]"<<std::endl;
    print_vector(res->path);
    if (res->cost < max_error){
      break;
    }
    if (res->cost < min_cost){
      min_cost = res->cost;
    }
  }
  return 0;
}
