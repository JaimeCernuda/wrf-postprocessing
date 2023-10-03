#include <iostream>
#include <string>
#include <vector>
#include <adios2.h>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);

  std::string instream, varname;

  if (argc == 3) {
    instream = argv[1];
    varname = argv[2];
  } else {
    std::cerr << "Usage: " << argv[0] << " <instream>(full file path) <varname>(variabel to read)" << std::endl;
    exit(1);
  }

  adios2::ADIOS adios("postprocessing-wrf.xml");
  adios2::IO io = adios.DeclareIO("wrf-postprocessing");
  adios2::Engine engine = io.Open(instream, adios2::Mode::Read);
  std::cout << "output all variable name" << std::endl;
  std::map<std::string, adios2::Params> variables = io.AvailableVariables(); // Print all the variable names
   if (variables.empty()) {
    std::cout << "No variable" << std::endl;
    return 0;
   }
   for (const auto &variablePair : variables) {
   std::cout << "Variable Name: " << variablePair.first << std::endl;
      for(auto & k: variablePair.second) {
        std::cout <<  k.first << ": " << k.second << std::endl;
        }
   }

  while (engine.BeginStep() == adios2::StepStatus::OK) {
  std::vector<float> V;
  auto varV = io.InquireVariable<float>(varname);
  engine.Get(varV, V);
  std::cout << "the size of variable "    << std::endl;
  std::cout << "the value V: " << V.size() << std::endl;
  for (int i = 0; i < 100; i++) {
        std::cout << V[std::rand()%V.size()] << " ";
    }
  std::cout << std::endl; 

    engine.EndStep();
  }

  engine.Close();
  MPI_Finalize();
  return 0;
}
