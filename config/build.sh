#!/bin/bash
delete_log_file() {
  if [ -d ../logs ]; then
      echo "Cleaning generated log files..."
      rm -rf ../logs
  fi
}
delete_build_file() {
  if [ -d ../build ]; then
      echo "Cleaning generated build files..."
      rm -rf ../build
  fi
}
delete_db_file() {
  if [ -d ../data ]; then
      echo "Cleaning generated database files..."
      rm -rf ../data
  fi
}
clean() {
  #------Log Files---------
  delete_log_file
  #------Build Files-------
  delete_build_file
  #------Database Files---------
  delete_db_file
}

arg_error() {
  echo -e " Error: no arguments provided.\n "
  echo -e " Usage: $0 [ --run | --debug | --clean ]\n "
}

inv_arg_error() {
  echo -e " Error: Invalid argument '$1'\n "
  echo -e " Usage: $0 [ --run | --debug | --clean ]\n "
  exit 1;
}

setup_directories() {
  # Create directory for database files, build files, & log files
  echo "Setting up build directories..."
  mkdir -p ../data
  mkdir -p ../build
  mkdir -p ../logs

  if [ ! -f ../logs/memcheck.log ]; then
      touch ../logs/memcheck.log
  fi
  if [ ! -f ../logs/errorlogs.log ]; then
      touch ../logs/errorlogs.log
  fi
}

build_project() {
  # change directory to the build directory, and build with Cmake & make
  cd ../build || { echo "Failed to change directory to 'build'"; exit 1; }
  cmake ../config/ || { echo "CMake configuration failed"; exit 1; }
  make main || { echo "Build failed"; exit 1; }
}

run_project() {
  # Run program with ./main or debug with valgrind
  if [[ "$1" == "--debug" ]]; then
      echo "Build successful. running the program with GDB debugger..."
      gdb ./main
  else
      echo "Build successful. running the program..."
      ./main
  fi
}

#? Check if no argument is provided
if [[ $# -eq 0 ]]; then
    arg_error
    exit 1
fi

# Check provided arguments "--run", "--memcheck", "--clean", or "--debug".
if [[ "$1" == "--clean" ]]; then
    clean
    exit 0
fi

if [[ "$1" == "--debug" ]] || [[ "$1" == "--run" ]]; then
    delete_log_file

    setup_directories

    build_project

    run_project "$@"
else
    inv_arg_error "$@"
fi