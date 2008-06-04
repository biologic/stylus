//-----------------------------------------------------------------------------
// HTML Javascript
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
//-----------------------------------------------------------------------------
function toggle(id)
{
	var e = document.getElementById(id);
	var a = document.getElementById('a-' + id);

	if (e && a)
	{
		if (e.style.display != 'none')
		{
			a.innerHTML = '[+]';
			e.style.display = 'none';
			e.style.height = 0;
		}
		else
		{
			a.innerHTML = '[-]';
			e.style.display = '';
			e.style.height = '';
		}
	}
}

function getExpandedLink()
{
	var strLink = '<span class="sibling-down">&nbsp;</span>';

	if (typeof(objExpanded) != 'undefined')
	{
		var i = window.location.href.lastIndexOf('/');
		var ary = window.location.href.substring(i+1).split('.');
		if (objExpanded[ary[0]])
			strLink = '<a class="sibling-down" href="' + ary[0] + '/default.html">expand</a>';
	}

	return strLink;
}
