/*******************************************************************************
 * \file    smartptr.hpp
 * \brief   Functions used by the smart_ptr and smartref_ptr templates
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

#ifndef SMARTPTR_HPP
#define SMARTPTR_HPP

namespace org_biologicinstitute_stylus
{
	/**
	 * \brief Function template to free a pointer using \c delete
	 * \param[in] _Tp Object type to free
	 */
	template<typename _Tp> void __delete(_Tp* p) { ::delete p; }
	
	/**
	 * \brief A smart pointer that allows user-selected free routines
	 * \param[in] _Tp Object type to protect
	 * \param[in] _Fpt Routine used to free the object (defaults to \c delete)
	 *
	 * This template is modeled on the auto_ptr template from the C++
	 * Standard Template Library. It adds the ability for the user to
	 * specify the routine for freeing the managed object (rather than
	 * assuming \c delete). The freeing routine must take a pointer to
	 * the object as a single argument and return \c void.
	 */
	template<typename _Tp, void (*_Fpt)(_Tp*)  = __delete<_Tp> >
	class smart_ptr
	{
	private:
		_Tp* _M_ptr;

	public:
		explicit
		smart_ptr(_Tp* __p = 0) : _M_ptr(__p) {}
		smart_ptr(smart_ptr& __a) : _M_ptr(__a.release()) {}

		smart_ptr&
		operator=(_Tp* __p)
		{
			reset(__p);
			return *this;
		}

		smart_ptr&
		operator=(smart_ptr& __a)
		{
			reset(__a.release());
			return *this;
		}

		~smart_ptr() { reset(); }

		_Tp& operator*() const { return *_M_ptr; }
		_Tp* operator->() const { return _M_ptr; }
		_Tp** operator&() { release(); return &_M_ptr; }

		_Tp* get() const { return _M_ptr; }

		_Tp*
		release()
		{
			_Tp* __tmp = _M_ptr;
			_M_ptr = 0;
			return __tmp;
		}

		void
		reset(_Tp* __p = 0)
		{
			if (__p != _M_ptr)
			{
				if (_M_ptr != 0)
					(*_Fpt)(_M_ptr);
				_M_ptr = __p;
			}
		}
	};

	/**
	 * \brief A referenced-counted smart pointer that allows user-selected free routines
	 * \param[in] _Tp Object type to protect
	 * \param[in] _Fpt Routine used to free the object (defaults to \c delete)
	 *
	 * This template is similar to smart_ptr above, but implements reference semantics
	 * making it useful for STL containers.
	 *
	 * \remarks
	 * - If no free routine is provided, \c delete is used
	 * - This class is based on that described in "The C++ Standard Library", by Nicolai Josuttis,
	 *   published by Addison-Wesley, Copyright 1999. See section 6.8, page 222 and following.
	 */
	template<typename _Tp, void (*_Fpt)(_Tp*) = __delete<_Tp> >
	class smartref_ptr
	{
	private:
		_Tp* _M_ptr;
		long* _pcReferences;

		void
		reset(_Tp* __p=0, long* __pcReferences=0)
		{
			if (_pcReferences != 0 && --*_pcReferences == 0)
			{
				if (_M_ptr != 0)
					(*_Fpt)(_M_ptr);
			}
			_M_ptr = __p;
			_pcReferences = __pcReferences;
		}

	public:
		explicit
		smartref_ptr(_Tp* __p = 0) : _M_ptr(0), _pcReferences(0) { operator=(__p); }

		smartref_ptr(const smartref_ptr<_Tp>& __a) : _M_ptr(0), _pcReferences(0)  { operator=(__a); }

		smartref_ptr<_Tp>&
		operator=(_Tp* __p)
		{
			if (_M_ptr != __p)
				reset(__p, ::new long(1));
			return *this;
		}

		smartref_ptr<_Tp>&
		operator=(const smartref_ptr<_Tp>& __a)
		{
			if (_M_ptr != __a._M_ptr)
			{
				reset(__a._M_ptr, __a._pcReferences);
				++*_pcReferences;
			}
			return *this;
		}

		~smartref_ptr()	{ reset(); }
		
		operator _Tp* () const { return _M_ptr; }

		_Tp& operator*() const { return *_M_ptr; }
		_Tp* operator->() const { return _M_ptr; }

		_Tp* get() const { return _M_ptr; }
	};

}	// namespace org_biologicinstitute_stylus

#endif // SMARTPTR_HPP
