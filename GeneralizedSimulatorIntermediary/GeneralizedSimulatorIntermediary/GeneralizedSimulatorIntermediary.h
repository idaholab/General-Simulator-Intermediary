//Copyright 2022, Battelle Energy Alliance, LLC
// GeneralizedSimulatorIntermediary.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <stdio.h>
#include <signal.h>
#include "NetworkHandling/NetworkHandler.h"
#include "DataHandling/InputScanning.h"

#ifdef WINDOWS

#endif

void dropInMenu(int value);

void constructResources();
void disposeResources();