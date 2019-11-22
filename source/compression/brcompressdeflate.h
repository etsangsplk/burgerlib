/***************************************

	Compression manager

	Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

	It is released under an MIT Open Source license. Please see LICENSE
	for license details. Yes, you can use it in a
	commercial title without paying anything, just give me a credit.
	Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRCOMPRESSDEFLATE_H__
#define __BRCOMPRESSDEFLATE_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRBASE_H__
#include "brbase.h"
#endif

#ifndef __BROUTPUTMEMORYSTREAM_H__
#include "broutputmemorystream.h"
#endif

#ifndef __BRCOMPRESS_H__
#include "brcompress.h"
#endif

#ifndef __BRMEMORYFUNCTIONS_H__
#include "brmemoryfunctions.h"
#endif

/* BEGIN */
namespace Burger {
class CompressDeflate : public Compress {
	BURGER_RTTI_IN_CLASS();
protected:
	enum eState {
		DEFAULT_STATE=0,		///< Dormant state
		INIT_STATE=42,			///< Initialization state
		BUSY_STATE=113,			///< Busy state
		FINISH_STATE=666		///< Finish state
	};

	enum eBlockState {
		STATE_NEEDMORE,      ///< Block not completed, need more input or more output
		STATE_BLOCKDONE,     ///< Block flush performed
		STATE_FINISHSTARTED, ///< Finish started, need only more output at next deflate
		STATE_FINISHDONE     ///< Finish done, accept no more input or output
	};

	enum eDataType {
		Z_BINARY=0,			///< Compress as binary data
		Z_ASCII=1,			///< Compress as ASCII text (Focus on 32-127)
		Z_UNKNOWN=2			///< Unknown data
	};

	enum eStreamTreeType {
		STORED_BLOCK=0,		///< Uncompressed data
		STATIC_TREES=1,		///< Compressed with the static tree
		DYN_TREES=2			///< Compressed with a dynamic tree
	};

	enum {
		MAX_WBITS=15,			///< Number of bits deep a huffman entry can be
		MAX_MEM_LEVEL=9,		///< Memory level
		Z_BEST_COMPRESSION=9,	///< Compression level
		Z_DEFLATED=8,			///< ZLIB token for Deflate
		SMALLEST=1,				///< Index within the heap array of least frequent node in the Huffman tree
		MIN_MATCH=3,			///< Minimum number of bytes of data to compress in a packet
		MAX_MATCH=258,			///< Maximum number of bytes of data to compress in a packet
		DIST_CODE_LEN=512,		///< Length of a distance code
		TOO_FAR=4096,			///< Matches of length 3 are discarded if their distance exceeds TOO_FAR
		MIN_LOOKAHEAD=(MAX_MATCH+MIN_MATCH+1),	///< Minimum amount of lookahead, except at the end of the input file.
		PRESET_DICT=0x20,		///< Preset dictionary flag in zlib header
		LENGTH_CODES=29,		///< Number of length codes, not counting the special END_BLOCK code
		LITERALS=256,			///< Number of literal bytes 0..255
		L_CODES=(LITERALS+1+LENGTH_CODES),	///< Number of Literal or Length codes, including the END_BLOCK code
		D_CODES=30,				///< Number of distance codes
		BL_CODES=19,			///< Number of codes used to transfer the bit lengths
		HEAP_SIZE=(2*L_CODES+1),	///< Maximum heap size
		MAX_BITS=15,			///< All codes must not exceed MAX_BITS bits
		MAX_BL_BITS=7,			///< Bit length codes must not exceed MAX_BL_BITS bits
		END_BLOCK=256,			///< End of block literal code
		REP_3_6=16,				///< Repeat previous bit length 3-6 times (2 bits of repeat count)
		REPZ_3_10=17,			///< Repeat a zero length 3-10 times  (3 bits of repeat count)
		REPZ_11_138=18			///< Repeat a zero length 11-138 times  (7 bits of repeat count)
	};

	struct CodeData_t {
		union {
			Word16 m_uFrequency;	///< Frequency count
			Word16 m_uCode;			///< Bit string
		} m_FrequencyCount;			///< Frequency count/bit string entry
		union {
			Word16 m_uDadBits;		///< Father node in Huffman tree
			Word16 m_uLength;		///< Length of bit string
		} m_DataLength;				///< Data value for the node
	};

	struct StaticTreeDesc_t {
		const CodeData_t *m_pTree;	///< Static tree or \ref NULL
		const int *m_pExtraBits;	///< Extra bits for each code or \ref NULL
		int m_iExtraBase;			///< Base index for extra_bits
		int m_iElements;			///< Max number of elements in the tree
		int m_iMaxLength;			///< Max bit length for the codes
	};

