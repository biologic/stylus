/*******************************************************************************
 * \file	codon.cpp
 * \brief	Stylus Codon class
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

// Includes ---------------------------------------------------------------------
#include "headers.hpp"

using namespace std;
using namespace stylus;

// NOTE: This array MUST be in the same order as the ACIDTYPE enumeration
const Acid Acid::s_aryACIDS[ACID_MAX] =
{
	// Stop vector
	{ 0, 0, 0.0, DIR_STOP, "STP" },

	// NORTH vectors
	{ 0,									Constants::s_nVECTOR_SHORT,			Constants::s_nVECTOR_SHORT,		DIR_NORTH,		"Nos" },
	{ 0,									Constants::s_nVECTOR_MEDIUM,		Constants::s_nVECTOR_MEDIUM,	DIR_NORTH,		"Nom" },
	{ 0,									Constants::s_nVECTOR_LONG,			Constants::s_nVECTOR_LONG,		DIR_NORTH,		"Nol" },

	// NORTHEAST vectors
	{ Constants::s_nSHORT_DIAGONALSTEP,		Constants::s_nSHORT_DIAGONALSTEP,	Constants::s_nVECTOR_SHORT,		DIR_NORTHEAST, 	"Nes" },
	{ Constants::s_nMEDIUM_DIAGONALSTEP,	Constants::s_nMEDIUM_DIAGONALSTEP,	Constants::s_nVECTOR_MEDIUM,	DIR_NORTHEAST, 	"Nem" },

	// EAST vectors
	{ Constants::s_nVECTOR_SHORT,			0,  								Constants::s_nVECTOR_SHORT,		DIR_EAST,	   	"Eas" },
	{ Constants::s_nVECTOR_MEDIUM,			0,									Constants::s_nVECTOR_MEDIUM,	DIR_EAST,	   	"Eam" },
	{ Constants::s_nVECTOR_LONG,			0,									Constants::s_nVECTOR_LONG,		DIR_EAST,	   	"Eal" },

	// SOUTHEAST vectors
	{ Constants::s_nSHORT_DIAGONALSTEP,		-Constants::s_nSHORT_DIAGONALSTEP,	Constants::s_nVECTOR_SHORT,		DIR_SOUTHEAST, 	"Ses" },
	{ Constants::s_nMEDIUM_DIAGONALSTEP,	-Constants::s_nMEDIUM_DIAGONALSTEP,	Constants::s_nVECTOR_MEDIUM,	DIR_SOUTHEAST, 	"Sem" },

	// SOUTH vectors
	{ 0,									-Constants::s_nVECTOR_SHORT,		Constants::s_nVECTOR_SHORT,		DIR_SOUTH,	   	"Sos" },
	{ 0,									-Constants::s_nVECTOR_MEDIUM,		Constants::s_nVECTOR_MEDIUM,	DIR_SOUTH,	   	"Som" },
	{ 0,									-Constants::s_nVECTOR_LONG,			Constants::s_nVECTOR_LONG,		DIR_SOUTH,	   	"Sol" },

	// SOUTHWEST vectors
	{ -Constants::s_nSHORT_DIAGONALSTEP,	-Constants::s_nSHORT_DIAGONALSTEP,	Constants::s_nVECTOR_SHORT,		DIR_SOUTHWEST, 	"Sws" },
	{ -Constants::s_nMEDIUM_DIAGONALSTEP,	-Constants::s_nMEDIUM_DIAGONALSTEP,	Constants::s_nVECTOR_MEDIUM,	DIR_SOUTHWEST, 	"Swm" },

	// WEST vectors
	{ -Constants::s_nVECTOR_SHORT,			0,									Constants::s_nVECTOR_SHORT,		DIR_WEST,	   	"Wes" },
	{ -Constants::s_nVECTOR_MEDIUM,			0,									Constants::s_nVECTOR_MEDIUM,	DIR_WEST,	   	"Wem" },
	{ -Constants::s_nVECTOR_LONG,			0,									Constants::s_nVECTOR_LONG,		DIR_WEST,	   	"Wel" },

	// NORTHWEST vectors
	{ -Constants::s_nSHORT_DIAGONALSTEP,	Constants::s_nSHORT_DIAGONALSTEP,   Constants::s_nVECTOR_SHORT,		DIR_NORTHWEST, 	"Nws" },
	{ -Constants::s_nMEDIUM_DIAGONALSTEP,	Constants::s_nMEDIUM_DIAGONALSTEP,  Constants::s_nVECTOR_MEDIUM,	DIR_NORTHWEST, 	"Nwm" }
};

const std::string Codon::s_strSTART("ATG");

const bool Codon::s_aryCOHERENCE[ACID_MAX][ACID_MAX][ACID_MAX] =
{
	{
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  }
	},

	{
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  }
	},

	{
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  }
	},

	{
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, true , true  },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, true , true  },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  }
	},

	{
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, true , true  },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, true , true  },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  }
	},

	{
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false }
	},

	{
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true  },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true  },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	},

	{
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , false, false, false, false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , false, false },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true  },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true , true , true  },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	},

	{
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	},

	{
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	},

	{
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , false, false },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, true , true , true , true , true , true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	},

	{
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	},

	{
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP    STP
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos    Nos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom    Nom
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol    Nol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes    Nes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem    Nem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, false, false, false, false, false, true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas    Eas
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam    Eam
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal    Eal
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses    Ses
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem    Sem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos    Sos
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som    Som
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol    Sol
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws    Sws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm    Swm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, false, false, false, false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes    Wes
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem    Wem
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel    Wel
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , false, false, false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws    Nws
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  },

		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm    Nwm
		// STP    Nos    Nom    Nol    Nes    Nem    Eas    Eam    Eal    Ses    Sem    Sos    Som    Sol    Sws    Swm    Wes    Wem    Wel    Nws    Nwm
		{  false, true , true , true , true , true , false, false, false, false, false, false, false, false, true , true , true , true , true , true , true  }
	}
};
