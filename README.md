Just an example how can be used Freetype2 standalone rasterizer.
---

![alt tag](https://raw.github.com/caway/Freetype2-Standalone-CPP-Rasterizer-Wrapper/master/example.png)

Example:
```cpp

#include "ft2ras_wrapper.hpp"

int main()
{
	ft2::rasterizer r;
	if ( r.is_valid() ) {

		r.set_buffer(
			buffer_pointer,
			buffer_width,
			buffer_height,
			buffer_stride ); // in bytes

		// render circle with radius 100 at 0,0
		int n = 64;
		for ( int i = 0; i < n; i++ ) {
			float angle = float(i) / n * 3.1415 * 2;
			float x = sin( angle ) * 100;
			float y = cos( angle ) * 100;
			if ( !i ) {
				r.move_to( x, y );
			} else {
				r.line_to( x, y );
			}
		}
		r.render();

		...
		blit buffer to screen or save to file
		...

	}
	return 0;
}
```
