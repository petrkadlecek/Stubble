#include "RandomGenerator.hpp"
#include "Common\CommonFunctions.hpp"

namespace Stubble
{

namespace HairShape
{

void RandomGenerator::reset( int aIJ, int aKL )
{
	double s, t;
	int i, j, k, l, m;
	int ii, jj;

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

double RandomGenerator::uniformNumber()
{
	double uni = u[i97] - u[j97];
	
	if ( uni < 0.0 )
		uni += 1.0;

	u[i97] = uni;
	if( --i97 < 0 )
		i97 = 96;

	if( --j97 < 0 )
		j97 = 96;

	if( (c -= cd) < 0.0 )
		c += cm;

	if( (uni -= c) < 0.0 )
		uni += 1.0;

	return uni;
}

} // namespace HairShape

} // namespace Stubble