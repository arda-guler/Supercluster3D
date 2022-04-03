#pragma once

class universe {
public:
	double scale;
	double volume;

	universe() {
		scale = 1;
		volume = 1;
	}

	universe(double s) {
		scale = s;
		volume = s * s * s;
	}

	universe& operator*=(double val);
};

