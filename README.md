Just an example how can be used Freetype2 standalone rasterizer.
---

![alt tag](https://raw.github.com/caway/FreeType2-Rasterizer-CPP-Wrapper/master/example.png)

Example:
```cpp

#include "ft2_rasterizer.h"

int main()
{
	ft2::rasterizer r;
	if ( r.is_valid() ) {
		r.set_buffer( ... );

		int n = 100;
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

	}
	return 0;
}
```