	struct TreeDesc_t {
		CodeData_t *m_pDynamicTree;			///< The dynamic tree
		const StaticTreeDesc_t *m_pStaticTree;	///< The corresponding static tree
		int m_iMaximumCode;					///< Largest code with non zero frequency
	};

	static const Word c_uBufSize = static_cast<Word>(8 * 2*sizeof(Word8));	///< Number of bits used within bi_buf. (bi_buf might be implemented on more than 16 bits on some systems.)
	static const Word c_uWBits = MAX_WBITS;				///< log2(c_uWSize) (8..16)
	static const Word c_uWSize = 1 << c_uWBits;			///< LZ77 window size (32K by default)
	static const Word c_uWMask = c_uWSize - 1;			///< c_uWSize - 1  Use a faster search when the previous match is longer than this
	static const Word c_uHashBits = MAX_MEM_LEVEL + 7;	///< log2(hash_size)
	static const Word c_uHashSize = 1 << c_uHashBits;	///< number of elements in hash table
	static const Word c_uHashMask = c_uHashSize - 1;	///< hash_size-1
	static const Word c_uHashShift = ((c_uHashBits+MIN_MATCH-1)/MIN_MATCH);		///< Number of bits by which m_uInsertHash must be shifted at each input step. It must be such that after MIN_MATCH steps, the oldest byte no longer takes part in the hash key, that is: hash_shift * MIN_MATCH >= hash_bits
	static const Word c_uLiteralBufferSize = 1 << (MAX_MEM_LEVEL + 6);	///< 16K elements by default
	static const Word c_uWindowSize = 2*c_uWSize;		///< Actual size of window: 2*wSize, except when the user input buffer is directly used as sliding window.
	static const Word c_uMaxLazyMatch = 258;	///< Attempt to find a better match only when the current match is strictly smaller than this value. This mechanism is used only for compression levels >= 4.
	static const Word c_uGoodMatch = 32;
	static const int c_iNiceMatch = 258;		///< Stop searching when current match exceeds this
	static const Word c_uMaxChainLength = 4096;	///< To speed up deflation, hash chains are never searched beyond this length.  A higher limit improves compression ratio but degrades the speed.

	const Word8 *m_pInput;		///< Next input byte
	Word8 *m_pPendingOutput;	///< Next pending byte to output to the stream
	WordPtr m_uInputLength;		///< Number of bytes available at next_in
	IntPtr m_iBlockStart;		///< Window position at the beginning of the current output block. Gets negative when the window is moved backwards.
	Word32 m_uAdler;			///< Adler32 value of the uncompressed data
	Word32 m_uOptimalLength;	///< bit length of current block with optimal trees
	Word32 m_uStaticLength;		///< bit length of current block with static trees
	Word m_uInsertHash;			///< hash index of string to be inserted
	Word m_uMatchLength;        ///< Length of best match
	Word m_uPreviousMatch;		///< previous match
	Word m_bMatchAvailable;		///< set if previous match exists
	Word m_uStringStart;		///< start of string to insert
	Word m_uMatchStart;			///< start of matching string
	Word m_uLookAhead;			///< number of valid bytes ahead in window
	Word m_uPreviousLength;		///< Length of the best match at previous step. Matches not greater than this are discarded. This is used in the lazy match evaluation.
	Word m_uLastLiteral;		///< running index in l_buf
	Word m_uMatches;			///< number of string matches in current block
	Word m_uBitIndexBuffer;		///< Number of valid bits in bi_buf.  All bits above the last valid bit are always zero.
	Word m_uBitIndexValid;		///< Number of bits in the output buffer
	Word m_bInitialized;		///< \ref TRUE if initialized
	Word m_uLastEOBLength;		///< bit length of EOB code for last block
	int m_iPending;				///< Number of bytes in the pending buffer
	int m_bNoHeader;			///< Suppress zlib header and adler32
	int m_iLastFlush;			///< Value of flush param for previous deflate call
	int m_iHeapLength;			///< number of elements in the heap
	int m_iHeapMaximum;			///< element of largest frequency
	eState m_eState;			///< As the name implies
	eDataType m_eDataType;		///< UNKNOWN, BINARY or ASCII
	Word8 m_bMethod;			///< STORED (for zip only) or DEFLATED

	CodeData_t m_DynamicLengthTrees[HEAP_SIZE];   ///< literal and length tree
	CodeData_t m_DynamicDistanceTrees[2*D_CODES+1]; ///< distance tree
	CodeData_t m_BitLengthTrees[2*BL_CODES+1];  ///< Huffman tree for bit lengths

