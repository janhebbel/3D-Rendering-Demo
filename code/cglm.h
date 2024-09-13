#ifndef CGLM_H
#define CGLM_H

#include <math.h>

typedef float vec2[2];
typedef float vec3[3];
typedef float vec4[4];

typedef int ivec2[2];
typedef int ivec3[3];
typedef int ivec4[4];

typedef unsigned int uvec2[2];
typedef unsigned int uvec3[3];
typedef unsigned int uvec4[4];

typedef vec3 mat3[3];
typedef vec4 mat4[4];

#define PI 3.14159265359f
#define TABLE_SIZE 257
#define STEP_SIZE 0.25f * 2 * PI / (TABLE_SIZE - 1)

float table[TABLE_SIZE] = {
    0.0000000f, 0.0061359f, 0.0122715f, 0.0184067f, 0.0245412f, 0.0306748f, 
    0.0368072f, 0.0429383f, 0.0490677f, 0.0551952f, 0.0613207f, 0.0674439f, 
    0.0735646f, 0.0796824f, 0.0857973f, 0.0919090f, 0.0980171f, 0.1041216f, 
    0.1102222f, 0.1163186f, 0.1224107f, 0.1284981f, 0.1345807f, 0.1406582f, 
    0.1467305f, 0.1527972f, 0.1588582f, 0.1649131f, 0.1709619f, 0.1770042f, 
    0.1830399f, 0.1890687f, 0.1950903f, 0.2011046f, 0.2071114f, 0.2131103f, 
    0.2191012f, 0.2250839f, 0.2310581f, 0.2370236f, 0.2429802f, 0.2489276f, 
    0.2548656f, 0.2607941f, 0.2667128f, 0.2726214f, 0.2785197f, 0.2844076f, 
    0.2902847f, 0.2961509f, 0.3020059f, 0.3078496f, 0.3136818f, 0.3195020f, 
    0.3253103f, 0.3311063f, 0.3368898f, 0.3426607f, 0.3484187f, 0.3541635f, 
    0.3598951f, 0.3656130f, 0.3713172f, 0.3770074f, 0.3826835f, 0.3883450f, 
    0.3939920f, 0.3996242f, 0.4052413f, 0.4108432f, 0.4164295f, 0.4220003f, 
    0.4275551f, 0.4330938f, 0.4386162f, 0.4441221f, 0.4496113f, 0.4550836f, 
    0.4605387f, 0.4659765f, 0.4713967f, 0.4767992f, 0.4821838f, 0.4875502f, 
    0.4928982f, 0.4982277f, 0.5035384f, 0.5088302f, 0.5141027f, 0.5193560f, 
    0.5245897f, 0.5298036f, 0.5349976f, 0.5401715f, 0.5453250f, 0.5504580f, 
    0.5555702f, 0.5606616f, 0.5657318f, 0.5707808f, 0.5758082f, 0.5808140f, 
    0.5857978f, 0.5907597f, 0.5956993f, 0.6006165f, 0.6055110f, 0.6103828f, 
    0.6152316f, 0.6200572f, 0.6248595f, 0.6296383f, 0.6343933f, 0.6391245f, 
    0.6438316f, 0.6485144f, 0.6531729f, 0.6578067f, 0.6624158f, 0.6669999f, 
    0.6715590f, 0.6760927f, 0.6806010f, 0.6850837f, 0.6895406f, 0.6939715f, 
    0.6983762f, 0.7027547f, 0.7071068f, 0.7114322f, 0.7157308f, 0.7200025f, 
    0.7242470f, 0.7284644f, 0.7326543f, 0.7368166f, 0.7409511f, 0.7450578f, 
    0.7491364f, 0.7531868f, 0.7572088f, 0.7612024f, 0.7651673f, 0.7691033f, 
    0.7730104f, 0.7768885f, 0.7807372f, 0.7845566f, 0.7883464f, 0.7921066f, 
    0.7958369f, 0.7995373f, 0.8032075f, 0.8068475f, 0.8104572f, 0.8140363f, 
    0.8175848f, 0.8211025f, 0.8245893f, 0.8280451f, 0.8314696f, 0.8348629f, 
    0.8382247f, 0.8415549f, 0.8448536f, 0.8481203f, 0.8513552f, 0.8545580f, 
    0.8577286f, 0.8608669f, 0.8639728f, 0.8670462f, 0.8700870f, 0.8730950f, 
    0.8760701f, 0.8790122f, 0.8819212f, 0.8847971f, 0.8876396f, 0.8904487f, 
    0.8932243f, 0.8959663f, 0.8986744f, 0.9013488f, 0.9039893f, 0.9065957f, 
    0.9091680f, 0.9117060f, 0.9142098f, 0.9166790f, 0.9191138f, 0.9215140f, 
    0.9238795f, 0.9262102f, 0.9285061f, 0.9307670f, 0.9329928f, 0.9351835f, 
    0.9373390f, 0.9394592f, 0.9415441f, 0.9435934f, 0.9456074f, 0.9475856f, 
    0.9495282f, 0.9514350f, 0.9533060f, 0.9551412f, 0.9569404f, 0.9587035f, 
    0.9604305f, 0.9621214f, 0.9637761f, 0.9653944f, 0.9669765f, 0.9685221f, 
    0.9700313f, 0.9715039f, 0.9729400f, 0.9743394f, 0.9757021f, 0.9770281f, 
    0.9783174f, 0.9795697f, 0.9807853f, 0.9819639f, 0.9831055f, 0.9842101f, 
    0.9852777f, 0.9863081f, 0.9873014f, 0.9882576f, 0.9891765f, 0.9900582f, 
    0.9909027f, 0.9917098f, 0.9924796f, 0.9932119f, 0.9939070f, 0.9945646f, 
    0.9951847f, 0.9957674f, 0.9963126f, 0.9968203f, 0.9972904f, 0.9977230f, 
    0.9981181f, 0.9984756f, 0.9987954f, 0.9990777f, 0.9993224f, 0.9995294f, 
    0.9996988f, 0.9998306f, 0.9999247f, 0.9999812f, 1.0000000f
};

