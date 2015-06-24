#ifndef	__FT2_RASTERIZER_H__
#define	__FT2_RASTERIZER_H__

namespace ft2 {

#define	_STANDALONE_

#define FT_ABS( a )		((a) < 0 ? -(a) : (a))

#define FT_HYPOT( x, y )                 \
          ( x = FT_ABS( x ),             \
            y = FT_ABS( y ),             \
            x > y ? x + ( 3 * y >> 3 )   \
                  : y + ( 3 * x >> 3 ) )

#include "ftgrays/ftgrays.c"

class rasterizer {
	void close_contour() {
		if ( m_pnts.size() ) { // close 
			m_cont.push_back( m_pnts.size() - 1 );
			++m_outline.n_contours;
		}
	}

	void add_point( float x, float y, char type = 1 ) {
		m_tform( x, y );
		m_pnts.push_back( { (FT_Pos)( x * 64 ), (FT_Pos)( y * 64 ) } );
		m_tags.push_back( type );
		++m_outline.n_points;
	}

public:
	rasterizer( const size_t pool_size = 1024 * 64 ) :
		m_pool( new (std::nothrow) uint8_t [pool_size] ),
		m_tform( []( float &, float & ){} )
	{
		if ( m_pool ) {
			memset( &m_outline, 0, sizeof( FT_Outline ) );
			memset( &m_params, 0, sizeof( FT_Raster_Params ) );
			memset( &m_bitmap, 0, sizeof( FT_Bitmap ) );

			m_outline.flags = FT_OUTLINE_EVEN_ODD_FILL;

			m_bitmap.num_grays = 256;
			m_bitmap.pixel_mode = FT_PIXEL_MODE_GRAY;
			m_bitmap.buffer = nullptr;

			m_params.source	= &m_outline;
			m_params.target	= &m_bitmap;
			m_params.flags	= FT_RASTER_FLAG_AA;

			ft_grays_raster.raster_new( 0 /* unused */, &m_ras );
			ft_grays_raster.raster_reset( m_ras, m_pool, pool_size );
		}
	}

	virtual ~rasterizer() {
		delete [] m_pool;
	}

	bool is_valid() const {
		return !!m_pool;
	}

	void set_buffer( uint8_t * ptr, int width, int height, int stride ) {
		m_bitmap.rows = height;
		m_bitmap.width = width;
		m_bitmap.pitch = -stride;
		m_bitmap.buffer = ptr;
	}

	void fill_even_odd( bool flag ) {
		m_outline.flags = flag ? FT_OUTLINE_EVEN_ODD_FILL : 0;
	}

	void reset() {
		m_tags.clear();
		m_cont.clear();
		m_pnts.clear();
		m_outline.n_contours = 0;
		m_outline.n_points = 0;
		m_outline.flags = FT_OUTLINE_EVEN_ODD_FILL;
		m_tform = []( float &, float & ){};
	}

	void set_transform( std::function <void( float &, float & )> tform ) {
		m_tform = tform;
	}


	void move_to( float x, float y ) {
		close_contour();
		add_point( x, y );
	}

	void line_to( float x, float y ) {
		add_point( x, y );
	}

	void render() {
		if ( m_bitmap.buffer ) {
			close_contour();
			m_outline.tags		= m_tags.data();
			m_outline.contours	= m_cont.data();
			m_outline.points	= m_pnts.data();
			ft_grays_raster.raster_render( m_ras, &m_params );
		}
	}

private:
	FT_Outline				m_outline;
	FT_Bitmap				m_bitmap;
	FT_Raster_Params		m_params;
	FT_Raster				m_ras;

	std::vector <char>		m_tags;
	std::vector <short>		m_cont;
	std::vector <FT_Vector>	m_pnts;

	uint8_t *				m_pool;

	std::function <void( float &, float & )> m_tform;
};

} // namespace ft2

#endif // __FT2_RASTERIZER_H__
