#pragma once

// converts parsecs to kilometres
double Mpc2km(double x) {
	return x * 3.0857E19;
}

// converts parsecs to light years
double Mpc2ly(double x) {
	return x * 3.26156E6;
}

// converts parsecs to petametres (1E15 metre)
double Mpc2pm(double x) {
	return x * 3.0857E6;
}