#define CGLMDEF static inline

//
// trigonometric functions
CGLMDEF float cglm_sin(float turn);
CGLMDEF float cglm_cos(float turn);
CGLMDEF float cglm_tan(float turn);
CGLMDEF float cglm_cot(float turn);

//
// float vector 2
CGLMDEF void cglm_vec2_set(vec2 v1, vec2 v2);
CGLMDEF void cglm_vec2_add(vec2 v1, vec2 v2, vec2 dst);
CGLMDEF void cglm_vec2_sub(vec2 v1, vec2 v2, vec2 dst);
CGLMDEF void cglm_vec2_mul(vec2 v1, vec2 v2, vec2 dst);
CGLMDEF void cglm_vec2_div(vec2 v1, vec2 v2, vec2 dst);
CGLMDEF float cglm_vec2_dot(vec2 v1, vec2 v2);
CGLMDEF float cglm_vec2_len(vec2 v);
CGLMDEF void cglm_vec2_normalize(vec2 v, vec2 dst);
CGLMDEF void cglm_vec2_normalize_this(vec2 v);
CGLMDEF void cglm_vec2_scale(vec2 v, float s, vec2 dst);
CGLMDEF void cglm_vec2_scale_this(vec2 v, float s);

//
// float vector 3
CGLMDEF void cglm_vec3_set(vec3 v1, vec3 v2);
CGLMDEF void cglm_vec3_add(vec3 v1, vec3 v2, vec3 dst);
CGLMDEF void cglm_vec3_sub(vec3 v1, vec3 v2, vec3 dst);
CGLMDEF void cglm_vec3_mul(vec3 v1, vec3 v2, vec3 dst);
CGLMDEF void cglm_vec3_div(vec3 v1, vec3 v2, vec3 dst);
CGLMDEF float cglm_vec3_dot(vec3 v1, vec3 v2);
CGLMDEF float cglm_vec3_len(vec3 v);
CGLMDEF void cglm_vec3_normalize(vec3 v, vec3 dst);
CGLMDEF void cglm_vec3_normalize_this(vec3 v);
CGLMDEF void cglm_vec3_cross(vec3 v1, vec3 v2, vec3 dst);
CGLMDEF void cglm_vec3_scale(vec3 v, float s, vec3 dst);
CGLMDEF void cglm_vec3_scale_this(vec3 v, float s);

