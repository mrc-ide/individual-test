#ifndef MODEL_H
#define MODEL_H

#include <R.h>
#include <Rcpp.h>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <numeric>
#include  <individual.h>
#include  <IterableBitset.h>
#include  <CategoricalVariable.h>

using individual_index_t = IterableBitset<uint64_t>;

class Model {
private:
  size_t N;
  double dt;
  size_t tmax;
  size_t steps;
  double gamma;
  double R0;
  double beta = R0 * gamma;
  CategoricalVariable health_;
  TargetedEvent recovery_event_;

public:
  Model(const std::vector<std::string>& health_states,
    const std::vector<std::string>& health_states_t0,
    size_t n) :
  health_(health_states, health_states_t0),
  recovery_event_(n) {
    N = n;
    dt = 0.1;
    tmax = 100;
    steps = tmax / dt;
    gamma = 0.1;
    R0 = 2.5;
    beta = R0 * gamma;
  }

  void infection_process(double t) {
    auto I = health_.get_size_of("I");
    auto foi = beta * I / N;
    individual_index_t S = health_.get_index_of("S");
    bitset_sample(S, R::pexp(foi * dt, 1, true, false));
    health_.queue_update("I", S);
  }

  void recovery_listener(double t, const individual_index_t &target) {
    health_.queue_update("R", target);
  }

  void bitset_sample(
      individual_index_t b,
      double rate
      ) {
      bitset_sample_internal(b, rate);
  }

  void recovery_process() {
    individual_index_t I = health_.get_index_of("I");
    individual_index_t already_scheduled = recovery_event_.get_scheduled();
    (I) &= (already_scheduled.inverse());
    auto rec_times = Rcpp::rgeom(I.size(), R::pexp(gamma * dt, 1, true, false));
    std::vector<double> vec_rec_times(rec_times.begin(), rec_times.end());
    recovery_event_.schedule(I, vec_rec_times);
  }

  void run_simulation() {
//    for (size_t t = 0; t < steps; ++t) {
//        infection_process(t);
//        recovery_process(t);
//        if (recovery_event->should_trigger()) {
//          auto target = new individual_index_t(recovery_event->current_target());
//          recovery_listener(t, target);
//        }
//        health_->update();
//        recovery_event->resize();
//        health_->resize();
//        recovery_event->tick();
//    }
  }
};

#endif
