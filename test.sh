#!/bin/bash
# Invalid Arguments
printf "Invalid Case 1: " && ./philo 1                               2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 2: " && ./philo 1 2                             2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 3: " && ./philo 1 2 3                           2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 4: " && ./philo 1 1 1 99999999999999            2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 5: " && ./philo 0 2 3 4                         2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 6: " && ./philo 201 2 3 4                       2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 7: " && ./philo 99999999999999999 2 3 4         2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 8: " && ./philo 2 300 100 100a                  2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 9: " && ./philo                                 2>/dev/null && echo "NG" || echo "OK"

# Valid and Die
printf "Valid Die Case 1: " &&  ./philo 1 2 3 4                                  && echo "OK" || echo "NG"
printf "Valid Die Case 2: " &&  ./philo 1 2 3 4 5                                && echo "OK" || echo "NG"
printf "Valid Die Case 3: " &&  ./philo 1 300 100 100                            && echo "OK" || echo "NG"
printf "Valid Die Case 4: " &&  ./philo 2 199 100 1                              && echo "OK" || echo "NG"

# Valid and Alive
printf "Valid Alive Case 1: " &&  ./philo 2 300 100 100                          && echo "OK" || echo "NG"
printf "Valid Alive Case 2: " &&  ./philo 2 300 100 100 5                        && echo "OK" || echo "NG"
printf "Valid Alive Case 3: " &&  ./philo 2 300 100 100 5                        && echo "OK" || echo "NG"
printf "Valid Alive Case 4: " &&  ./philo 2 201 100 100 5                        && echo "OK" || echo "NG"
printf "Valid Alive Case 5: " &&  ./philo 3 201 100 100 5                        && echo "OK" || echo "NG"
printf "Valid Alive Case 6: " &&  ./philo 2 200 100 100 5                        && echo "OK" || echo "NG"
printf "Valid Alive Case 7: " &&  ./philo 3 200 100 100 5                        && echo "OK" || echo "NG"
