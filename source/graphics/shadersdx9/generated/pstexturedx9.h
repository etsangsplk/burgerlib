#if 0
//
// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
//
//   fxc ..\source\graphics\shadersdx9\pstexturedx9.hlsl /nologo /O3
//    /FhD:\projects\burger\libs\burgerlib\source\graphics\shadersdx9\Generated\pstexturedx9.h
//    /Vng_pstexturedx9 /Tps_2_0
//
//
// Parameters:
//
//   sampler2D DiffuseTexture;
//
//
// Registers:
//
//   Name           Reg   Size
//   -------------- ----- ----
//   DiffuseTexture s0       1
//

    ps_2_0
    dcl t0.xy
    dcl_2d s0
    texld r0, t0, s0
    mov oC0, r0

// approximately 2 instruction slots used (1 texture, 1 arithmetic)
#endif

const BYTE g_pstexturedx9[] =
{
      0,   2, 255, 255, 254, 255, 
     35,   0,  67,  84,  65,  66, 
     28,   0,   0,   0,  87,   0, 
      0,   0,   0,   2, 255, 255, 
      1,   0,   0,   0,  28,   0, 
      0,   0,   0, 129,   0,   0, 
     80,   0,   0,   0,  48,   0, 
      0,   0,   3,   0,   0,   0, 
      1,   0,   2,   0,  64,   0, 
      0,   0,   0,   0,   0,   0, 
     68, 105, 102, 102, 117, 115, 
    101,  84, 101, 120, 116, 117, 
    114, 101,   0, 171,   4,   0, 
     12,   0,   1,   0,   1,   0, 
      1,   0,   0,   0,   0,   0, 
      0,   0, 112, 115,  95,  50, 
     95,  48,   0,  77, 105,  99, 
    114, 111, 115, 111, 102, 116, 
     32,  40,  82,  41,  32,  72, 
     76,  83,  76,  32,  83, 104, 
     97, 100, 101, 114,  32,  67, 
    111, 109, 112, 105, 108, 101, 
    114,  32,  57,  46,  50,  57, 
     46,  57,  53,  50,  46,  51, 
     49,  49,  49,   0,  31,   0, 
      0,   2,   0,   0,   0, 128, 
      0,   0,   3, 176,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      0,   8,  15, 160,  66,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 176,   0,   8, 
    228, 160,   1,   0,   0,   2, 
      0,   8,  15, 128,   0,   0, 
    228, 128, 255, 255,   0,   0
};
