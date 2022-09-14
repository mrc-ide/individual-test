#ifndef MODEL_CLOSED_H
#define MODEL_CLOSED_H

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

class ModelClosed {
private:
  size_t N_;
  double dt_;
  size_t tmax_;
  size_t steps_;
  size_t size_;
  double gamma_;
  double R0_;
  double beta_;
  double birth_rate_;
  double death_rate_;
  CategoricalVariable health_;
  TargetedEvent recovery_event_;
  std::vector<size_t> counts_;

public:
  ModelClosed(const std::vector<std::string>& health_states,
    const std::vector<std::string>& health_states_t0,
    size_t n,
    double dt,
    size_t end_time,
    double gamma,
    double R0) :
  health_(health_states, health_states_t0),
  recovery_event_(n),
  counts_((end_time / dt) * health_states.size()) {
    N_ = n;
    dt_ = dt;
    tmax_ = end_time;
    steps_ = tmax_ / dt_;
    gamma_ = gamma;
    R0_ = R0;
    beta_ = R0 * gamma;
    size_ = health_states.size();
  }

  void infection_process(double t) {
    auto I = health_.get_size_of("I");
    auto foi = beta_ * I / N_;
    individual_index_t S = health_.get_index_of("S");
    bitset_sample(S, R::pexp(foi * dt_, 1, true, false));
    health_.queue_update("I", S);
  }

  void recovery_listener(double t, const individual_index_t &target) {
    health_.queue_update("R", target);
  }

  void bitset_sample(
      individual_index_t& b,
      double rate
      ) {
      bitset_sample_internal(b, rate);
  }

  void recovery_process(size_t t) {
    individual_index_t I = health_.get_index_of("I");
    individual_index_t already_scheduled = recovery_event_.get_scheduled();
    I &= !(already_scheduled);
    auto rec_times = Rcpp::rgeom(I.size(), R::pexp(gamma_ * dt_, 1, true, false));
    std::vector<double> vec_rec_times(rec_times.begin(), rec_times.end());
    for (size_t i = 0; i < I.size(); ++i) {
        vec_rec_times[i] = vec_rec_times[i] + 1;
    }
    recovery_event_.schedule(I, vec_rec_times);
  }

  void render_process(size_t t) {
    counts_[(t * size_)] = health_.get_index_of("S").size();
    counts_[(t * size_) + 1] = health_.get_index_of("I").size();
    counts_[(t * size_) + 2] = health_.get_index_of("R").size();
  }

  void run_simulation() {
    for (size_t t = 0; t < steps_; ++t) {
        // execute processes
        infection_process(t);
        recovery_process(t);
        render_process(t);

        // process events
        if (recovery_event_.should_trigger()) {
          auto target = new individual_index_t(recovery_event_.current_target());
          recovery_listener(t, *target);
        }

        // update variables
        health_.update();

        // resize events
        recovery_event_.resize();

        // resize variables
        health_.resize();

        // events tick
        recovery_event_.tick();
    }
  }

  std::vector<size_t> render() {
    return counts_;
  }
};

#endif
