
#define BOOST_TEST_MODULE direct_PicoSAT
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/SAT_Clause.hpp>
#include <metaSMT/backend/PicoSAT.hpp>
#include <metaSMT/BitBlast.hpp>

using namespace metaSMT::solver;
using namespace metaSMT;
struct Solver_Fixture
{
  typedef DirectSolver_Context<BitBlast<SAT_Clause<PicoSAT> > > ContextType;
  ContextType ctx;
};

#include "test_solver.cpp"
#include "test_QF_BV.cpp"
#include "test_unsat.cpp"
