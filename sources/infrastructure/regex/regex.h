#ifndef REGEX_H
#define REGEX_H

// matches a string against a regular expression
int regex_match(char *pattern, char *string, int *matched, char ***tokens, int *tokens_count);

#endif
