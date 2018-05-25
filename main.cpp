#include "CImg.h" // http://cimg.eu/
#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;

int main() {
	cout << "Resolution: ";
	int a;
	cin >> a; cin.ignore();
	double cx, cy;
	cout << "c (real): ";
	cin >> cx; cin.ignore();
	cout << "c (imaginary): ";
	cin >> cy; cin.ignore();
	cout << "Iterations: ";
	unsigned short iterations;
	cin >> iterations; cin.ignore();

	cimg_library::CImg<unsigned short> julia(a * 2, a, 1, 1);

	float start = static_cast<float>(clock());
	omp_set_num_threads(omp_get_num_procs());
#pragma omp parallel for schedule(dynamic)
	for (int y = -(a / 2); y < (a / 2); y += 1) {
		for (int x = -a; x < a; x += 1) {
			double zx = -(x / (double)(a / 2)), zy = y / (double)(a / 2);
			double xtemp;

			unsigned short i = 0;
			for (i = 0; i < iterations && zx * zx - zy * zy < 4; i += 1) {
				xtemp = zx * zx - zy * zy;
				zy = 2 * zx * zy + cy;
				zx = xtemp + cx;
			}
			julia(x + a, y + (a / 2)) = i;
		}
	}
	cout << (static_cast<float>(clock()) - start) / 1000 << " s" << endl;
	if (MessageBox(NULL, "Would you like to save the image?", "Julia", MB_YESNO) == 6)
		julia.save_png("D:\julia.png");
	julia.display();
}
