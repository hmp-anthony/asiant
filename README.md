## asiant

AI functionality based on the book "AI for Games" by Ian Millington.

My code improves on that suggested in the book in the following ways:
- The path following algorithm in the book is O(n), where n is the number of line_segments in the path.  I get away with a O(1) implementation by getting the character to follow a path_constrained_entity.
- In seek with velocity radius that appears in the book, the factor that provides a gradient between full speed and zero
is incorrect. I have corrected it in my implementation. 

To build and execute just type `make` and then `./build/demos/seek/seek`, for instance.
