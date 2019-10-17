// 50==================================================
/*!
  \file   main.cpp
  \brief

  This is the headquater of the 3D high resolution ecosystem model.
  This program will call ecosystem class member functions to initiate,
  read, run, save and exit the simulation.

  \author Chang Liao
  \date   2016-09-28
*/
// 50==================================================
#include <fenv.h> //for floating point managements
#include <iostream>
#include <string>

#include "eco3d.h" //the ecosystem model

// cros-platform header
#ifdef _WIN32
// define something for Windows (32-bit and 64-bit, this part is common)
#pragma float_control(precise, on)
#pragma fenv_access(on)
#pragma float_control(except, on)
#ifdef _WIN64
// define something for Windows (64-bit only)
#else
// define something for Windows (32-bit only)
#endif
#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
// linux

#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif
#if defined(__linux__)

#else
#endif
// 50==================================================
using namespace std;
// 50==================================================
/*!
  This is the entrance of the program

  \param : number of arguments passed in the main program
  \param  char *argv[]
  \return <ReturnValue>
*/
// 50==================================================

int main(int argc, char *argv[])
{
// cros-platform header
#ifdef _WIN32
  // define something for Windows (32-bit and 64-bit, this part is common)

#ifdef _WIN64
// define something for Windows (64-bit only)
#else
// define something for Windows (32-bit only)
#endif
#elif __APPLE__
#if TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_IPHONE
// iOS device
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#else
#error "Unknown Apple platform"
#endif
#elif __linux__
  // linux
  feenableexcept(FE_INVALID | FE_DIVBYZERO | FE_OVERFLOW | FE_UNDERFLOW);

#elif __unix__ // all unices not caught above
// Unix
#elif defined(_POSIX_VERSION)
// POSIX
#else
#error "Unknown compiler"
#endif
#if defined(__linux__)

#else
#endif


  // initial program running status as success,
  // the status variable changes to 0 if any step fails to proceed.
  std::cout << "Level 1: Start to run ecosystem model!" << std::endl;
  int program_status = 1;
  std::string sConfiguration_file = "";
  std::cout << "==Level 2: Checking input arguments:" << std::endl;
  if (argc == 2) // at least 2 arguments are needed
    {
      std::cout << "===Level 3: The following arguments are provided:" << argv[1] << std::endl; // print out all the arguments
      sConfiguration_file = argv[1];
    }
  else
    {
      std::cout << "===Level 3: No arguments are provided!" << std::endl;
      std::cout << "===Level 3: Please input the configuration file:" << std::endl;
      std::cin >> sConfiguration_file;
    }
  // 50==================================================
  // in our model, the first argument is the model itself,
  // the second argument is the configuration file
  // the other arguments are currently unused
  // 50==================================================

  // initialize the ecosystem model
  ecohydrology::eco3d cEco3d(sConfiguration_file);

  // since the class initialization does not have a return value, we check it
  // here directly
  if (cEco3d.iFlag_configuration_file ==
      0) // the configuration file is effective.
    {
      std::cout << "===Level 3: The configuration file does not exit!"
                << "\n";
      exit(0);
    }
  // initialize the class member
  std::cout << "==Level 2: Setting up the ECO3D model:" << std::endl;
  if (cEco3d.setup_eco3d() != 1)
    {
      exit(0);
    }
  // read_eco3d
  std::cout << "==Level 2: Reading data for the ECO3D model" << std::endl;
  if (cEco3d.read_eco3d() != 1)
    {
      exit(0);
    }
  // initialize other variables
  std::cout << "==Level 2: Initialize the ECO3D model" << std::endl;
  if (cEco3d.initialize_eco3d() != 1)
    {
      exit(0);
    }
  // run the simulation
  std::cout << "==Level 2: Run and Save the ECO3D model simulation" << std::endl;
  if (cEco3d.run_and_save_eco3d() != 1)
    {
      exit(0);
    }
  std::cout << "==Level 2: Cleanup the ECO3D model" << std::endl;
  if (cEco3d.cleanup_eco3d() != 1)
    {
      exit(0);
    }
  return program_status;
}
