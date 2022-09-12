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
  std::vector<size_t> deaths_;

public:
  Model(const std::vector<std::string>& health_states,
    const std::vector<std::string>& health_states_t0,
    size_t n,
    double dt,
    size_t end_time,
    double gamma,
    double R0,
    double birth_rate,
    double death_rate) :
  health_(health_states, health_states_t0),
  recovery_event_(n),
  counts_((end_time / dt) * health_states.size()),
  deaths_(n) {
    N_ = n;
    dt_ = dt;
    tmax_ = end_time;
    steps_ = tmax_ / dt_;
    gamma_ = gamma;
    R0_ = R0;
    beta_ = R0 * gamma;
    birth_rate_ = birth_rate;
    death_rate_ = death_rate;
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

  void birth_process(size_t t) {
    auto n_births = R::rpois(birth_rate_ / dt_);
    std::vector<std::string> new_s = std::vector<std::string>(n_births, "S");
    health_.queue_extend(new_s);
    recovery_event_.queue_extend(n_births);
  }

  void death_process(size_t t) {
    size_t pop_size = health_.size();
    deaths_.resize(pop_size);
    std::iota(deaths_.begin(), deaths_.end(), 0);
    double dr = death_rate_ / dt_;
    if (dr > 1) {
      dr = 1;
    }
    int n_deaths = R::rbinom(pop_size, dr);
    for (size_t i = 0; i < n_deaths - 1; ++i) {
       size_t j = floor(R::unif_rand() * (pop_size - i)) + i;
       deaths_[i] = deaths_[j];
       deaths_[j] = deaths_[i];
    }

    deaths_.resize(n_deaths);

    health_.queue_shrink(deaths_);
    recovery_event_.queue_shrink(deaths_);
  }

  void run_simulation() {
    for (size_t t = 0; t < steps_; ++t) {
        // execute processes
        infection_process(t);
        recovery_process(t);
        render_process(t);
        birth_process(t);
        death_process(t);

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
