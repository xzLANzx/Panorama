Prerequisites:
cmake, opencv


1. To compile
———
cd panorama/cmake-build-debug/
cmake ../
make


2. To run
———
In the cmake-build-debug folder:

1st argument: number of images to stitch
2nd argument: name of images without .png

	To see stitched Rainier:
	./panorama 6 Rainier

	To see stitched MelakwaLake
	./panorama 4 MelakwaLake

	To see stitched images taken by myself
	./panorama 6 desk

	To see stitched Hangings(Extra credit)
	./panorama 2 Hanging


After Running
———
Please waiting for about 20 seconds...
"Stitching Result" will show afterwards.
The result will also be save to file as 'image_name'_Result.png

