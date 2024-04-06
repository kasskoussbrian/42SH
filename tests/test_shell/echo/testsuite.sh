# Basic tests
run_test echo.test
run_test echo_empty.test
run_test echo2.test
run_test echo_classic.test

# Test arguments (-eeEEen -E...)
run_test echo_EeEennnEE.test
run_test echo_eeenee.test
run_test echo_e.test
run_test echo_E.test
run_test echo_n.test
run_test echo_fake_arg.test

# Test arguments quotes
run_test echo_simple_quote.test
run_test echo_double_quote.test
run_test echo_simple_double_quote.test
run_test echo_double_simple_quote.test
run_test echo_quote_fun.test

# Test fail dementor 1
run_test echo_dementor.test
run_test echo_dementor2.test
run_test echo_dementor3.test

# Test fail dementor 3
run_test echo_big_args.test
