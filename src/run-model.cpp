#include "model.hpp"

//[[Rcpp::export]]
Rcpp::XPtr<Model> create_model() {
  N = 1e3;
  I0 = 5;
  S0 = N - I0;
  std::vector<std::string> health_states = {"S", "I", "R"};
  std::vector<std::string> health_states_t0 = std::vector<std::string>(N);
  for (size_t i; i < N, i++) {
    health_states_t0[i] = "N"
  }
  std::vector<int> ivec(100);
  std::vector<int> out;
  std::iota(ivec.begin(), ivec.end(), 0); // ivec will become: [0..99]
  std::sample(ivec.begin(), ivec.end(),
           std::back_inserter(out),
           I0, std::mt19937{std::random_device{}()});

  for (size_t i; i < I0, i++) {
    health_states_t0[out[i]] = "I"
  }
  auto health = new individual::CategoricalVariable(health_states, out);
  auto recovery_event = individual::TargetedEvent(N);
  return Rcpp::XPtr<Model>(new Model(health, recovery_event), true);
}

//[[Rcpp::export]]
void run_model(const Rcpp::XPtr<Model> m) {
  m->run_simulation();
}
