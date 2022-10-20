#!/bin/bash
function print_ng() {
	RED='\033[0;31m'
	NC='\033[0m' # No Color
	printf "${RED}NG${NC}\n"
}
function print_ok() {
	GREEN='\033[0;32m'
	NC='\033[0m' # No Color
	printf "${GREEN}OK${NC}\n"
}
on_int(){
	echo
}
# register signal handler subroutine for SIGINT
trap on_int SIGINT

# Invalid Arguments
printf "Invalid Case 1: " && ./philo 3                          >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 2: " && ./philo 3 2                        >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 3: " && ./philo 3 2 1                      >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 4: " && ./philo 3 2 1 99999999999999       >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 5: " && ./philo 0 2 3 4                    >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 6: " && ./philo 201 2 3 4                  >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 7: " && ./philo 99999999999999999 2 3 4    >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 8: " && ./philo 2 300 100 100a             >/dev/null 2>&1 && print_ng || print_ok
printf "Invalid Case 9: " && ./philo                            >/dev/null 2>&1 && print_ng || print_ok

# Basic Tests
printf "./philo 1 800 200 200: \n"     &&  ./philo 1 800 200 200           2>&1 | tail -1
printf "./philo 5 800 200 200: \n"     &&  ./philo 5 800 200 200           2>&1 | tail -1
printf "./philo 5 800 200 200 7: \n"   &&  ./philo 5 800 200 200 7         2>&1 | tail -1
printf "./philo 4 410 200 200: \n"     &&  ./philo 4 410 200 200           2>&1 | tail -1
printf "./philo 4 310 200 100: \n"     &&  ./philo 4 310 200 100           2>&1 | tail -1

# Dead
printf "Dead Case 1: \n"               &&  ./philo 4 300 200 101           2>&1 | tail -1
printf "Dead Case 2: \n"               &&  ./philo 4 300 200 101 10        2>&1 | tail -1
printf "Dead Case 3: \n"               &&  ./philo 3 300 200 100           2>&1 | tail -1
printf "Dead Case 4: \n"               &&  ./philo 2 199 100 10            2>&1 | tail -1

# Alive
printf "Alive Case 1: \n"              &&  ./philo 2 210 100 100 10        2>&1 | tail -1
printf "Alive Case 2: \n"              &&  ./philo 3 310 100 100 10        2>&1 | tail -1
printf "Alive Case 3: \n"              &&  ./philo 4 410 200 100 10        2>&1 | tail -1
printf "Alive Case 4: \n"              &&  ./philo 5 510 200 100 10        2>&1 | tail -1
printf "Alive Case 5: \n"              &&  ./philo 6 610 300 100 10        2>&1 | tail -1
printf "Alive Case 6: \n"              &&  ./philo 7 710 300 100 10        2>&1 | tail -1
printf "Alive Case 7: \n"              &&  ./philo 199 2000 990 1000 10    2>&1 | tail -1
printf "Alive Case 8: \n"              &&  ./philo 200 2010 1000 1000 10   2>&1 | tail -1

# Extreme Alive
printf "Extreme Alive Case 1: \n"      &&  ./philo 2 200 100 100 10        2>&1 | tail -1
printf "Extreme Alive Case 2: \n"      &&  ./philo 3 300 100 100 10        2>&1 | tail -1
printf "Extreme Alive Case 3: \n"      &&  ./philo 4 400 200 100 10        2>&1 | tail -1
printf "Extreme Alive Case 4: \n"      &&  ./philo 5 500 200 100 10        2>&1 | tail -1
printf "Extreme Alive Case 5: \n"      &&  ./philo 6 600 300 100 10        2>&1 | tail -1
printf "Extreme Alive Case 6: \n"      &&  ./philo 7 700 300 100 10        2>&1 | tail -1
printf "Extreme Alive Case 7: \n"      &&  ./philo 199 1990 990 1000 10    2>&1 | tail -1
printf "Extreme Alive Case 8: \n"      &&  ./philo 200 2000 1000 1000 10   2>&1 | tail -1
