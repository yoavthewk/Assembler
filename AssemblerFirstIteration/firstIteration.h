#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../Hardware/hardware.h"
#include "../FileHandling/files.h"

void containsLabel(char* line);
bool isExtern(char* line);
bool isEntry(char* line);
void processData(char* line);
void isData(char* line);