//
// float vector 4
CGLMDEF void cglm_vec4_set(vec4 v1, vec4 v2);
CGLMDEF void cglm_vec4_add(vec4 v1, vec4 v2, vec4 dst);
CGLMDEF void cglm_vec4_sub(vec4 v1, vec4 v2, vec4 dst);
CGLMDEF void cglm_vec4_mul(vec4 v1, vec4 v2, vec4 dst);
CGLMDEF void cglm_vec4_div(vec4 v1, vec4 v2, vec4 dst);
CGLMDEF float cglm_vec4_dot(vec4 v1, vec4 v2);
CGLMDEF float cglm_vec4_len(vec4 v);
CGLMDEF void cglm_vec4_normalize(vec4 v, vec4 dst);
CGLMDEF void cglm_vec4_normalize_this(vec4 v);
CGLMDEF void cglm_vec4_scale(vec4 v, float s, vec4 dst);
CGLMDEF void cglm_vec4_scale_this(vec4 v, float s);

// 
// float matrix 4 by 4
CGLMDEF void cglm_mat4_fill(float v, mat4 dst);
CGLMDEF void cglm_mat4_identity(mat4 m);
CGLMDEF void cglm_mat4_transpose(mat4 m, mat4 dst);
CGLMDEF void cglm_mat4_transpose_this(mat4 m);
CGLMDEF void cglm_mat4_add(mat4 m1, mat4 m2, mat4 dst);
CGLMDEF void cglm_mat4_sub(mat4 m1, mat4 m2, mat4 dst);
CGLMDEF void cglm_mat4_mul(mat4 m1, mat4 m2, mat4 dst);
CGLMDEF void cglm_mat4_mul3(mat4 m1, mat4 m2, mat4 m3, mat4 dst);

//
// float matrix vector operations
CGLMDEF void cglm_mat4_vec4_mul(mat4 m, vec4 v, vec4 dst);

//
// transformations
CGLMDEF void cglm_look_at(vec3 position, vec3 target, vec3 fake_up, mat4 dst);
CGLMDEF void cglm_orthographic(float left, float right, float bottom, float top, float znear, float zfar, mat4 dst);
CGLMDEF void cglm_perspective(float fov, float aspect, float znear, float zfar, mat4 dst);
CGLMDEF void cglm_perspective2(float fov, float aspect, float znear, float zfar, mat4 dst);

#ifdef CGLM_IMPLEMENTATION

//
// trigonometric functions
CGLMDEF float cglm_sin(float turn) {
    float normalized_turn = turn - floorf(turn); /* keep turn in range 0..1 */
    
    int mirror = 0;
    int flip = 0;
    
    float index;
    if (normalized_turn >= 0.0f && normalized_turn < 0.25f) {
        index = normalized_turn * 4.0f * (TABLE_SIZE - 1);
    }
    else if (normalized_turn >= 0.25f && normalized_turn < 0.5f) {
        index = (normalized_turn - 0.25f) * 4.0f * (TABLE_SIZE - 1);
        mirror = 1;
    }
    else if (normalized_turn >= 0.5f && normalized_turn < 0.75f) {
        index = (normalized_turn - 0.5f) * 4.0f * (TABLE_SIZE - 1);
        flip = 1;
    }
    else {
        index = (normalized_turn - 0.75f) * 4.0f * (TABLE_SIZE - 1);
        mirror = 1;
        flip = 1;
    }
    if (mirror) {
        index = (TABLE_SIZE - 1) - index;
    }
    int index0 = (int)index;
    int index1 = index0 + 1;
    
    float lerp = table[index0] + (((table[index1] - table[index0]) / STEP_SIZE) *
                                  ((index - index0) * STEP_SIZE));
    
    if (flip) {
        return(-lerp);
    }
    else {
        return(lerp);
    }
}

