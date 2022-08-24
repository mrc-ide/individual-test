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
std::vector<size_t> run_model(size_t N, size_t I0, double dt, size_t end_time, double gamma, double R0) {
  std::vector<std::string> health_states = {"S", "I", "R"};
  std::vector<std::string> health_states_t0 = std::vector<std::string>(N);
  for (size_t i = 0; i < N; i++) {
    health_states_t0[i] = "S";
  }

  std::vector<int> ivec(N);
  std::vector<int> out;
  std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..99]
  std::sample(ivec.begin(), ivec.end(),
           std::back_inserter(out),
           I0, std::mt19937{std::random_device{}()});
  for (size_t i = 0; i < I0; i++) {
    health_states_t0[out[i]] = "I";
  }
  auto model = new Model(health_states, health_states_t0, N, dt, end_time, gamma, R0);
  return model->run_simulation();
}

//[[Rcpp::export]]
double model_prob(size_t N, size_t I0, double dt, size_t end_time, double gamma, double R0) {
  std::vector<std::string> health_states = {"S", "I", "R"};
  std::vector<std::string> health_states_t0 = std::vector<std::string>(N);
  for (size_t i = 0; i < N; i++) {
    health_states_t0[i] = "S";
  }

  std::vector<int> ivec(N);
  std::vector<int> out;
  std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..99]
  std::sample(ivec.begin(), ivec.end(),
           std::back_inserter(out),
           I0, std::mt19937{std::random_device{}()});
  for (size_t i = 0; i < I0; i++) {
    health_states_t0[out[i]] = "I";
  }
  auto model = new Model(health_states, health_states_t0, N, dt, end_time, gamma, R0);
  return model->sample_prob(I0);
}
