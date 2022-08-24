#include "model.hpp"
#include <iostream>
#include <random>
#include <string>
#include <iterator>
#include <algorithm>
#include  <individual.h>
#include  <IterableBitset.h>
#include  <CategoricalVariable.h>

//[[Rcpp::export]]
void run_model() {
  size_t N = 1e3;
  size_t I0 = 5;
  size_t S0 = N - I0;
  std::vector<std::string> health_states = {"S", "I", "R"};
  std::vector<std::string> health_states_t0 = std::vector<std::string>(N);
  for (size_t i; i < N; i++) {
    health_states_t0[i] = "N";
  }
  std::vector<int> ivec(100);
  std::vector<int> out;
  std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..99]
  std::sample(ivec.begin(), ivec.end(),
           std::back_inserter(out),
           I0, std::mt19937{std::random_device{}()});

  for (size_t i; i < I0; i++) {
    health_states_t0[out[i]] = "I";
  }
  auto model = new Model(health_states, health_states_t0, N);
  model->run_simulation();
}