CGLMDEF float cglm_cos(float turn) {
    float normalized_turn = turn - floorf(turn); /* keep turn in range 0..1 */
    
    int mirror = 0;
    int flip = 0;
    
    float index;
    if (normalized_turn >= 0.0f && normalized_turn < 0.25f) {
        index = normalized_turn * 4.0f * (TABLE_SIZE - 1);
        mirror = 1;
    }
    else if (normalized_turn >= 0.25f && normalized_turn < 0.5f) {
        index = (normalized_turn - 0.25f) * 4.0f * (TABLE_SIZE - 1);
        flip = 1;
    }
    else if (normalized_turn >= 0.5f && normalized_turn < 0.75f) {
        index = (normalized_turn - 0.5f) * 4.0f * (TABLE_SIZE - 1);
        mirror = 1;
        flip = 1;
    }
    else {
        index = (normalized_turn - 0.75f) * 4.0f * (TABLE_SIZE - 1);
    }
    if (mirror) {
        index = (TABLE_SIZE - 1) - index;
    }
    int index0 = (int)index;
    int index1 = index0 + 1;
    
    float lerp = table[index0] + (((table[index1] - table[index0]) / STEP_SIZE) *
                                  ((index - index0) * STEP_SIZE));
    
    if (flip) {
        return(-lerp);
    }
    else {
        return(lerp);
    }    
}

CGLMDEF float cglm_tan(float turn) {
    return(cglm_sin(turn) / cglm_cos(turn));
}

CGLMDEF float cglm_cot(float turn) {
    return(cglm_cos(turn) / cglm_sin(turn));
}

//
// float vector 2
CGLMDEF void cglm_vec2_set(vec2 v1, vec2 v2) {
	v1[0] = v2[0];
	v1[1] = v2[1];
}

CGLMDEF void cglm_vec2_add(vec2 v1, vec2 v2, vec2 dst) {
    dst[0] = v1[0] + v2[0];
    dst[1] = v1[1] + v2[1];
}

CGLMDEF void cglm_vec2_sub(vec2 v1, vec2 v2, vec2 dst) {
    dst[0] = v1[0] - v2[0];
    dst[1] = v1[1] - v2[1];
}

CGLMDEF void cglm_vec2_mul(vec2 v1, vec2 v2, vec2 dst) {
    dst[0] = v1[0] * v2[0];
    dst[1] = v1[1] * v2[1];
}

CGLMDEF void cglm_vec2_div(vec2 v1, vec2 v2, vec2 dst) {
    dst[0] = v1[0] / v2[0];
    dst[1] = v1[1] / v2[1];
}

CGLMDEF float cglm_vec2_dot(vec2 v1, vec2 v2) {
    return(v1[0] * v2[0] + v1[1] * v2[1]);
}

CGLMDEF float cglm_vec2_len(vec2 v) {
    return(sqrtf(cglm_vec2_dot(v, v)));
}

CGLMDEF void cglm_vec2_normalize(vec2 v, vec2 dst) {
    float len = cglm_vec2_len(v);
    dst[0] = v[0] / len;
    dst[1] = v[1] / len;
    dst[2] = v[2] / len;
}

CGLMDEF void cglm_vec2_normalize_this(vec2 v) {
    cglm_vec2_normalize(v, v);
}

CGLMDEF void cglm_vec2_scale(vec2 v, float s, vec2 dst) {
    dst[0] = v[0] * s;
    dst[1] = v[1] * s;
}

CGLMDEF void  cglm_vec2_scale_this(vec2 v, float s) {
    cglm_vec2_scale(v, s, v);
}

