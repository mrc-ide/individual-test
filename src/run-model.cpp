#include "model.hpp"
#include "model_closed.hpp"
#include <iostream>
#include <random>
#include <string>
#include <iterator>
#include <algorithm>
#include  <individual.h>
#include  <IterableBitset.h>
#include  <CategoricalVariable.h>

//[[Rcpp::export]]
std::vector<size_t> run_model_open(size_t N, size_t I0, double dt, size_t end_time,
 double gamma, double R0, double birth_rate, double death_rate) {
  std::vector<std::string> health_states = {"S", "I", "R"};
  std::vector<std::string> health_states_t0 = std::vector<std::string>(N);
  for (size_t i = 0; i < N; i++) {
    health_states_t0[i] = "S";
  }

  std::vector<size_t> ivec(N);
  std::vector<size_t> out;
  std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..n]
  std::sample(ivec.begin(), ivec.end(),
           std::back_inserter(out),
           I0, std::mt19937{std::random_device{}()});

  for (size_t i = 0; i < I0; i++) {
    health_states_t0[out[i]] = "I";
  }
  auto model = new Model(health_states, health_states_t0, N, dt, end_time,
   gamma, R0, birth_rate, death_rate);
  model->run_simulation();
  return model->render();
}

//[[Rcpp::export]]
std::vector<size_t> run_model_closed(size_t N, size_t I0, double dt, size_t end_time,
 double gamma, double R0) {
  std::vector<std::string> health_states = {"S", "I", "R"};
  std::vector<std::string> health_states_t0 = std::vector<std::string>(N);
  for (size_t i = 0; i < N; i++) {
    health_states_t0[i] = "S";
  }

  std::vector<size_t> ivec(N);
  std::vector<size_t> out;
  std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..n]
  std::sample(ivec.begin(), ivec.end(),
           std::back_inserter(out),
           I0, std::mt19937{std::random_device{}()});

  for (size_t i = 0; i < I0; i++) {
    health_states_t0[out[i]] = "I";
  }
  auto model = new ModelClosed(health_states, health_states_t0, N, dt, end_time,
   gamma, R0);
  model->run_simulation();
  return model->render();
}
