// By: Sesilly Cruz
#include "CImg.h"
#include "math.h"
#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace cimg_library;
using namespace std;
const double PI = 3.1415927;
CImg<unsigned char> image1("ClownFish.bmp");	// Use a bmp image file
CImg<unsigned char> imageFinal = image1;		// imageFinal is set equivalent to image1 because
												// the program crashes if left uninstantiated

struct blur{		// Struct for our threads to use in order to create
	int width;		// our image
	int height;
	int i;
	int j;
	float radius;
};

static void *blurThread(void *threadArgument){
	struct blur *thisBlur;
	thisBlur = (struct blur *) threadArgument;
	float r = thisBlur -> radius;					// Point to the current thread's variables
	int w = thisBlur -> width;						// and use them to create their section
	int h = thisBlur -> height;						// of the image
	int rs = (int)ceil(r * 2.57);    				// significant radius
	for (int i = thisBlur -> i; i < h; i++) { 		// for every column
		for (int j = thisBlur -> j; j < w; j++) { 	// for every row
			double									// RGB channels sum, weights sum
			redSum = 0,
			greenSum = 0,
			blueSum = 0,
			wSum = 0;
			for (int iy = i - rs; iy < i + rs + 1; iy++) {					//x blur mask position
				for (int ix = j - rs; ix < j + rs + 1; ix++) {				//y blur mask position
					int x = fmin(w - 1, fmax(0, ix));
					int y = fmin(h - 1, fmax(0, iy));
					double dsq = (ix - j)*(ix - j) + (iy - i)*(iy - i); 	// position square
					double wght = exp(-dsq / (2 * r*r)) / (PI * 2 * r*r); 	//weight
					redSum += ((double)image1(x, y, 0) * wght);
					greenSum += ((double)image1(x, y, 1) * wght);
					blueSum += ((double)image1(x, y, 2) * wght);
					wSum += wght;
				}
			}
			imageFinal(j, i, 0) = round(redSum / wSum);		// Update the specified coordinates
			imageFinal(j, i, 1) = round(greenSum / wSum);	// on the final image
			imageFinal(j, i, 2) = round(blueSum / wSum);
		}
	}
	return(NULL);
}


int main() {		// Using 8 threads to create the blur

	pthread_t t1, t2, t3, t4, t5, t6, t7, t8;
	struct blur b1, b2, b3, b4, b5, b6, b7, b8;

	b1.j = 0;
	b1.i = 0;
	b1.height = image1.height()/2;
	b1.width = image1.width()/4;
	b1.radius = 5;

	b2.j = image1.width()/4;
	b2.i = 0;
	b2.height = image1.height()/2;
	b2.width = image1.width()/2;
	b2.radius = 5;

	b3.j = image1.width()/2;
	b3.i = 0;
	b3.height = image1.height()/2;
	b3.width = 3*(image1.width()/4);
	b3.radius = 5;

	b4.j = 3*(image1.width()/4);
	b4.i = 0;
	b4.height = image1.height()/2;
	b4.width = image1.width();
	b4.radius = 5;

	b5.j = 0;
	b5.i = image1.height()/2;
	b5.height = image1.height();
	b5.width = image1.width()/4;
	b5.radius = 5;

	b6.j = image1.width()/4;
	b6.i = image1.height()/2;
	b6.height = image1.height();
	b6.width = image1.width()/2;
	b6.radius = 5;

	b7.j = image1.width()/2;
	b7.i = image1.height()/2;
	b7.height = image1.height();
	b7.width = 3*(image1.width()/4);
	b7.radius = 5;

	b8.j = 3*(image1.width()/4);
	b8.i = image1.height()/2;
	b8.height = image1.height();
	b8.width = image1.width();
	b8.radius = 5;

	int start_s = clock();	// Start to calculate time elapsed w/ multi-threading

	pthread_create(&t1, NULL, blurThread, (void*)&b1);	// Create all our threads
	pthread_create(&t2, NULL, blurThread, (void*)&b2);
	pthread_create(&t3, NULL, blurThread, (void*)&b3);
	pthread_create(&t4, NULL, blurThread, (void*)&b4);
	pthread_create(&t5, NULL, blurThread, (void*)&b5);
	pthread_create(&t6, NULL, blurThread, (void*)&b6);
	pthread_create(&t7, NULL, blurThread, (void*)&b7);
	pthread_create(&t8, NULL, blurThread, (void*)&b8);

	void *ret;
	pthread_join(t1, &ret);	// Make the main method wait until all threads are finished
	pthread_join(t2, &ret);	// so that the final image can be displayed
	pthread_join(t3, &ret);
	pthread_join(t4, &ret);
	pthread_join(t5, &ret);
	pthread_join(t6, &ret);
	pthread_join(t7, &ret);
	pthread_join(t8, &ret);

	int stop_s = clock();	// All threads are done, so we calculate total elapsed time
	cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC) << endl;

	CImgDisplay main_disp(imageFinal, "GaussianBlur");	// Display the image
	while (!main_disp.is_closed()) {
		main_disp.wait();
	}
	imageFinal.save("CFBlur8.bmp");		// Save the image

	pthread_exit(NULL);
}