//
// float vector 3
CGLMDEF void cglm_vec3_set(vec3 v1, vec3 v2) {
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

CGLMDEF void cglm_vec3_add(vec3 v1, vec3 v2, vec3 dst) {
    dst[0] = v1[0] + v2[0];
    dst[1] = v1[1] + v2[1];
    dst[2] = v1[2] + v2[2];
}

CGLMDEF void cglm_vec3_sub(vec3 v1, vec3 v2, vec3 dst) {
    dst[0] = v1[0] - v2[0];
    dst[1] = v1[1] - v2[1];
    dst[2] = v1[2] - v2[2];
}

CGLMDEF void cglm_vec3_mul(vec3 v1, vec3 v2, vec3 dst) {
    dst[0] = v1[0] * v2[0];
    dst[1] = v1[1] * v2[1];
    dst[2] = v1[2] * v2[2];
}

CGLMDEF void cglm_vec3_div(vec3 v1, vec3 v2, vec3 dst) {
    dst[0] = v1[0] / v2[0];
    dst[1] = v1[1] / v2[1];
    dst[2] = v1[2] / v2[2];
}

CGLMDEF float cglm_vec3_dot(vec3 v1, vec3 v2) {
    return(v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2]);
}

CGLMDEF float cglm_vec3_len(vec3 v) {
    return(sqrtf(cglm_vec3_dot(v, v)));
}

CGLMDEF void cglm_vec3_normalize(vec3 v, vec3 dst) {
    float len = cglm_vec3_len(v);
    dst[0] = v[0] / len;
    dst[1] = v[1] / len;
    dst[2] = v[2] / len;
}

CGLMDEF void cglm_vec3_normalize_this(vec3 v) {
    cglm_vec3_normalize(v, v);
}

