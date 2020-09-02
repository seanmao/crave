#define BOOST_TEST_MODULE direct_BitBlast_CUDD
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/CUDD_Context.hpp>
#include <metaSMT/BitBlast.hpp>

using namespace metaSMT::solver;
using namespace metaSMT;
struct Solver_Fixture {
  typedef DirectSolver_Context<BitBlast<CUDD_Context> > ContextType;
  ContextType ctx ;
};

#include "test_solver.cpp"
#include "test_QF_BV.cpp"
#include "test_unsat.cpp"

