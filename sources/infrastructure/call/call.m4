divert(-1)dnl

define(opening_parenthesis_index, `index(`$1', `(')')

define(function_name, `substr(`$1', 0, opening_parenthesis_index(`$1'))')

define(function_params_start, `incr(opening_parenthesis_index(`$1'))')
define(function_params_length, `eval(len(`$1') - function_params_start(`$1') - 1)')
define(function_params, `substr(`$1', function_params_start(`$1'), function_params_length(`$1'))')

define(formats_comma_separated, `patsubst(`$1', ` ', `, ')')

define(formats_d_expanded, `patsubst(patsubst(`$1', `%ppd', `%p->%p->%d'), `%pd', `%p->%d')')
define(formats_f_expanded, `patsubst(patsubst(`$1', `%ppf', `%p->%p->%f'), `%pf', `%p->%f')')
define(formats_s_expanded, `patsubst(patsubst(`$1', `%pps', `%p->%p->%s'), `%ps', `%p->%s')')
define(formats_expanded, `formats_d_expanded(
                          formats_f_expanded(
                          formats_s_expanded(`$1')))')

define(comma_after_if_needed, `ifelse(`$1',`',`',`,')')

define(checked_call, `int function_name(`$1')_result = function_name(`$1')(function_params(`$1'));
check(function_name(`$1')_result == `$3', "function_name(`$1')(formats_comma_separated(formats_expanded(`$2'))): %d",
  function_params(`$1')`'comma_after_if_needed(function_params(`$1')) function_name(`$1')_result)')

divert(0)dnl
checked_call(sql_connection_open(x, y), "%pd %d", SQLITE_OK);
