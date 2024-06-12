//
// Useful Macros
#define ARRAYCOUNT(x) sizeof(x) / sizeof(x[0])

#define ZEROSTRUCT(x) memset(&(x), 0, sizeof(x))

#define MINIMUM(x, y) ((x) < (y)) ? (x) : (y)
#define MAXIMUM(x, y) ((x) > (y)) ? (x) : (y)
#define SIGN(x) ((x > 0) - (x < 0))
#define ABS(value) ((value < 0) ? -(value) : (value))
#define CLAMP(x, max, min) MAXIMUM(MINIMUM(x, max), min)

#define KILOBYTES(x) ((x) << 10)
#define MEGABYTES(x) ((x) << 20)
#define GIGABYTES(x) ((x) << 30)

//
// Useful Functions
int is_end_of_line(char c)
{
    return((c == '\n') || (c == '\r'));
}

int is_spacing(char c)
{
    return((c == ' ') || (c == '\t') || (c == '\v') || (c == '\f'));
}

int is_whitespace(char c)
{
    return(is_spacing(c) || is_end_of_line(c));
}

int strings_are_equal(char *a, char *b)
{
    int result = (a == b);
    
    if(a && b)
    {
        while(*a && *b && (*a == *b))
        {
            ++a;
            ++b;
        }
        
        result = ((*a == 0) && (*b == 0));
    }
    
    return(result);
}

int strings_are_equal_1l(int a_length, char *a, char *b)
{
    int result = false;
    
    if(b)
    {
        char *at = b;
        for(uintptr_t i = 0; i < a_length; ++i, ++at)
        {
            if(b[i] == '\0' || (a[i] != b[i]))
            {
                return(false);
            }
        }
        
        result = (*at == 0);
    }
    else
    {
        result = (a_length == 0);
    }
    
    return(result);
}

int strings_are_equal_2l(int a_length, char *a, int b_length, char *b)
{
    int result = (a_length == b_length);
    
    if(result)
    {
        result = true;
        for(int i = 0; i < a_length; ++i)
        {
            if(a[i] != b[i])
            {
                result = false;
                break;
            }
        }
    }
    
    return(result);
}

int round_float(float val)
{
    // if val is positive 1 - 0 = +1,
    // if val is negative 0 - 1 = -1.
    int   sign = (val > 0) - (val < 0);
    float half = 0.5;

    return((int)(val + half * sign));
}
