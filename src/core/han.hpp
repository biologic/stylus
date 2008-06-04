/*******************************************************************************
 * \file	han.hpp
 * \brief	Stylus Han and helper classes
 *
 * Each instance of the Han class describes a single Han glyph (identified by the
 * contained Unicode value).
 *
 * Stylus, Copyright 2006-2008 Biologic Institute
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

#ifndef HAN_HPP
#define HAN_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief HDimensions class
	 *
	 * HDimensions manages dimensions common to different elements of a Han
	 * glyph.
	 */
	class HDimensions
	{
	public:
		HDimensions();
		HDimensions(const HDimensions& hd);

		HDimensions& operator=(const HDimensions& hd);
		
		void set(const HDimensions& hd);

		void load(XMLDocument* pxd, xmlNodePtr pxn);

		UNIT getLength() const;
		const Rectangle& getBounds() const;

	private:
		Unit _nLength;							///< Length of all contained strokes
		Rectangle _rectBounds;					///< Bounds of the element
	};

	/**
	 * \brief HStroke class
	 *
	 * A stroke within a Han definition.
	 */
	class HStroke : public HDimensions
	{
	public:
		HStroke();
		HStroke(const HStroke& hs);

		HStroke& operator=(const HStroke& hs);
		
		void set(const HStroke& hs);

		void load(XMLDocument* pxd, xmlNodePtr pxnStroke);

		const HPOINTARRAY& getPointsForward() const;
		const HPOINTARRAY& getPointsReverse() const;

	private:
		HPOINTARRAY _vecPointsForward;			///< Stroke points moving forward
		HPOINTARRAY _vecPointsReverse;			///< Stroke points moving in reverse
	};

	/**
	 * \brief HGroup class
	 *
	 * A group within a Han definition.
	 */
	class HGroup : public HDimensions
	{
	public:
		HGroup();
		HGroup(const HGroup& hg);

		HGroup& operator=(const HGroup& hg);
		
		void set(const HGroup& hg);

		void load(XMLDocument* pxd, xmlNodePtr pxnGroup);

		const Point& getWeightedCenter() const;

		const NUMERICARRAY& getStrokes() const;

	private:
		Point _ptWeightedCenter;				///< Stroke-ink weighted center point
		NUMERICARRAY _vecStrokes;				///< Array of contained strokes (by index)
	};

	/**
	 * \brief HOverlap class
	 *
	 * Describe a required or optional overlap between two strokes in a Han glyph.
	 */
	class HOverlap
	{
	public:
		HOverlap();
		HOverlap(const HOverlap& ho);

		HOverlap& operator=(const HOverlap& ho);
		
		void set(const HOverlap& ho);

		void load(XMLDocument* pxd, xmlNodePtr pxnOverlap);

		bool operator==(const HOverlap& ol) const;
		bool operator<(const HOverlap& ol) const;
		bool operator<=(const HOverlap& ol) const;
		bool operator>(const HOverlap& ol) const;
		bool operator>=(const HOverlap& ol) const;

		int compare(const HOverlap& hol) const;

		bool operator==(const STROKEOVERLAP& ol) const;
		bool operator<(const STROKEOVERLAP& ol) const;
		bool operator<=(const STROKEOVERLAP& ol) const;
		bool operator>(const STROKEOVERLAP& ol) const;
		bool operator>=(const STROKEOVERLAP& ol) const;

		int compare(const STROKEOVERLAP& ol) const;

		size_t getFirst() const;
		size_t getSecond() const;

		bool isRequired() const;

		void convertStrokes(const NUMERICMAP& mapHanToStroke);

	private:
		size_t _iStroke1;
		size_t _iStroke2;
		bool _fRequired;
	};

	/**
	 * \brief Han class
	 *
	 * Describe a single Han glypyh used in scoring.
	 */
	class Han : public HDimensions
	{
		friend class HanPtr;
		
	public:
		static void initialize();
		static void terminate();
		
		static void setScope(const char* pszScope);

		static const Han& getDefinition(const std::string& strUnicode);

		Han();
		Han(const Han& han);

		Han& operator=(const Han& han);

		void set(const Han& han);

		void load(XMLDocument* pxd, xmlNodePtr pxnHanDefinition);
		
		bool operator==(const Han& han) const;
		bool operator!=(const Han& han) const;
		bool operator<(const Han& han) const;
		bool operator<=(const Han& han) const;
		bool operator>(const Han& han) const;
		bool operator>=(const Han& han) const;

		bool operator==(const std::string& strUnicode) const;
		bool operator!=(const std::string& strUnicode) const;
		bool operator<(const std::string& strUnicode) const;
		bool operator<=(const std::string& strUnicode) const;
		bool operator>(const std::string& strUnicode) const;
		bool operator>=(const std::string& strUnicode) const;

		const std::string& getUUID() const;
		const std::string&  getUnicode() const;

		UNIT getMinimumStrokeLength() const;

		const HGROUPARRAY& getGroups() const;
		const HSTROKEARRAY& getStrokes() const;
		const HOVERLAPARRAY& getOverlaps() const;

		const HGroup& mapStrokeToGroup(size_t iStroke) const;

	private:
		static std::string s_strScope;			///< URL from which to obtain Han definitions
		static std::vector<Han*> s_vecHan;		///< An array of loaded Han definitions

		std::string _strUUID;
		std::string _strUnicode;

		Unit _nMinimumStrokeLength;

		HGROUPARRAY _vecGroups;
		HSTROKEARRAY _vecStrokes;
		HOVERLAPARRAY _vecOverlaps;

		NUMERICMAP _mapStrokeToGroup;
	};

} // namespace org_biologicinstitute_stylus
namespace stylus = org_biologicinstitute_stylus;

#endif // HAN_HPP
