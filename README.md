SERBAN Victor Gabriel, 313CA

The implemented program presents a text-based interface that allows transformations on image files.

To develop the program, I defined two different types of structures: one for storing color images (struct pixel) and another for storing the corners of the working area (struct colt).

The program operates on image files, which it loads using the LOAD function. File operations are performed exclusively within the LOAD and SAVE subprograms. The SAVE function opens a file to write the transformed image into it.

The program runs based on a set of commands provided as input by the user. These commands determine the actions the user wishes to perform, triggering the necessary functions accordingly.

The possible commands are:

LOAD <filename>
SAVE <filename>
APPLY <parameter>
HISTOGRAM x y
EQUALIZE
SELECT <x1> <y1> <x2> <y2>
SELECT ALL
CROP
ROTATE <angle>
EXIT

The APPLY function applies a kernel to color images. This process involves multiplying a 3x3 kernel with 3x3 sections of the image. Applying a kernel alters the appearance of the image. This subprogram accepts parameters such as <EDGE>, <SHARPEN>, <BLUR>, and <GAUSSIAN_BLUR>. Depending on the given parameter, a different kernel is applied. After multiplying the image sections with the kernel, all elements are summed to obtain a pixel in the new image.

The HISTOGRAM function creates and displays a histogram for grayscale images based on the input parameters (the number of bins and "*" characters). The program calculates the frequency of all possible pixel values in the image. Based on the number of bins, it determines the bin size. A formula is then used to calculate the number of "*" for each bin, followed by the histogram display.

The EQUALIZE function performs histogram equalization on grayscale images, reducing the effect of incorrect exposure and enhancing image details. It applies a formula specifically designed for image equalization.

The SELECT function restricts subsequent operations to the pixels within the rectangle defined by the provided coordinates <x1> <y1> <x2> <y2>. It verifies whether the input coordinates are valid for the loaded image. If they are, the working area is updated to reflect the new selection.

The SELECT ALL function selects the entire image and updates the corners accordingly, allowing operations to be performed on the entire image.

The CROP function cuts the image based on the current selection. The selection becomes the new image, and the corners are updated to reflect the changes. The new image contains all pixels from the previous selection.

The ROTATE function rotates the current selection by the given <angle>. If the entire image is selected, it rotates completely, modifying the number of rows and columns based on the angle. However, if the selection is not the entire image, the subprogram only works on selections where the number of rows and columns are equal.

The ROTATE function relies on another function, ROTATE_90, which determines the image type (color or grayscale) and then calls a subprogram that rotates the selection based on the angle. A 90-degree rotation involves swapping the number of rows and columns and allocating a new image with the corresponding dimensions and appearance.

The EXIT function frees the allocated memory for images and terminates the program.

In each function, dynamically allocated memory is freed after use if it is no longer needed.

Files are closed immediately after their operations are completed.

Thus, the program runs correctly and efficiently, without errors or memory leaks, meeting all assignment requirements while adhering to the imposed coding style.







