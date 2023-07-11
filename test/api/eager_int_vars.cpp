//
// Created by Miquel Ramirez on 11/07/2023.
//
#include <cadical/cadical.hpp>
#include <memory>
#include <unordered_map>
#include <cassert>
#include <iostream>

struct Reason {
    std::vector<int> lits = {};
    bool added = true;
    int  lit_idx = 0;
};

struct IntVar {
    int id = -1;
    std::vector<int> domain = {};
    std::vector<int> eq_lits = {};
};

struct NotEqual {
    std::vector<int> scope = {};
};

class Model : public CaDiCaL::ExternalPropagator {
public:

    typedef std::tuple<int, int>    LitKey;
    typedef std::unordered_map<int, LitKey>    LitTable;

    typedef std::shared_ptr<Model>  ptr;

    Model() = delete;

    explicit Model(CaDiCaL::Solver *s);

    ~Model() = default;

    void    prepare();

    int     new_var(const std::vector<int>& D);
    void    add_not_equal(const std::vector<int>& V);

    void    notify_assignment(int lit, bool is_fixed) override;
    void    notify_new_decision_level() override;
    void    notify_backtrack(size_t new_level) override;

    bool    cb_check_found_model(const std::vector<int> &model) override;

    int     cb_decide() override;
    int     cb_propagate() override;

    int     cb_add_reason_clause_lit(int propagated_lit) override;

    bool    cb_has_external_clause() override;
    int     cb_add_external_clause_lit() override;

    int     value(int x_id) const;

    std::vector<int>& trail() { return int_trail; }

protected:

    CaDiCaL::Solver*        solver = nullptr;
    Reason                  reason_failure;
    std::vector<IntVar>     vars;
    std::vector<NotEqual>   constraints;
    LitTable                bool_lit_map;
    int                     bool_var_hi = 1;
    std::vector<int>        int_trail;
    int                     tail = -1;
    std::vector<int>        assigned;
    std::vector<int>        levels;
    int                     level = -1;

public:
    // statistics
    int                     num_calls = 0;
    int                     num_neq_confl = 0;
};

Model::Model(CaDiCaL::Solver *s)
        : solver(s) {
    solver->connect_external_propagator(this);
}

int
Model::new_var(const std::vector<int> &D) {

    int var_id = (int)vars.size();
    std::vector<int> eq(D.size());
    for (std::size_t i = 0; i < D.size(); i++) {
        eq[i] = bool_var_hi++;
        bool_lit_map[eq[i]] = LitKey{var_id, i};
        solver->add_observed_var(eq[i]);
        solver->add(eq[i]);
    }

    // Submit disjunction clause to solver
    solver->add(0);

    // at-most-one
    for (std::size_t i = 0; i < D.size(); i++) {
        for (std::size_t j = i + 1; j < D.size(); j++) {
            solver->add(-eq[i]);
            solver->add(-eq[j]);
            solver->add(0);
        }
    }

    vars.emplace_back(IntVar{(int)vars.size(), D, eq});

    return var_id;
}

void
Model::prepare() {
    tail = -1;
    int_trail.resize(vars.size(), -1);
    assigned.resize(vars.size(), -1);
    levels.resize(vars.size(), -1);
}

int
Model::value(int x_id) const {
    if (int_trail.empty()) return -1;
    return vars.at(x_id).domain[int_trail[x_id]];
}

void
Model::add_not_equal(const std::vector<int> &V) {
    constraints.push_back(NotEqual{V});
}

void
Model::notify_assignment(int lit, bool is_fixed) {
    num_calls++;

    if (!bool_lit_map.contains(lit))
        return;

    auto [x, v] = bool_lit_map[lit];
    assert(int_trail[x] < 0);
    int_trail[x] = v;
    tail++;
    assigned[tail] = x;
    levels[tail] = level;

    for (const auto& c: constraints) {

        auto l_id = c.scope[0];
        const auto& l_x = vars[l_id];
        if (int_trail[l_id] < 0)
            continue;

        auto r_id = c.scope[1];
        const auto& r_x = vars[r_id];

        if (int_trail[r_id] < 0)
            continue;

        if (int_trail[l_id] == int_trail[r_id]) {
            // conflict
            num_neq_confl++;
            reason_failure.lits.resize(2);
            reason_failure.lits[0] = -(l_x.eq_lits[int_trail[l_x.id]]);
            reason_failure.lits[1] = -(r_x.eq_lits[int_trail[r_x.id]]);
            reason_failure.added = false;
            return;
        }
    }
}

