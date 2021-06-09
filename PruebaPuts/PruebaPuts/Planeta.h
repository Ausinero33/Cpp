#pragma once
#include "Motor.h"

class Planeta : public Motor {
public:
	void logica() override {
		puts("Adios");
	}
};