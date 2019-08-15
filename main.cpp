#include "CImg.h" // http://cimg.eu/
#include <chrono>
#include <iostream>
#include <omp.h>
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
	int a = stoi(argv[1]) / 2, b = stoi(argv[2]) / 2;
	double cx = stod(argv[3]), cy = stod(argv[4]);
	unsigned short iterations = stoi(argv[5]);

	cout << a * 2 << "x" << b * 2<< " " << argv[3] << " " << argv[4] << " " << argv[5] << endl;

	cimg_library::CImg<unsigned short> julia(a, b, 1, 1);

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	omp_set_num_threads(omp_get_num_procs());
#pragma omp parallel for schedule(dynamic)
	for (int y = -b; y < b; y += 1) {
		for (int x = -a; x < a; x += 1) {
			double zx = -(x / (double)a), zy = y / (double)b;
			double xtemp;

			unsigned short i = 0;
			for (i = 0; i < iterations && zx * zx - zy * zy < 4; i += 1) {
				zx = zx * zx - zy * zy + cx;
				zy = 2 * zx * zy + cy;
			}
			julia(x + a, y + b) = i;
		}
	}
	cout << chrono::duration_cast<chrono::duration<double>>(chrono::high_resolution_clock::now() - start).count() << " s" << endl;

	ostringstream ss;
	ss << "./" << a << "x" << b << " " << argv[3] << " " << argv[4] << " " << argv[5] << ".png";
	string filename = ss.str();
	julia.save_png(filename.c_str());

	return 0;
}
