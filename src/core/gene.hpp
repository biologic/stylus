/*******************************************************************************
 * \file	gene.hpp
 * \brief	Stylus Gene class
 *
 * A Gene represents a sequence of bases within the genome possibly corresponding
 * to a Han glyph. It divides the bases into segments - continuous coherent or
 * incoherent sequences, strokes - collections of coherent and very small
 * incoherent segments, and groups - collections of logically related strokes
 * determined by the corresponding Han glyph. Though Segments, Strokes, and
 * Groups are defined as separate classes, the Gene tends to manage them rather
 * than treating them opaquely.
 *
 * NOTE:
 * - All gene sub-objects, such as segments and strokes, should keep their
 *   ranges and other index values relative to the start of the gene, not
 *   the genome since genes may move about (due to insertion/deletion events)
 *
 * Stylus, Copyright 2006-2009 Biologic Institute
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************/

#ifndef GENE_HPP
#define GENE_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Association between bases and a Han stroke
	 *
	 */
	class StrokeRange : public Range
	{
	public:
		StrokeRange();
		StrokeRange(const StrokeRange& sr);

		StrokeRange& operator=(const StrokeRange& sr);

		void load(XMLDocument* pxd, xmlNodePtr pxnStroke);

		size_t getCorrespondsTo() const;

	private:
		size_t _nCorrespondsTo;					///< Associated Han stroke (by index)
	};
	
	/**
	 * \brief A continuous sequence of coherent or incoherent vectors
	 *
	 */
	class Segment
	{
		friend Segment& operator+(Segment& sg, ACIDTYPE at);
		friend Segment& operator+=(Segment& sg, ACIDTYPE at);
		friend Segment& operator-(Segment& sg, ACIDTYPE at);
		friend Segment& operator-=(Segment& sg, ACIDTYPE at);

	public:
		Segment();
		Segment(const Segment& sg);

		Segment& operator=(const Segment& sg);
		Segment& operator=(ACIDTYPE at);
		
		bool operator==(const Segment& sg) const;
		bool operator!=(const Segment& sg) const;
		bool operator<(const Segment& sg) const;
		bool operator>(const Segment& sg) const;

		void setRange(const Range& rgAcids);
		const Range& getRange() const;

		void setStart(long d);
		long getStart() const;
		
		void setEnd(long d);
		long getEnd() const;

		void extendRange(long d);
		void moveRange(long d);

		bool isCoherent() const;
		void setCoherent(bool fIsCoherent);
		
		UNIT length() const;

	private:
		Range _rgAcids;							///< Range of acids within the segment
		bool _fIsCoherent;						///< Segment is coherent or incoherent
		Unit _nLength;							///< Sum of contained vectors
		
		int compare(const Segment& sg) const;
	};

	/**
	 * \brief Advance the segment length by the acid length
	 * \param[in] pt The Segment to adjust
	 * \param[in] at The ACIDTYPE to retrieve
	 */
	//{@
	Segment& operator+(Segment& sg, ACIDTYPE at);
	Segment& operator+=(Segment& sg, ACIDTYPE at);
	//@}

	/**
	 * \brief Retreat the segment length by the acid length
	 * \param[in] pt The Segment to adjust
	 * \param[in] at The ACIDTYPE to retrieve
	 */
	//{@
	Segment& operator-(Segment& sg, ACIDTYPE at);
	Segment& operator-=(Segment& sg, ACIDTYPE at);
	//@}

	/**
	 * \brief A sequence of bases corresponding to a stroke in a Han glyph
	 *
	 */
	class Stroke
	{
	public:
		Stroke();
		Stroke(const Stroke& st);

		Stroke& operator=(const Stroke& st);

		bool operator==(const Stroke& stroke) const;
		bool operator!=(const Stroke& stroke) const;
		bool operator<(const Stroke& stroke) const;
		bool operator>(const Stroke& stroke) const;

		void setID(size_t id);
		size_t getID() const;

		void setRange(const Range& rgAcids);
		const Range& getRange() const;

		void setStart(long d);
		long getStart() const;
		
		void setEnd(long d);
		long getEnd() const;

		void extendRange(long d);
		void moveRange(long d);

 		void incSegments(size_t n = 1);
		size_t getSegments() const;

		void incDropouts(size_t n = 1);
		size_t getDropouts() const;

		enum STROKEINVALID
		{
			STI_DIMENSIONS = 0x0001,		///< Invalidate dimension values
			STI_SEGMENTS   = 0x0002,		///< Invalidate segment related values
			STI_LOCATION   = 0x0004,		///< Invalidate location related values
			STI_SCORE      = 0x0008,		///< Invalidate score related values

			STI_ALL = STI_DIMENSIONS | STI_SEGMENTS | STI_LOCATION | STI_SCORE
		};

		void invalidate(STFLAGS grfInvalid = STI_ALL);

		void calcDimensions(Gene& gene);
		bool calcScale(Gene& gene, const HStroke& hst);
		void promoteScale(UNIT sxToHan, UNIT syToHan);
		void calcOffsets(Group& grp, const HStroke& hst);

		bool calcScore(Gene& gene, const HStroke& hst);

		const Rectangle& getBounds() const;

		const bool sxIsInherited() const;
		const bool syIsInherited() const;

		const UNIT sxToHan() const;
		const UNIT syToHan() const;
		const UNIT sxyToHan() const;
		const UNIT dxToHan() const;
		const UNIT dyToHan() const;
		const UNIT dxParentToHan() const;
		const UNIT dyParentToHan() const;

		const UNIT deviation() const;
		const UNIT extraLength() const;

#ifdef ST_TRACE
		void traceScore() const;
#endif

	private:
		size_t _id;								///< Stroke ID (used mostly in trace statements)

		Range _rgAcids;							///< Range of acids within the stroke

		size_t _cSegments;						///< Number of segments contained within the stroke
		size_t _cDropouts;						///< Number of dropouts within the stroke

		Rectangle _rectBounds;					///< Stroke bounding rectangle

		bool _sxIsInherited;					///< sxToHan was inherited
		bool _syIsInherited;					///< sxToHan was inherited

		ScaledLength _slVectors;				///< Length of contained vectors

		Unit _sxToHan;							///< X-dimension Han scaling factor
		Unit _syToHan;							///< Y-dimension Han scaling factor
		Unit _sxyToHan;							///< Computed scale to apply to diagonal vectors

		Unit _dxToHan;							///< X-dimension Han translation quantity
		Unit _dyToHan;							///< Y-dimension Han translation quantity

		Unit _dxParentToHan;					///< X-dimension Han translation quantity (using Group scaling)
		Unit _dyParentToHan;					///< Y-dimension Han translation quantity (using Group scaling)

		Unit _nDeviation;						///< Maximum deviation between stroke and Han
		Unit _nExtraLength;						///< Length of Stroke length in excess of Han length

		int compare(const Stroke& stroke) const;

		const HPOINTARRAY& determineStrokeOrientation(const Gene& gene, const HStroke& hst);
	};

	/**
	 * \brief A collection of related strokes determined by the Han glyph
	 *
	 */
	class Group
	{
	public:
		Group();
		Group(const Group& grp);

		Group& operator=(const Group& grp);
		
		bool operator==(const Group& group) const;
		bool operator!=(const Group& group) const;

		void setID(size_t id);
		size_t getID() const;

		enum GROUPINVALID
		{
			GPI_DIMENSIONS = 0x0001,			///< Invalidate dimension values
			GPI_SCORE = 0x0002,					///< Invalidate score values

			GPI_ALL = GPI_DIMENSIONS | GPI_SCORE
		};

		void invalidate(STFLAGS grfInvalid = GPI_ALL);

		void calcDimensions(Gene& gene, const HGroup& hgrp);
		bool calcScale(Gene& gene, const Han& han, const HGroup& hgrp);
		void promoteScale(Gene& gene, const HGroup& hgrp, UNIT sxToHan, UNIT syToHan);
		void calcOffsets(Gene& gene, const Han& han, const HGroup& hgrp);

		bool calcScore(Gene& gene, const Han& han, const HGroup& hgrp);
		void incIllegalOverlaps();
		void incMissingOverlaps();

		const Rectangle& getBounds() const;

		const bool sxIsInherited() const;
		const bool syIsInherited() const;

		const UNIT sxToHan() const;
		const UNIT syToHan() const;
		const UNIT dxToHan() const;
		const UNIT dyToHan() const;
		const UNIT dxParentToHan() const;
		const UNIT dyParentToHan() const;

		const UNIT exponent(SCORECOMPONENT sc) const;
		const UNIT score() const;

#ifdef ST_TRACE
		void traceScore() const;
#endif
	private:
		size_t _id;								///< Group ID (used mostly in trace statements)
		Rectangle _rectBounds;					///< Group bounding rectangle

		bool _sxIsInherited;					///< sxToHan was inherited
		bool _syIsInherited;					///< sxToHan was inherited

		Unit _sxToHan;							///< Weighted mean of Stroke sxToHan
		Unit _syToHan;							///< Weighted mean of Stroke syToHan

		Unit _dxToHan;							///< Weighted mean of X-dimension Han translation quantity at center
		Unit _dyToHan;							///< Weighted mean of Y-dimension Han translation quantity at center

		Unit _dxParentToHan;					///< Weighted mean of X-dimension Han translation quantity at center (using Gene scaling)
		Unit _dyParentToHan;					///< Weighted mean of Y-dimension Han translation quantity at center (using Gene scaling)

		Unit _aryScoreExponents[SC_GROUPMAX];	///< Score exponent values
		Unit _nScore;							///< Group score
	
		int compare(const Group& group) const;
	};

	/**
	 * \brief A change record documenting a modification to stroke ranges
	 *
	 */
	class StrokeRangeChange : public IModification
	{
	public:
		StrokeRangeChange(size_t idGene, const std::vector<Range>& vecStrokeRanges);
		virtual ~StrokeRangeChange();

		virtual void undo();
		virtual size_t length() const;
		virtual std::string toString() const;
		virtual void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	private:
		size_t _idGene;
		std::vector<Range> _vecStrokeRanges;
	};
	
	/**
	 * \brief The range of bases from the Genome corresponding to a single Han glyph.
	 *
	 * Genes contain Groups and Strokes; it tends to manage them recursively, though some
	 * aspects (tracking overlaps) is handled at the gene level.
	 */
	class Gene
	{
		friend class StrokeRangeChange;
		
	public:
		enum GENECHANGE
		{
			GC_CHANGE = 0,
			GC_INSERT,
			GC_DELETE
		};
		
		Gene();
		Gene(const Gene& gene);

		Gene& operator=(const Gene& gene);
		
		bool operator==(const Gene& gene) const;
		bool operator!=(const Gene& gene) const;

		void setID(size_t id);
		size_t getID() const;

		void load(XMLDocument* pxd, xmlNodePtr pxnGene);

		///-------------------------------------------------------------------------------
		/// This section includes methods whose function is independent of the Han glyph
		/// used in scoring.
		///-------------------------------------------------------------------------------
		//{@
		void setOrigin(const Point& ptOrigin);	///< Set the start point for the gene

		/**
		 * \brief Associate the gene with a range of bases
		 *
		 * This routine is, essentially, an initializer for the Gene.
		 * All internal state is reset by this routine, including any
		 * Han related data.
		 */
		void setRange(const Range& rgBases);
		const Range& getRange() const;

		void extendRange(long d);
		void moveRange(long d);

		bool intersectsRange(const Range& rg) const;

		Range codonToBaseRange(const Range& rgCodons) const;
		Range baseToCodonRange(const Range& rgBases) const;

		const ACIDTYPEARRAY& getAcids() const;
		const POINTARRAY& getPoints() const;
		const SEGMENTARRAY& getSegments() const;
		//@}

		///-------------------------------------------------------------------------------
		/// This section includes methods whose function is dependent on the Han glyph
		/// used in scoring.
		///-------------------------------------------------------------------------------
		//{@
		const std::string& getUnicode() const;
		const STROKEARRAY& getStrokes() const;
		STROKEARRAY& getStrokes();

		size_t mapHanToStroke(size_t iHStroke) const;
		size_t mapStrokeToHan(size_t iStroke) const;
		size_t mapStrokeToGroup(size_t iStroke) const;

		const Rectangle& getBounds() const;
		const UNIT sxToHan() const;
		const UNIT syToHan() const;
		const UNIT dxToHan() const;
		const UNIT dyToHan() const;

		const UNIT score() const;
		const UNIT units() const;
		//@}

		void markInvalid();
		void markInvalid(GENECHANGE gc, const Range& rg, bool fSilent);

		bool ensureCompiled();
		bool ensureScore();
		bool ensureValid();

		std::string toString() const;
		void toXML(XMLStream& xs, STFLAGS grfRecordDetail) const;

	private:
		static const size_t s_cDROPOUT = 1;		///< Number of continuous incoherent vectors allowed within a stroke

		///-------------------------------------------------------------------------------
		/// This section includes data members whose values derive from the base sequence
		/// assigned to the gene. These values are not dependent upon the Han glyph being
		/// used in scoring.
		///-------------------------------------------------------------------------------
		//{@
		size_t _id;								///< Gene ID
		Range _rgBases;							///< Range of bases, in the genome, belonging to the gene

		/**
		 * \brief Gene invalidation flags
		 *
		 */
		enum GENEINVALID
		{
			GI_NONE       = 0x0000,				///< All parts are valid

			GI_ACIDS      = 0x0001,				///< Acid array is invalid
			GI_POINTS     = 0x0002,				///< Points array is invalid
			GI_COHERENCE  = 0x0004,				///< Coherence array is invalid
			GI_SEGMENTS   = 0x0008,				///< Segments are invalid
			GI_COMPILED   = GI_ACIDS | GI_POINTS | GI_COHERENCE | GI_SEGMENTS,

			GI_STROKES    = 0x0010,				///< Strokes-to-segment assignments are invalid
			GI_DIMENSIONS = 0x0020,				///< Dimensions (all) are invalid
			GI_OVERLAPS   = 0x0040,				///< Overlaps are invalid
			GI_VALIDATED  = GI_STROKES | GI_DIMENSIONS | GI_OVERLAPS,

			GI_SCORE      = 0x0100,				///< Scores are invalid
			GI_SCORED     = GI_SCORE,

			GI_ALL        = GI_COMPILED | GI_VALIDATED | GI_SCORED
		};
		STFLAGS _grfInvalid;					///< Set of GENEINVALID flags

		ACIDTYPEARRAY _vecAcids;				///< Array of acids in the gene
		POINTARRAY _vecPoints;					///< Array of points *after* applying codon vector
		UINTARRAY _vecCoherent;					///< Array of coherence count for each vector

		SEGMENTARRAY _vecSegments;				///< Array of Segments within the gene
		size_t _cCoherent;						///< Number of coherent segments within the gene
		Unit _nUnits;							///< Gene unit length
		//@}

		///-------------------------------------------------------------------------------
		/// This section includes data members whose values depend on the Han glyph
		/// used in scoring.
		/// 
		/// NOTE:
		/// - Scoring against multiple Han glyphs simultaneously will require replicating
		///   and managing this data once per glyph.
		/// TODO: Add support for multiple Han glyphs per gene
		///-------------------------------------------------------------------------------
		//{@
		std::string _strUnicode;				///< Unicode of associated Han glyph
		HOVERLAPARRAY _vecHOverlaps;			///< Han overlaps converted to gene stroke indexes

		GROUPARRAY _vecGroups;					///< Array of Groups within the gene

		bool _fStrokesAssigned;					///< Strokes locations were originally assigned
		STROKEARRAY _vecStrokes;				///< Array of Strokes within the gene
		NUMERICMAP _mapStrokeToHan;				///< Map of gene strokes to Han glyph strokes (by index)
		NUMERICMAP _mapHanToStroke;				///< Map of Han glyph strokes to gene strokes (by index)
		NUMERICMAP _mapStrokeToGroup;			///< Map of gene strokes to the containing group (by index)

		NUMERICMAP _vecMarks;					///< Marks within the gene (by index into segments)
		STROKEOVERLAPS _setOverlaps;			///< Array of overlaps between strokes
		NUMERICPAIRARRAY _vecMissingOverlaps;	///< Array of missing overlaps
		NUMERICPAIRARRAY _vecIllegalOverlaps;	///< Array of illegal overlaps

		Point _ptOrigin;						///< Point of origin
		Rectangle _rectBounds;					///< Gene bounding rectangle

		Unit _sxToHan;							///< Weighted mean of Group sxToHan
		Unit _syToHan;							///< Weighted mean of Group syToHan
		Unit _sxyToHan;							///< Computed scale to apply to diagonal vectors
		
		Unit _dxToHan;							///< Weighted mean of Group dxToHan
		Unit _dyToHan;							///< Weighted mean of Group dyToHan

		Unit _aryScoreExponents[SC_GENEMAX];	///< Score exponent values
		Unit _nScore;							///< Gene score
		//@}

		void invalidate();

		bool isInvalid(STFLAGS grf) const;
		bool isValid(STFLAGS grf) const;
		void markValid(STFLAGS grf);
		void markInvalid(STFLAGS grf);

		void ensureAcids(size_t iAcidChange, long cAcidsChanged);
		void ensureCoherence();
		void ensureSegments();
		void ensureStrokes();
		void ensureDimensions();
		void ensureOverlaps();

		bool calcScore();
		void scoreOverlaps(const Han& han);

		void validateStrokeRanges(const STROKERANGEARRAY& vecStrokeRange) const;
		
		int compare(const Gene& gene) const;

#ifdef ST_TRACE
		void traceGene() const;
		void traceGroups() const;
		void traceOverlaps() const;
		void traceSegments() const;
		void traceStrokes() const;
		void traceStrokeOverlaps() const;
		void traceScores() const;
#endif
	};

} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // GENE_HPP