void
Model::notify_new_decision_level() {
    level++;
}

void
Model::notify_backtrack(size_t new_level) {
    while (tail >= 0 && tail > (int)new_level) {
        int_trail[assigned[tail]] = -1;
        tail--;
    }
}

int
Model::cb_decide() {
    return 0;
}

int
Model::cb_propagate() {

    return 0;
}

int
Model::cb_add_reason_clause_lit(int propagated_lit) {
    (void) propagated_lit;
    return 0;
}

bool
Model::cb_check_found_model(const std::vector<int> &model) {
    num_calls++;

    // 0. Construct trail and Check Domain constraints, which are partially encoded
    // MRJ: Note that model only contains literals of variables observed by the current propagator
    for (std::size_t lit_idx = 0; lit_idx < model.size(); lit_idx++) {
        if (model[lit_idx] > 0) {
            auto [x, v] = bool_lit_map[model[lit_idx]];
            assert(int_trail[x] < 0);
            int_trail[x] = v;
        }
    }

    // 1. NotEqual(x,y) constraints
    for (const auto& c: constraints) {

        auto l_id = c.scope[0];
        const auto& l_x = vars[l_id];

        auto r_id = c.scope[1];
        const auto& r_x = vars[r_id];

        if (int_trail[l_id] == int_trail[r_id]) {
            // conflict
            num_neq_confl++;
            reason_failure.lits.resize(2);
            reason_failure.lits[0] = -(l_x.eq_lits[int_trail[l_x.id]]);
            reason_failure.lits[1] = -(r_x.eq_lits[int_trail[r_x.id]]);
            reason_failure.added = false;
            return false;
        }
    }

    return true;
}

bool
Model::cb_has_external_clause() {
    return !reason_failure.added;
}

int
Model::cb_add_external_clause_lit() {

    assert(!reason_failure.added);

    if (reason_failure.lit_idx < (int)reason_failure.lits.size()) {
        return reason_failure.lits[reason_failure.lit_idx++];
    }

    reason_failure.added = true;
    reason_failure.lit_idx = 0;
    return 0;
}

int main(int argc, char **argv) {

    enum Color {
        red = 1,
        green,
        blue
    };

    auto solver = new CaDiCaL::Solver();

    auto cp_model = std::make_shared<Model>(solver);

    std::vector<int> dom{red, green, blue};

    int WA = cp_model->new_var(dom);
    int NT = cp_model->new_var(dom);
    int SA = cp_model->new_var(dom);
    int QLD = cp_model->new_var(dom);
    int NSW = cp_model->new_var(dom);
    int VIC = cp_model->new_var(dom);
    int TAS = cp_model->new_var(dom);

    std::vector<int> vars{WA, NT, SA, QLD, NSW, VIC, TAS};


    cp_model->add_not_equal({WA, NT});
    cp_model->add_not_equal({WA, SA});
    cp_model->add_not_equal({NT, SA});
    cp_model->add_not_equal({NT, QLD});
    cp_model->add_not_equal({SA, QLD});
    cp_model->add_not_equal({SA, NSW});
    cp_model->add_not_equal({SA, VIC});
    cp_model->add_not_equal({QLD, NSW});
    cp_model->add_not_equal({NSW, VIC});

    cp_model->prepare();

    int res = solver->solve();

    assert(res == 10);

    assert(!cp_model.int_trail.empty());

    auto var_to_name = [](int x_id) -> std::string {
        switch(x_id) {
            case 0: return "WA";
            case 1: return "NT";
            case 2: return "SA";
            case 3: return "QLD";
            case 4: return "NSW";
            case 5: return "VIC";
            case 6: return "TAS";
            default:
                break;
        }
        return "UNK";
    };

    auto color_to_name = [](Color c) -> std::string {
        switch(c) {
            case red: return "R";
            case green: return "G";
            case blue: return "B";
            default:
                break;
        }
        return "???";
    };

    std::cout   << "# Active vars: " << solver->active()
                << " Clauses: # Irredundant: " << solver->irredundant()
                << " # Redundant: " << solver->redundant() << std::endl;
    solver->resources();
    solver->statistics();

    std::cout << "Calls: " << cp_model->num_calls << " NEQ conflicts: " << cp_model->num_neq_confl << std::endl;

    int nvars = (int)cp_model->trail().size();
    for (int x_id = 0; x_id < nvars; x_id++) {
        std::cout << var_to_name(x_id) << " = " << color_to_name((Color)cp_model->value(x_id)) << std::endl;
    }

    delete solver;

    return 0;
}