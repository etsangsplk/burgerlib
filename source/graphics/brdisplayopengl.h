/***************************************

	OpenGL manager class

	Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!
	
***************************************/

#ifndef __BRDISPLAYOPENGL_H__
#define __BRDISPLAYOPENGL_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRDISPLAY_H__
#include "brdisplay.h"
#endif

#if defined(BURGER_WINDOWS) && !defined(__BRWINDOWSTYPES_H__)
#include "brwindowstypes.h"
#endif

#if defined(BURGER_MACOSX) && !defined(__BRMACOSXTYPES_H__)
#include "brmacosxtypes.h"
#endif

#if defined(BURGER_SHIELD) && !defined(__BRSHIELDTYPES_H__)
#include "brshieldtypes.h"
#endif

/* BEGIN */
namespace Burger {
class Image;
#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
class DisplayOpenGL : public Display {
	BURGER_RTTI_IN_CLASS();
protected:
	Word *m_pCompressedFormats;		///< Pointer to an array of supported OpenGL compressed textures
	HDC__ *m_pOpenGLDeviceContext;	///< (Windows only) Window to attach the GL context to
	HGLRC__ *m_pOpenGLContext;		///< (Windows only) Current OpenGL context
	Word m_bResolutionChanged;		///< (Windows only) Set to \ref TRUE if the display needs to be restored
	float m_fOpenGLVersion;					///< Numeric value for the version of OpenGL
	float m_fShadingLanguageVersion;		///< Numeric value for the version of the Shader compiler
	Word m_uCompressedFormatCount;			///< Number of supported compressed texture formats
	Word m_uMaximumVertexAttributes;		///< Maximum number of vertex attributes GL_MAX_VERTEX_ATTRIBS
	Word m_uMaximumColorAttachments;		///< Maximum number of color frame attachments GL_MAX_COLOR_ATTACHMENTS
	Word m_uActiveTexture;					///< OpenGL glActiveTexture() shadow value
public:
	DisplayOpenGL(GameApp *pGameApp);
	Word Init(Word uWidth,Word uHeight,Word uDepth=32,Word uFlags=DEFAULTFLAGS) BURGER_OVERRIDE;
	void Shutdown(void) BURGER_OVERRIDE;
	void BeginScene(void) BURGER_OVERRIDE;
	void EndScene(void) BURGER_OVERRIDE;
	Texture *CreateTextureObject(void) BURGER_OVERRIDE;
	VertexBuffer *CreateVertexBufferObject(void) BURGER_OVERRIDE;
	void Resize(Word uWidth,Word uHeight) BURGER_OVERRIDE;
	void SetViewport(Word uX,Word uY,Word uWidth,Word uHeight) BURGER_OVERRIDE;
	void SetScissorRect(Word uX,Word uY,Word uWidth,Word uHeight) BURGER_OVERRIDE;
	void SetClearColor(float fRed,float fGreen,float fBlue,float fAlpha) BURGER_OVERRIDE;
	void SetClearDepth(float fDepth) BURGER_OVERRIDE;
	void Clear(Word uMask) BURGER_OVERRIDE;
	void Bind(Texture *pTexture,Word uIndex=0) BURGER_OVERRIDE;
	void Bind(Effect *pEffect) BURGER_OVERRIDE;
	void SetBlend(Word bEnable) BURGER_OVERRIDE;
	void SetBlendFunction(eSourceBlendFactor uSourceFactor,eDestinationBlendFactor uDestFactor) BURGER_OVERRIDE;
	void SetLighting(Word bEnable) BURGER_OVERRIDE;
	void SetZWrite(Word bEnable) BURGER_OVERRIDE;
	void SetDepthTest(eDepthFunction uDepthFunction) BURGER_OVERRIDE;
	void SetCullMode(eCullMode uCullMode) BURGER_OVERRIDE;
	void SetScissor(Word bEnable) BURGER_OVERRIDE;
    void DrawPrimitive(ePrimitiveType uPrimitiveType, VertexBuffer* pVertexBuffer) BURGER_OVERRIDE;
	void DrawElements(ePrimitiveType uPrimitiveType,VertexBuffer *pVertexBuffer) BURGER_OVERRIDE;

	BURGER_INLINE float GetOpenGLVersion(void) const { return m_fOpenGLVersion; }
	BURGER_INLINE float GetShadingLanguageVersion(void) const { return m_fShadingLanguageVersion; }
	BURGER_INLINE Word GetCompressedFormatCount(void) const { return m_uCompressedFormatCount; }
	BURGER_INLINE const Word *GetCompressedFormats(void) const { return m_pCompressedFormats; }
	BURGER_INLINE Word GetMaximumVertexAttributes(void) const { return m_uMaximumVertexAttributes; }
	void BURGER_API SetupOpenGL(void);
	Word BURGER_API CompileShader(Word GLEnum,const char *pShaderCode,WordPtr uShaderCodeLength=0) const;
	Word BURGER_API CompileProgram(const char *pUnifiedShader,WordPtr uLength,const OpenGLVertexInputs_t *pVertexInputs=NULL,const Word *pMembers=NULL) const;
	Word BURGER_API CompileProgram(const char *pVertexShader,WordPtr uVSLength,const char *pPixelShader,WordPtr uPSLength,const OpenGLVertexInputs_t *pVertexInputs=NULL,const Word *pMembers=NULL) const;
	Word BURGER_API CreateVertexArrayObject(const OpenGLVertexBufferObjectDescription_t *pDescription) const;
	void BURGER_API DeleteVertexArrayObject(Word uVertexArrayObject) const;
	Word BURGER_API BuildFrameBufferObject(Word uWidth,Word uHeight,Word uGLDepth,Word uGLClamp,Word uGLZDepth=0) const;
	static void BURGER_API DeleteFrameBufferObjectAttachment(Word uAttachment);
	void BURGER_API DeleteFrameBufferObject(Word uFrameBufferObject) const;
	static Word BURGER_API CreateTextureID(const Image *pImage,Word bGenerateMipMap=FALSE);
	static const char * BURGER_API GetErrorString(Word uGLErrorEnum);
	static WordPtr BURGER_API GetGLTypeSize(Word uGLTypeEnum);
	static Word BURGER_API PrintGLError(const char *pErrorLocation);
	static void BURGER_API WindowsLink(void);
	static void BURGER_API WindowsUnlink(void);
    static BURGER_INLINE Word GetFrontBuffer(void) { return 0; }

};
#endif
}
/* END */

#endif