CGLMDEF void cglm_vec3_cross(vec3 v1, vec3 v2, vec3 dst) {
    dst[0] = v1[1] * v2[2] - v1[2] * v2[1];
    dst[1] = v1[2] * v2[0] - v1[0] * v2[2];
    dst[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

CGLMDEF void cglm_vec3_scale(vec3 v, float s, vec3 dst) {
    dst[0] = v[0] * s;
    dst[1] = v[1] * s;
    dst[2] = v[2] * s;
}

CGLMDEF void cglm_vec3_scale_this(vec3 v, float s) {
    cglm_vec3_scale(v, s, v);
}

//
// float vector 4
CGLMDEF void cglm_vec4_set(vec4 v1, vec4 v2) {
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
	v1[3] = v2[3];
}

CGLMDEF void cglm_vec4_add(vec4 v1, vec4 v2, vec4 dst) {
    dst[0] = v1[0] + v2[0];
    dst[1] = v1[1] + v2[1];
    dst[2] = v1[2] + v2[2];
    dst[3] = v1[3] + v2[3];
}

CGLMDEF void cglm_vec4_sub(vec4 v1, vec4 v2, vec4 dst) {
    dst[0] = v1[0] - v2[0];
    dst[1] = v1[1] - v2[1];
    dst[2] = v1[2] - v2[2];
    dst[3] = v1[3] - v2[3];
}

CGLMDEF void cglm_vec4_mul(vec4 v1, vec4 v2, vec4 dst) {
    dst[0] = v1[0] * v2[0];
    dst[1] = v1[1] * v2[1];
    dst[2] = v1[2] * v2[2];
    dst[3] = v1[3] * v2[3];
}

CGLMDEF void cglm_vec4_div(vec4 v1, vec4 v2, vec4 dst) {
    dst[0] = v1[0] / v2[0];
    dst[1] = v1[1] / v2[1];
    dst[2] = v1[2] / v2[2];
    dst[3] = v1[3] / v2[3];
}

CGLMDEF float cglm_vec4_dot(vec4 v1, vec4 v2) {
    return(v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3]);
}

CGLMDEF float cglm_vec4_len(vec4 v) {
    return(sqrtf(cglm_vec4_dot(v, v)));
}

CGLMDEF void cglm_vec4_normalize(vec4 v, vec4 dst) {
    float len = cglm_vec4_len(v);
    dst[0] = v[0] / len;
    dst[1] = v[1] / len;
    dst[2] = v[2] / len;
}

CGLMDEF void cglm_vec4_normalize_this(vec4 v) {
    cglm_vec4_normalize(v, v);
}

CGLMDEF void cglm_vec4_scale(vec4 v, float s, vec4 dst) {
    dst[0] = v[0] * s;
    dst[1] = v[1] * s;
    dst[2] = v[2] * s;
    dst[3] = v[3] * s;
}

CGLMDEF void cglm_vec4_scale_this(vec4 v, float s) {
    cglm_vec4_scale(v, s, v);
}

//
// float matrix 4 by 4
CGLMDEF void cglm_mat4_fill(float v, mat4 dst) {
    float *p = (float *)dst;
    for(int i = 0; i < 16; ++i) {
        p[i] = v;
    }
}

CGLMDEF void cglm_mat4_identity(mat4 m) {
    m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f;
    m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = 0.0f;
    m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = 0.0f;
    m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
}

CGLMDEF void cglm_mat4_transpose(mat4 m, mat4 dst) {
    for(int i = 0; i < 4; ++i) {
        for(int j = 0; j < 4; ++j) {
            dst[j][i] = m[i][j];
        }
    }
}

CGLMDEF void cglm_mat4_transpose_this(mat4 m) {
    for(int i = 0; i < 4; ++i) {
        for(int j = i + 1; j < 4; ++j) {
            float f = m[i][j];
            m[i][j] = m[j][i];
            m[j][i] = f;
        }
    }
}

CGLMDEF void cglm_mat4_add(mat4 m1, mat4 m2, mat4 dst) {
    // TODO: what is faster?
    /*
    dst = (mat4)
    {
        {m1[0][0] + m2[0][0], m1[0][1] + m2[0][1], m1[0][2] + m2[0][2], m1[0][3] + m2[0][3]},
        {m1[1][0] + m2[1][0], m1[1][1] + m2[1][1], m1[1][2] + m2[1][2], m1[1][3] + m2[1][3]},
        {m1[2][0] + m2[2][0], m1[2][1] + m2[2][1], m1[2][2] + m2[2][2], m1[2][3] + m2[2][3]},
        {m1[3][0] + m2[3][0], m1[3][1] + m2[3][1], m1[3][2] + m2[3][2], m1[3][3] + m2[3][3]}
    };
    */
    for(int row = 0; row < 4; ++row) {
        for(int column = 0; column < 4; ++column) {
            dst[row][column] = m1[row][column] + m2[row][column];
        }
    }
}


CGLMDEF void cglm_mat4_sub(mat4 m1, mat4 m2, mat4 dst) {
    for(int row = 0; row < 4; ++row) {
        for(int column = 0; column < 4; ++column) {
            dst[row][column] = m1[row][column] - m2[row][column];
        }
    }
}

CGLMDEF void  cglm_mat4_mul(mat4 m1, mat4 m2, mat4 dst) {
    for (int i = 0; i < 4; ++i) { /* row of m1 */
        for (int j = 0; j < 4; ++j) { /* column of m2 */
            dst[j][i] = 0.0f;
            for (int k = 0; k < 4; ++k) {
                dst[j][i] += m1[k][i] * m2[j][k];
            }
        }
    }
}

CGLMDEF void cglm_mat4_mul3(mat4 m1, mat4 m2, mat4 m3, mat4 dst) {
    mat4 tmp;
    cglm_mat4_mul(m3, m2, tmp);
    cglm_mat4_mul(tmp, m1, dst);
}

//
// float matrix vector operations
CGLMDEF void cglm_mat4_vec4_mul(mat4 m, vec4 v, vec4 dst) {
    for (int i = 0; i < 4; ++i) { // row of m
        dst[i] = 0.0f;
        for (int k = 0; k < 4; ++k) {
            dst[i] += m[k][i] * v[k];
        }
    }
}

//
// transformations
CGLMDEF void cglm_look_at(vec3 position, vec3 target, vec3 fake_up, mat4 dst) {
    vec3 backward, right, up;

    cglm_vec3_sub(position, target, backward);
    cglm_vec3_cross(fake_up, backward, right);
    cglm_vec3_cross(backward, right, up);

    cglm_vec3_normalize_this(backward);
    cglm_vec3_normalize_this(right);
    cglm_vec3_normalize_this(up);

    float cos1, cos2, cos3;

    cos1 = cglm_vec3_dot(right, position);
    cos2 = cglm_vec3_dot(up, position);
    cos3 = cglm_vec3_dot(backward, position);

    dst[0][0] = right[0], dst[0][1] = up[0], dst[0][2] = backward[0], dst[0][3] = 0.0f;
    dst[1][0] = right[1], dst[1][1] = up[1], dst[1][2] = backward[1], dst[1][3] = 0.0f;
    dst[2][0] = right[2], dst[2][1] = up[2], dst[2][2] = backward[2], dst[2][3] = 0.0f;
    dst[3][0] = -cos1,    dst[3][1] = -cos2, dst[3][2] = -cos3,       dst[3][3] = 1.0f;
}

CGLMDEF void cglm_orthographic(float left, float right, float bottom, float top, float znear, float zfar, mat4 dst) {
    float width = right - left;
    float height = top - bottom;
    float depth = zfar - znear;

    float m30 = (right + left) / width;
    float m31 = (top + bottom) / height;
    float m32 = (zfar + znear) / depth;

    dst[0][0] = 2.0f / width,  dst[0][1] = 0.0f,          dst[0][2] = 0.0f,          dst[0][3] = 0.0f;
    dst[1][0] = 0.0f,          dst[1][1] = 2.0f / height, dst[1][2] = 0.0f,          dst[1][3] = 0.0f;
    dst[2][0] = 0.0f,          dst[2][1] = 0.0f,          dst[2][2] = -2.0f / depth, dst[2][3] = 0.0f;
    dst[3][0] = -m30,          dst[3][1] = -m31,          dst[3][2] = -m32,          dst[3][3] = 1.0f;
}

CGLMDEF void cglm_perspective(float fov, float aspect, float znear, float zfar, mat4 dst) {
    float f = cglm_cot(0.5f * fov);
    float inv_depth = 1.0f / (znear - zfar);

    dst[0][0] = f * aspect, dst[0][1] = 0.0f, dst[0][2] = 0.0f,                         dst[0][3] =  0.0f;
    dst[1][0] = 0.0f,       dst[1][1] = f,    dst[1][2] = 0.0f,                         dst[1][3] =  0.0f;
    dst[2][0] = 0.0f,       dst[2][1] = 0.0f, dst[2][2] = (zfar + znear) * inv_depth,   dst[2][3] = -1.0f;
    dst[3][0] = 0.0f,       dst[3][1] = 0.0f, dst[3][2] = 2 * zfar * znear * inv_depth, dst[3][3] =  0.0f;
}

CGLMDEF void cglm_perspective2(float fov, float aspect, float znear, float zfar, mat4 dst) {
    float right = znear * cglm_tan(fov * 0.5f);
    float top = right * aspect;
    float inv_d = 1.0f / (zfar - znear);

    dst[0][0] = znear / right, dst[0][1] = 0.0f,        dst[0][2] = 0.0f,                      dst[0][3] =  0.0f;
    dst[1][0] = 0.0f,          dst[1][1] = znear / top, dst[1][2] = 0.0f,                      dst[1][3] =  0.0f;
    dst[2][0] = 0.0f,          dst[2][1] = 0.0f,        dst[2][2] = -(zfar + znear) * inv_d,   dst[2][3] = -1.0f;
    dst[3][0] = 0.0f,          dst[3][1] = 0.0f,        dst[3][2] = -2 * zfar * znear * inv_d, dst[3][3] =  0.0f;
}

#endif // CGLM_IMPLEMENTATION

#endif // CLM_H
