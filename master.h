#ifndef MASTER_H
#define MASTER_H

#include "mail.h"

void ConstructMail(char dirname[],struct mail_t mail[],char word[],
                   int *mailsize);
static int realsize=0;
static int signals=0;
static unsigned int totalcount=0;

#endif
