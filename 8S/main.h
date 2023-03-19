#ifndef __MAIN_H_
#define __MAIN_H_

#define uchar unsigned char 
#define uint unsigned int

#include <STC15F2K60S2.H>

#include "Init.h"
#include "SMG.h"
#include "YanShi.h"
#include "DLKey.h"
#include "DSQ.h"
#include "onewire.h"
#include "ds1302.h"

unsigned char DLKey_GetKeynum();
void LED_Show();

#endif