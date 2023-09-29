#include <iostream>
#include <string>
#include <vector>
#include <adios2.h>
#include <mpi.h>

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

  while (engine.BeginStep() == adios2::StepStatus::OK) {
    auto varX = io.InquireVariable<double>("XLAT");
    auto varY = io.InquireVariable<double>("XLONG");
    auto varData = io.InquireVariable<double>(varname);

    // Get data
    std::vector<double> x, y, data;
    engine.Get(varX, x);
    engine.Get(varY, y);
    engine.Get(varData, data);

    // For demonstration purposes, print the first few values of each variable
    std::cout << "Data for step " << engine.CurrentStep() << ":\n";
    std::cout << "XLAT: " << x[0] << ", XLONG: " << y[0] << ", " << varname << ": " << data[0] << "\n";

    engine.EndStep();
  }

  engine.Close();
  MPI_Finalize();
  return 0;
}
