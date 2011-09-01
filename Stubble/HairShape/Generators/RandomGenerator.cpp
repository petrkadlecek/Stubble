#include "RandomGenerator.hpp"
#include "Common\CommonFunctions.hpp"

namespace Stubble
{

namespace HairShape
{

void RandomGenerator::reset( __int32 aIJ, __int32 aKL )
{
	Real s, t;
	__int32 i, j, k, l, m;
	__int32 ii, jj;

	aIJ = clamp( aIJ, 0, 31328 );
	aKL = clamp( aKL, 0, 30081 );

	i = ((aIJ / 177) % 177) + 2;
	j =  (aIJ % 177) + 2;
	k = ((aKL / 169) % 178) + 1;
	l =  (aKL % 169);

	for ( ii = 0; ii < 97; ii++ )
	{
		s = 0.0;
		t = 0.5;

		for ( jj = 0; jj++ < 24; )
		{
			m = ( (i*j % 179) * k ) % 179;
			i = j;
			j = k;
			k = m;
			l = (53 * l + 1) % 169;

			if ( l * m % 64 >= 32 )
				s += t;

			t *= 0.5;
		}

		u[ii] = s;
    }

	c  =   362436.0 / 16777216.0;
	cd =  7654321.0 / 16777216.0;
	cm = 16777213.0 / 16777216.0;
	
	i97 = 96;
	j97 = 32;
}

} // namespace HairShape

} // namespace Stubble