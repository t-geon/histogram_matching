# histogram_matching

This is the code I wrote through visual studio 2019 on windows.

Explanation
 It transforms the distribution of the pixel values ​​of the input image to match the distribution of the pixel values of the target image.
  
mathematical concepts
  Assume that a and b have an arbitrary distribution and that s has a uniform distribution.
  If the cdf of a is T() and the cdf of b is G(), then T(a)=s=G(b).
  We can convert a to s by substituting a for T, and we can convert s to b by mapping s to the inverse of G.

process
  1. Read the input image file.
  2. Check the probability and cdf of the input image file.
  3. Check the distribution, probability, and cdf of the target image file.
  4. Map the cdf value of the input image to the inverse of the cdf of the target image.
  5. Change the value of each pixel of the input image.
  6. Output the converted image file.
  7. Check the pixel-by-pixel distribution, cdf of the converted image file.
  
  
 - result
  If you check the graph after copying and pasting the values ​​into Excel, you can see that the distribution of the input image has been converted to be similar to the distribution of the target image.
  If you compare the input image, the target image, and the result image, you can see the converted image.
