/*
 * Utils.cpp
 *
 *  Created on: Sep 6, 2013
 *      Author: dev
 */

#include "Utils.h"
#include "Log.h"
#include <iostream>
#include <string>


Utils::Utils()
{
}

Utils::~Utils()
{
}

void Utils::Sleep(unsigned long microSeconds, unsigned long seconds)
{
    struct timespec sleepTime, remTime;
    sleepTime.tv_sec = seconds;
    sleepTime.tv_nsec = microSeconds * 1000;
    nanosleep (&sleepTime, &remTime);
}



