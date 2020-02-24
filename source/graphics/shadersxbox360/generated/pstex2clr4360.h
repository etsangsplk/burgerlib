#if 0
//
// Generated by 2.0.21256.0
//
//   fxc ..\source\graphics\shadersxbox360\pstex2clr4360.x360sl /nologo
//    /FhD:\projects\burger\libs\burgerlib\source\graphics\shadersxbox360\Generated\pstex2clr4360.h
//    /XFdD:\projects\burger\libs\burgerlib\projects\temp\unittestsv10x36rel\pstex2clr4360.updb
//    /Vng_pstex2clr4360 /Tps_2_0
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

// Shader type: pixel 

xps_3_0
defconst DiffuseTexture, sampler2d, object, [1, 1], s0
config AutoSerialize=false
config AutoResource=false
config PsMaxReg=1
// PsExportColorCount=1
// PsSampleControl=both

dcl_texcoord r0.xy
dcl_color_centroid r1


    exec
    tfetch2D r0, r0.xy, tf0
    alloc colors
    exece
    mul oC0, r0, r1

// PDB hint 00000000-00000000-00000000

#endif

// This microcode is in native DWORD byte order.

const DWORD g_pstex2clr4360[] =
{
    0x102a1100, 0x000000b4, 0x0000003c, 0x00000000, 0x00000024, 0x00000000, 
    0x0000008c, 0x00000000, 0x00000000, 0x00000064, 0x0000001c, 0x00000057, 
    0xffff0300, 0x00000001, 0x0000001c, 0x00000000, 0x00000050, 0x00000030, 
    0x00030000, 0x00010000, 0x00000040, 0x00000000, 0x44696666, 0x75736554, 
    0x65787475, 0x726500ab, 0x0004000c, 0x00010001, 0x00010000, 0x00000000, 
    0x70735f33, 0x5f300032, 0x2e302e32, 0x31323536, 0x2e3000ab, 0x00000000, 
    0x0000003c, 0x10000100, 0x00000008, 0x00000000, 0x00001842, 0x00010003, 
    0x00000001, 0x00003050, 0x0000f1a0, 0x00011002, 0x00001200, 0xc4000000, 
    0x00001003, 0x00002200, 0x00000000, 0x10080001, 0x1f1ff688, 0x00004000, 
    0xc80f8000, 0x00000000, 0xe1000100, 0x00000000, 0x00000000, 0x00000000
};
