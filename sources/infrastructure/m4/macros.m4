divert(-1)dnl

define(opening_parenthesis_index, `index(`$1', `(')')

define(check_not_null, `check(`$1' != NULL, "`$1': NULL")')

define(function_name, `substr(`$1', 0, opening_parenthesis_index(`$1'))')

define(function_params_start, `incr(opening_parenthesis_index(`$1'))')
define(function_params_length, `eval(len(`$1') - function_params_start(`$1') - 1)')
define(function_params, `substr(`$1', function_params_start(`$1'), function_params_length(`$1'))')

define(check_result, `int function_name(`$1')_result`'__line__ = function_name(`$1')(function_params(`$1')); dnl
check(function_name(`$1')_result`'__line__ == `$2', "function_name(`$1')_result: %d",dnl
function_name(`$1')_result`'__line__)')dnl

define(check_result_not, `int function_name(`$1')_result`'__line__ = function_name(`$1')(function_params(`$1')); dnl
check(function_name(`$1')_result`'__line__ != `$2', "function_name(`$1')_result: %d",dnl
function_name(`$1')_result`'__line__)')dnl

define(check_result_greater, `int function_name(`$1')_result`'__line__ = function_name(`$1')(function_params(`$1')); dnl
check(function_name(`$1')_result`'__line__ > `$2', "function_name(`$1')_result: %d",dnl
function_name(`$1')_result`'__line__)')dnl

divert(0)dnl