	TreeDesc_t m_LiteralDescription;			///<desc. for literal tree
	TreeDesc_t m_DistanceDescription;			///< desc. for distance tree
	TreeDesc_t m_BitLengthDescription;			///< desc. for bit length tree
	int m_Heap[2*L_CODES+1];					///< heap used to build the Huffman trees
	Word16 m_Head[c_uHashSize];					///< Heads of the hash chains or 0.
	Word16 m_Previous[c_uWSize];				///< Link to older string with same hash index. To limit the size of this array to 64K, this link is maintained only for the last 32K strings. An index in this array is thus a window index modulo 32K.
	Word16 m_BitLengthCount[MAX_BITS+1];		///< MAX_BITS = 15, so this is long aligned
	Word16 m_DataBuffer[c_uLiteralBufferSize];		///< Buffer for distances. To simplify the code, d_buf and l_buf have the same number of elements. To use different lengths, an extra flag array would be necessary.
	Word8 m_LiteralBuffer[c_uLiteralBufferSize];	///< buffer for literals or lengths
	Word8 m_PendingBuffer[c_uLiteralBufferSize];	///< Output still pending
	Word8 m_Depth[2*L_CODES+1];					///< Depth of each subtree used as tie breaker for trees of equal frequency
	Word8 m_Window[c_uWSize*2];					///< Sliding window. Input bytes are read into the second half of the window, and move to the first half later to keep a dictionary of at least wSize bytes. With this organization, matches are limited to a distance of wSize-MAX_MATCH bytes, but this ensures that IO is always performed with a length multiple of the block size. 

	BURGER_INLINE Word TallyLiteral(Word uInput)
	{
		Word uLastLiteral = m_uLastLiteral;
		m_DataBuffer[uLastLiteral] = 0;
		m_LiteralBuffer[uLastLiteral] = static_cast<Word8>(uInput);
		++uLastLiteral;
		++m_DynamicLengthTrees[uInput].m_FrequencyCount.m_uFrequency;
		m_uLastLiteral = uLastLiteral;
		return (uLastLiteral == (c_uLiteralBufferSize-1));
	}

	BURGER_INLINE Word TallyDistance(Word uDistance,Word uLength)
	{
		Word uLastLiteral = m_uLastLiteral;
		m_DataBuffer[uLastLiteral] = static_cast<Word16>(uDistance);
		m_LiteralBuffer[uLastLiteral] = static_cast<Word8>(uLength);
		++uLastLiteral;
		--uDistance;
		m_DynamicLengthTrees[g_LengthCodes[uLength]+LITERALS+1].m_FrequencyCount.m_uFrequency++;
		m_DynamicDistanceTrees[(uDistance < 256) ? g_DistanceCodes[uDistance] : g_DistanceCodes[256+(uDistance>>7)]].m_FrequencyCount.m_uFrequency++;
		m_uLastLiteral = uLastLiteral;
		return (uLastLiteral == (c_uLiteralBufferSize-1));
	}

/* ===========================================================================
 * Send a value on a given number of bits.
 * IN assertion: length <= 16 and value fits in length bits.
 */

	BURGER_INLINE void SendBits(Word uInput,Word uLength)
	{
		Word uBits = m_uBitIndexBuffer | (uInput<<m_uBitIndexValid);
		if (static_cast<int>(m_uBitIndexValid) > static_cast<int>(c_uBufSize - uLength)) {
			Word uPending = static_cast<Word>(m_iPending);
			Word8 *pOutput = &m_PendingBuffer[uPending];
			m_iPending = static_cast<int>(uPending+2);
			pOutput[0] = static_cast<Word8>(uBits & 0xff);
			pOutput[1] = static_cast<Word8>(uBits >> 8);
			uBits = static_cast<Word>(static_cast<Word16>(uInput) >> (c_uBufSize - m_uBitIndexValid));
			uLength -= c_uBufSize;
		}
		m_uBitIndexBuffer = uBits;
		m_uBitIndexValid += uLength;
	}

