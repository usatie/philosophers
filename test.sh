#!/bin/bash
# Invalid Arguments
printf "Invalid Case 1: " && ./philo 1                               2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 2: " && ./philo 1 2                             2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 3: " && ./philo 1 2 3                           2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 4: " && ./philo 1 1 1 99999999999999            2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 5: " && ./philo 0 2 3 4                         2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 6: " && ./philo 201 2 3 4                       2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 7: " && ./philo 99999999999999999 2 3 4         2>/dev/null && echo "NG" || echo "OK"
printf "Invalid Case 8: " && ./philo                                 2>/dev/null && echo "NG" || echo "OK"

# Valid Arguments
printf "Valid Case 1: " &&  ./philo 1 2 3 4                                      && echo "OK" || echo "NG"
printf "Valid Case 2: " &&  ./philo 1 2 3 4 5                                    && echo "OK" || echo "NG"
printf "Valid Case 3: " &&  ./philo 2 200 100 100                                && echo "OK" || echo "NG"
printf "Valid Case 4: " &&  ./philo 2 200 100 100 5                              && echo "OK" || echo "NG"
printf "Valid Case 5: " &&  ./philo 2 200 100 100 5                              && echo "OK" || echo "NG"
