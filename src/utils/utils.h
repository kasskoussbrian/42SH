#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

char *my_strdup(const char *buff);
char *my_itoa(int value, char *s);
char *add_char(char *buff, char c);
char *generate_random(void);
char *get_pwd(void);

#endif /* ( ! UTILS_H ) */
