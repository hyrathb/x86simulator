#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
    unsigned long long ret = (((unsigned long long)a)*b >> 16);
    return ret;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
    unsigned long long aa = (a>0?a:-a);
    unsigned long long bb = (b>0?b:-b);
    FLOAT ret = 0;
    int i = 16;
    aa <<= 16;
    bb <<= 16;
    while (aa)
    {
        if (aa >= bb)
        {
            aa -= bb;
            ret |= 1 << i;
        }
        if (!i)
            break;
        bb >>= 1;
        --i;
    }
    if ((a<0 && b>0) || (a>0 && b<0))
        ret = -ret;
    return ret;
}

FLOAT f2F(float a) {
    struct ieee754 *bytes = (void *)&a;
    if (bytes->exp < 111)
    {
        return 0;
    }
    else if (bytes->exp > 181)
    {
        return 0x7fffffff;
    }
    else
    {
        unsigned int tail = 1 << 23 | (0x7fffff & bytes->tail);
        int exp = bytes->exp - 127;
        unsigned int i, f;
        if (exp <0)
            i=0;
        else if (exp <=23)
            i = tail >> (23 - exp);
        else
            i = tail << (exp - 23);
        i <<= 16;
        if (exp >= -9)
            f = tail << (9 +exp);
        else
            f = tail >> (-exp - 9);
        f >>= 16;
        FLOAT result = (i & 0x7fff0000 ) | (f & 0xffff);
        if (bytes->sign)
            result = -result;
        return result;
    }
}

FLOAT Fabs(FLOAT a) {
    return (a>0?a:-a);
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