	BURGER_INLINE void send_code(Word uCode,const CodeData_t *pTree) 
	{
		SendBits(pTree[uCode].m_FrequencyCount.m_uCode,pTree[uCode].m_DataLength.m_uLength); 
	}

/* ===========================================================================
 * Initialize the hash table (avoiding 64K overflow for 16 bit systems).
 * prev[] will be initialized on the fly.
 */
	BURGER_INLINE void ClearHash(void)
	{
		MemoryClear(m_Head,sizeof(m_Head));
	}

/* ===========================================================================
 * Update a hash value with the given input byte
 * IN  assertion: all calls to to UPDATE_HASH are made with consecutive
 *    input characters, so that a running hash key can be computed from the
 *    previous key instead of complete recalculation each time.
 */
    static BURGER_INLINE Word UpdateHash(Word uHash,Word8 uInput)
	{
		return ((uHash<<c_uHashShift) ^ uInput) & c_uHashMask;
	}

/* ===========================================================================
 * Insert string str in the dictionary and set match_head to the previous head
 * of the hash chain (the most recent string with same hash key). Return
 * the previous length of the hash chain.
 * If this file is compiled with -DFASTEST, the compression level is forced
 * to 1, and no hash chains are maintained.
 * IN  assertion: all calls to to INSERT_STRING are made with consecutive
 *    input characters and the first MIN_MATCH bytes of str are valid
 *    (except for the last MIN_MATCH-1 bytes of the input file).
 */
	BURGER_INLINE Word InsertString(Word uStringIndex)
	{
		m_uInsertHash = UpdateHash(m_uInsertHash,m_Window[uStringIndex + (MIN_MATCH-1)]);
		Word uMatchHead = m_Head[m_uInsertHash];
		m_Previous[uStringIndex & c_uWMask] = static_cast<Word16>(uMatchHead);
		m_Head[m_uInsertHash] = static_cast<Word16>(uStringIndex);
		return uMatchHead;
	}

/* ===========================================================================
 * Flush the current block, with given end-of-file flag.
 * IN assertion: strstart is set to the end of the current match.
 */
	BURGER_INLINE void FlushBlock(Word bEOF)
	{
		FlushBlock((m_iBlockStart >= 0) ? &m_Window[static_cast<Word>(m_iBlockStart)] : NULL,static_cast<Word32>(m_uStringStart - m_iBlockStart),bEOF);
		m_iBlockStart = static_cast<IntPtr>(m_uStringStart);
		FlushPending();
	}

	void OutputBigEndian16(Word b);
	void BitIndexFlush(void);
	void BitIndexFlushToByte(void);
	void CopyBlock(const Word8 *pInput,WordPtr uInputLength);
	void SetDataType(void);
	void InitBlock(void);
	void StaticTreeInit(void);
	void PQDownHeap(const CodeData_t *pTree,int k);
	void CompressBlock(const CodeData_t *pLengthTree,const CodeData_t *pDistanceTree);
	WordPtr ReadBuffer(Word8 *pOutput,WordPtr uOutputSize);
	void FillWindow(void);
	void GenerateBitLengths(const TreeDesc_t *pTreeDescription);
	void GenerateCodes(CodeData_t *tree, int max_code, Word16 *bl_count);
	void BuildTree(TreeDesc_t *desc);
	void ScanTree(CodeData_t *tree,int max_code);
	int BuildBitLengthTree(void);
	void StoredBlock(const Word8 *buf,Word32 stored_len,int eof);
	void SendTree(CodeData_t *tree, int max_code);
	void SendAllTrees(int lcodes,int dcodes,int blcodes);
	void FlushBlock(const Word8 *buf,Word32 stored_len,Word bEOF);
	void FlushPending(void);
	Word LongestMatch(Word cur_match);
	eBlockState DeflateSlow(int flush);
	void Align(void);
	int DeflateEnd(void);
	void LongestMatchInit(void);
	int DeflateReset(void);
	int DeflateInit(void);
	int PerformDeflate(int flush);

	static const int g_ExtraLengthBits[LENGTH_CODES];
	static const int g_ExtraDistanceBits[D_CODES];
	static const int g_ExtraBitLengthBits[BL_CODES];
	static const Word8 g_BitLengthOrder[BL_CODES];
	static const CodeData_t g_StaticLengthTrees[L_CODES+2];
	static const CodeData_t g_StaticDistanceTrees[D_CODES];
	static const int g_BaseLengths[LENGTH_CODES];
	static const int g_BaseDistances[D_CODES];
	static const StaticTreeDesc_t g_StaticLengthDescription;
	static const StaticTreeDesc_t g_StaticDistanceDescription;
	static const StaticTreeDesc_t g_StaticBitLengthDescription;
	static const Word8 g_DistanceCodes[DIST_CODE_LEN];
	static const Word8 g_LengthCodes[MAX_MATCH-MIN_MATCH+1];
public:
	static const Word32 Signature = 0x5A4C4942;		///< 'ZLIB'
	CompressDeflate(void);
	eError Init(void) BURGER_OVERRIDE;
	eError Process(const void *pInput, uintptr_t uInputLength) BURGER_OVERRIDE;
	eError Finalize(void) BURGER_OVERRIDE;
};
}
/* END */

#endif

