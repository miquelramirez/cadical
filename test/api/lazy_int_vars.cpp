//
// Created by Miquel Ramirez on 7/07/2023.
//
#include <cadical/cadical.hpp>

struct Reason {
  std::vector<int>  lits = {};
  bool added = true;
};

struct IntVar {
  int id = -1;
  std::vector<int>  domain = {};
};

struct NotEqual {
  std::vector<int>  scope = {};
};

class Model : public CaDiCaL::ExternalPropagator {
public:

  Model() = delete;
  explicit Model(CaDiCaL::Solver* s);

  ~Model() = default;

  void notify_assignment (int lit, bool is_fixed) override {}
  void notify_new_decision_level () override {};
  void notify_backtrack (size_t new_level) override {};

  bool cb_check_found_model (const std::vector<int> &model) override;

  bool cb_has_external_clause() override;
  int  cb_add_external_clause_lit() override;

protected:

  CaDiCaL::Solver*  solver = nullptr;
  Reason            reason_failure;
};

Model::Model(CaDiCaL::Solver* s)
  : solver(s) {
  is_lazy = true;
}

bool
Model::cb_check_found_model (const std::vector<int> &model) {

}

bool
Model::cb_has_external_clause() {
  return !reason_failure.added;
}

int
Model::cb_add_external_clause_lit() {

  return 0;
}

int main(int argc, char** argv) {

  auto solver = new CaDiCaL::Solver();



  delete solver;

  return 0;
}