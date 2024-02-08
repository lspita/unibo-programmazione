#ifndef INPUT_H
#define INPUT_H

void fflush_stdin();
int limited_input(const char *const message, const int min, const int max);
char yesno_question(const char *const question);

#endif