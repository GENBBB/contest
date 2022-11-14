int
satsum(int v1, int v2)
{
    enum { MY_INT_MAX = ~(unsigned int) 0 >> !(unsigned int) 0 };
    enum { MY_INT_MIN = -MY_INT_MAX - !(unsigned int) 0 };
    if (v1 > 0 && v2 > MY_INT_MAX - v1) {
        return  MY_INT_MAX;
    } else if (v1 < 0 && v2 < MY_INT_MIN - v1) {
        return  MY_INT_MIN;
    }
    return v1 + v2;
}