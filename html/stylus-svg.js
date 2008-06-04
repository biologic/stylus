//----------------------------------------------------------------------------
// SVG Javascript
// 
// Stylus, Copyright 2006-2008 Biologic Institute
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//----------------------------------------------------------------------------

function isVisible(e)
{
	return !e.hasAttribute('opacity') || e.getAttribute('opacity') != 0.0;
}

function makeVisible(e, f)
{
	e.setAttribute('opacity', (f ? 1.0 : 0.0));
}

function showHide(id)
{
	var c = document.getElementById(id + '-control');
	if (c)
	{
		var e = null;
		var f = false;

		if (id == 'labels' || id == 'vertices' || id == 'coherent' || id == 'incoherent' || id == 'marks' || id == 'dropouts')
		{
			var eL = document.getElementById('labels');
			var eD = document.getElementById('vertices');

			var eC = document.getElementById('coherent');
			var eCL = document.getElementById('labels-coherent');
			var eCD = document.getElementById('vertices-coherent');

			var eI = document.getElementById('incoherent');
			var eIL = document.getElementById('labels-incoherent');
			var eID = document.getElementById('vertices-incoherent');

			var eM = document.getElementById('marks');
			var eMD = document.getElementById('vertices-marks');

			var eDO = document.getElementById('dropouts');
			var eDOD = document.getElementById('vertices-dropouts');

			if (eL && eD && eC && eCL && eCD && eI && eIL && eID && eM && eMD && eDO && eDOD)
			{
				var fVisibleL = isVisible(eL);
				var fVisibleD = isVisible(eD);
				var fVisibleC = isVisible(eC);
				var fVisibleI = isVisible(eI);
				var fVisibleM = isVisible(eM);
				var fVisibleDO = isVisible(eDO);

				var fShowCL = fVisibleL && fVisibleC;
				var fShowCD = fVisibleD && fVisibleC;
				var fShowIL = fVisibleL && fVisibleI;
				var fShowID = fVisibleD && fVisibleI;
				var fShowMD = fVisibleD && fVisibleM;
				var fShowDOD = fVisibleD && fVisibleDO;

				if (id == 'labels')
				{
					e = eL;
					f = !fVisibleL;
					fShowCL = f && fVisibleC;
					fShowIL = f && fVisibleI;
				}
				else if (id == 'vertices')
				{
					e = eD;
					f = !fVisibleD;
					fShowCD = f && fVisibleC;
					fShowID = f && fVisibleI;
					fShowMD = f && fVisibleM;
					fShowDOD = f && fVisibleDO;
				}
				else if (id == 'coherent')
				{
					e = eC;
					f = !fVisibleC;
					fShowCL = fVisibleL && f;
					fShowCD = fVisibleD && f;
				}
				else if (id == 'incoherent')
				{
					e = eI;
					f = !fVisibleI;
					fShowIL = fVisibleL && f;
					fShowID = fVisibleD && f;
				}
				else if (id == 'marks')
				{
					e = eM;
					f = !fVisibleM;
					fShowMD = fVisibleD && f;
				}
				else if (id == 'dropouts')
				{
					e = eDO;
					f = !fVisibleDO;
					fShowDOD = fVisibleD && f;
				}

				makeVisible(eCL, fShowCL);
				makeVisible(eCD, fShowCD);
				makeVisible(eIL, fShowIL);
				makeVisible(eID, fShowID);
				makeVisible(eMD, fShowMD);
				makeVisible(eDOD, fShowDOD);
			}
		}
		else
		{
			e = document.getElementById(id);
			f = e && !isVisible(e);
		}

		if (e)
			makeVisible(e, f);
		c.setAttribute('fill', (f ? '#999' : '#fff'));
	}
}
