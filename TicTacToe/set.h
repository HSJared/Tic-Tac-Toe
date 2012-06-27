/****************************************************************************
*																			*
*	File:		set.h														*
*																			*
*	Author:		Robb T. Koether												*
*																			*
*	Date:		Aug 20, 2011												*
*																			*
*	Purpose:	This is the header file for the Set template class			*
*																			*
****************************************************************************/

#ifndef SET_H
#define SET_H

#include <iostream>
#include <string>
#include <cassert>
#include <cstdlib>

using namespace std;

/****************************************************************************
*																			*
*	The definition of the Set template class								*
*																			*
****************************************************************************/

template <int N>
class Set
{
//	Public member functions

	public:

	//	Constructors

		Set(unsigned int s = 0);
		int size() const;

	//	Inspectors

		bool isMember(int e) const;
		bool isEmpty() const;
		bool isUniversal() const;
		int getElement(int i) const;

	//	Mutators

		void makeEmpty();
		void makeUniversal();
		void input(istream& in);
		void output(ostream& out) const;
	
	//	Facilitators

		Set<N> unionx(const Set<N>& s) const;
		Set<N> intersection(const Set<N>& s) const;
		Set<N> difference(const Set<N>& s) const;
		Set<N> symmetricDifference(const Set<N>& s) const;
		Set<N> add(int e) const;
		Set<N> remove(int e) const;
		bool isEqual(const Set<N>& s) const;
		bool isSubsetOf(const Set<N>& s) const;

	//	Operators
	
		Set<N> operator-() const;				// Complement
		Set<N>& operator+=(const Set<N>& s);	// Union
		Set<N>& operator*=(const Set<N>& s);	// Intersection
		Set<N>& operator-=(const Set<N>& s);	// Difference
		Set<N>& operator^=(const Set<N>& s);	// Symmetric difference
		Set<N>& operator+=(int e);				// Add element
		Set<N>& operator-=(int e);				// Delete element
		int operator[](int i);
	
	//	Other member functions
	
		int randomMember() const;
		void validate() const;
		
//	Private data members

	private:
		unsigned int mask;
		unsigned int elements;
};

/************************************************************************
*																		*
*	Operator prototypes													*
*																		*
************************************************************************/

//	Input/Output operators

template <int N>
istream& operator>>(istream& in, Set<N>& s);
template <int N>
ostream& operator<<(ostream& out, const Set<N>& s);

//	Set operators

template <int N>
Set<N> operator+(const Set<N>& s, const Set<N>& t);
template <int N>
Set<N> operator*(const Set<N>& s, const Set<N>& t);
template <int N>
Set<N> operator-(const Set<N>& s, const Set<N>& t);
template <int N>
Set<N> operator^(const Set<N>& s, const Set<N>& t);
template <int N>
Set<N> operator+(const Set<N>& s, int e);
template <int N>
Set<N> operator+(int e, const Set<N>& s);
template <int N>
Set<N> operator-(const Set<N>& s, int e);

//	Relational operators

template <int N>
bool operator==(const Set<N>& s, const Set<N>& t);
template <int N>
bool operator!=(const Set<N>& s, const Set<N>& t);
template <int N>
bool operator<(const Set<N>& s, const Set<N>& t);
template <int N>
bool operator>(const Set<N>& s, const Set<N>& t);
template <int N>
bool operator<=(const Set<N>& s, const Set<N>& t);
template <int N>
bool operator>=(const Set<N>& s, const Set<N>& t);

/************************************************************************
*																		*
*	Function:	Set(unsigned int)										*
*																		*
*	Purpose:	To create a set containing the specified elements		*
*																		*
*	Note:		The bits of the unsigned int are numbered 0 - 31.  		*
*				Each bit represents an element.  The elements of the 	*
*				set are determined by the bits that equal 1.			*
*																		*
*	Example:	0x0000014f = 0000 0000 0000 0000 0000 0001 0100 1111	*
*																		*
*						   = {0, 1, 2, 3, 6, 8}							*
*																		*
************************************************************************/

template <int N>
Set<N>::Set(unsigned int s)
{
// Initialize random number generator for RandomMember() function

//	Create the universal set

	mask = 0;
	for (int i = 0; i < N; i++)
		mask |= (1 << i);

//	Add the elements to the set

	elements = mask & s;
	return;
}

/************************************************************************
*																		*
*	Function:	size													*
*																		*
*	Purpose:	To return the number of elements in the set				*
*																		*
************************************************************************/

template <int N>
int Set<N>::size() const
{
//	Count the elements in the set

	int size = 0;
	for (int i = 0; i < N; i++)
		if (isMember(i))
			size++;
	return size;
}

/************************************************************************
*																		*
*	Function:	isMember												*
*																		*
*	Purpose:	To determine whether the specified element is a member	*
*				of the set												*
*																		*
************************************************************************/

template <int N>
bool Set<N>::isMember(int e) const
{
	assert(e >= 0 && e < N);
	return (elements & (1 << e)) != 0;
}

/************************************************************************
*																		*
*	Function:	isEmpty													*
*																		*
*	Purpose:	To determine whether the set is empty					*
*																		*
************************************************************************/

template <int N>
bool Set<N>::isEmpty() const
{
	return elements == 0;
}

/************************************************************************
*																		*
*	Function:	getElement												*
*																		*
*	Purpose:	To get the i-th element of a set						*
*																		*
************************************************************************/

template <int N>
int Set<N>::getElement(int i) const
{
	assert(i >= 0 && i < size());
	int e = -1;
	int count = 0;
	while (count <= i && e < N)
	{
		e++;
		if (e < N && isMember(e))
			count++;
	}
	if (e < N)
		return e;
	else
		return -1;
}

/************************************************************************
*																		*
*	Function:	isUniversal												*
*																		*
*	Purpose:	To determine whether the set is the universal set		*
*																		*
*	Note:		The universal set consists of the elements 0 - N-1		*
*																		*
************************************************************************/

template <int N>
bool Set<N>::isUniversal() const
{
	return elements == mask;
}

/************************************************************************
*																		*
*	Function:	makeEmpty												*
*																		*
*	Purpose:	To make the set empty									*
*																		*
************************************************************************/

template <int N>
void Set<N>::makeEmpty()
{
	elements = 0;
	return;
}

/************************************************************************
*																		*
*	Function:	makeUniversal											*
*																		*
*	Purpose:	To make the set the universal set						*
*																		*
************************************************************************/

template <int N>
void Set<N>::makeUniversal()
{
	elements = mask;
	return;
}

/************************************************************************
*																		*
*	Function:	input													*
*																		*
*	Purpose:	To read a set from the specified input stream			*
*																		*
*	Note:		The set should be written in the form {e1, e2, ..., ek}	*
*				for some 0 <= k <= N									*
*																		*
************************************************************************/

template <int N>
void Set<N>::input(istream& in)
{
	elements = 0;

//	Read the open brace

	char c;
	in >> c;
	assert(c == '{');

//	Check for the close brace

	in >> c;
	if (c == '}') return;	// Set is the empty set
	
	in.putback(c);

//	Read the elements in the set

	int e;
	while (c != '}')
	{
		in >> e;
		elements |= (1 << e);
		in >> c;
		assert(c == ',' || c == '}');
	}
	return;
}

/************************************************************************
*																		*
*	Function:	output													*
*																		*
*	Purpose:	To write the set to the specified output stream			*
*																		*
************************************************************************/

template <int N>
void Set<N>::output(ostream& out) const
{
	out << '{';
	int i = 0;
	while (i < N && !isMember(i))
		i++;
	if (i < N)
		out << i;
	i++;
	while (i < N)
	{
		if (isMember(i))
			out << ", " << i;
		i++;
	}
	out << '}';
	return;
}

/************************************************************************
*																		*
*	Function:	union													*
*																		*
*	Purpose:	To find the union of the two sets						*
*																		*
*	Note:		Union(A, B) = {x | x is in A OR x is in B}				*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::unionx(const Set<N>& s) const
{
	return Set<N>(elements | s.elements);
}

/************************************************************************
*																		*
*	Function:	intersection											*
*																		*
*	Purpose:	To find the intersection of the two sets				*
*																		*
*	Note:		Intersection(A, B) = {x | x is in A AND x is in B}		*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::intersection(const Set<N>& s) const
{
	return Set<N>(elements & s.elements);
}

/************************************************************************
*																		*
*	Function:	difference												*
*																		*
*	Purpose:	To find the difference between the two sets				*
*																		*
*	Note:		Difference(A, B) = {x | x is in A BUT x is NOT in B}	*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::difference(const Set<N>& s) const
{
	return Set<N>(elements & ~s.elements);
}

/************************************************************************
*																		*
*	Function:	symmetricDifference										*
*																		*
*	Purpose:	To find the symmetric difference between the two sets	*
*																		*
*	Note:		SymmetricDifference(A, B) = (A - B) U (B - A)			*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::symmetricDifference(const Set<N>& s) const
{
	return Set<N>((elements & ~s.elements) | (~elements & s.elements));
}

/************************************************************************
*																		*
*	Function:	add														*
*																		*
*	Purpose:	To add the specified element to the set					*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::add(int e) const
{
	return Set<N>(elements | (1 << e));
}

/************************************************************************
*																		*
*	Function:	remove													*
*																		*
*	Purpose:	To remove the specified element from the set			*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::remove(int e) const
{
	return Set<N>(elements & ~(1 << e));
}

/************************************************************************
*																		*
*	Function:	isEqual													*
*																		*
*	Purpose:	To determine whether the set equals the specified set	*
*																		*
************************************************************************/

template <int N>
bool Set<N>::isEqual(const Set<N>& s) const
{
	return elements == s.elements;
}

/************************************************************************
*																		*
*	Function:	isSubsetOf												*
*																		*
*	Purpose:	To determine whether the set is a subset of the 		*
*				specified set											*
*																		*
************************************************************************/

template <int N>
bool Set<N>::isSubsetOf(const Set<N>& s) const
{
	return (elements & s.elements) == elements;
}

/************************************************************************
*																		*
*	Function:	randomMember											*
*																		*
*	Purpose:	To return a randomly selected member of the set			*
*																		*
*	Note:		The set must be non-empty								*
*																		*
************************************************************************/

template <int N>
int Set<N>::randomMember() const
{
//	Make sure set is nonempty

	assert(!isEmpty());

//	Select a random position

	int n = rand() % size();

	return getElement(n);
}

/************************************************************************
*																		*
*	Function:	operator>>												*
*																		*
*	Purpose:	To read a set from the specified input stream			*
*																		*
************************************************************************/

template <int N>
istream& operator>>(istream& in, Set<N>& s)
{
	s.input(in);
	return in;
}

/************************************************************************
*																		*
*	Function:	operator<<												*
*																		*
*	Purpose:	To write a set to the specified output stream			*
*																		*
************************************************************************/

template <int N>
ostream& operator<<(ostream& out, const Set<N>& s)
{
	s.output(out);
	return out;
}

/************************************************************************
*																		*
*	Function:	operator+(Set, Set)										*
*																		*
*	Purpose:	To find the union of the specified sets					*
*																		*
************************************************************************/

template <int N>
Set<N> operator+(const Set<N>& s, const Set<N>& t)
{
	return s.unionx(t);
}

/************************************************************************
*																		*
*	Function:	operator*												*
*																		*
*	Purpose:	To find the intersection of the specified sets			*
*																		*
************************************************************************/

template <int N>
Set<N> operator*(const Set<N>& s, const Set<N>& t)
{
	return s.intersection(t);
}

/************************************************************************
*																		*
*	Function:	operator-(Set, Set)										*
*																		*
*	Purpose:	To find the difference of the specified sets			*
*																		*
************************************************************************/

template <int N>
Set<N> operator-(const Set<N>& s, const Set<N>& t)
{
	return s.difference(t);
}

/************************************************************************
*																		*
*	Function:	operator^												*
*																		*
*	Purpose:	To find the symmetric difference of the specified sets	*
*																		*
************************************************************************/

template <int N>
Set<N> operator^(const Set<N>& s, const Set<N>& t)
{
	return s.symmetricDifference(t);
}

/************************************************************************
*																		*
*	Function:	operator-()												*
*																		*
*	Purpose:	To find the complement of a set							*
*																		*
************************************************************************/

template <int N>
Set<N> Set<N>::operator-() const
{
	return mask & ~elements;
}

/************************************************************************
*																		*
*	Function:	operator+(Set, int)										*
*																		*
*	Purpose:	To add the specified element to the specified set		*
*																		*
************************************************************************/

template <int N>
Set<N> operator+(const Set<N>& s, int e)
{
	assert(e >= 0 && e < N);
	return s.add(e);
}

/************************************************************************
*																		*
*	Function:	operator+(int, Set)										*
*																		*
*	Purpose:	To add the specified element to the specified set		*
*																		*
************************************************************************/

template <int N>
Set<N> operator+(int e, const Set<N>& s)
{
	return s + e;
}

/************************************************************************
*																		*
*	Function:	operator-(Set, int)										*
*																		*
*	Purpose:	To remove the specified element from the specified set	*
*																		*
************************************************************************/

template <int N>
Set<N> operator-(const Set<N>& s, int e)
{
	assert(e >= 0 && e < N);
	return s.remove(e);
}

/************************************************************************
*																		*
*	Function:	operator==												*
*																		*
*	Purpose:	To determine whether the specified sets are equal		*
*																		*
************************************************************************/

template <int N>
bool operator==(const Set<N>& s, const Set<N>& t)
{
	return s.isEqual(t);
}

/************************************************************************
*																		*
*	Function:	operator!=												*
*																		*
*	Purpose:	To determine whether the specified sets are unequal		*
*																		*
************************************************************************/

template <int N>
bool operator!=(const Set<N>& s, const Set<N>& t)
{
	return !s.isEqual(t);
}

/************************************************************************
*																		*
*	Function:	operator<												*
*																		*
*	Purpose:	To determine whether the first set is a proper subset	*
*				of the second set										*
*																		*
************************************************************************/

template <int N>
bool operator<(const Set<N>& s, const Set<N>& t)
{
	return s.isSubsetOf(t) && !s.isEqual(t);
}

/************************************************************************
*																		*
*	Function:	operator>												*
*																		*
*	Purpose:	To determine whether the second set is a proper subset	*
*				of the first set										*
*																		*
************************************************************************/

template <int N>
bool operator>(const Set<N>& s, const Set<N>& t)
{
	return t.isSubsetOf(s) && !t.isEqual(s);
}

/************************************************************************
*																		*
*	Function:	operator<=												*
*																		*
*	Purpose:	To determine whether the first set is a subset of the 	*
*				second set												*
*																		*
************************************************************************/

template <int N>
bool operator<=(const Set<N>& s, const Set<N>& t)
{
	return s.isSubsetOf(t);
}

/************************************************************************
*																		*
*	Function:	operator>=												*
*																		*
*	Purpose:	To determine whether the second set is a subset of the 	*
*				first set												*
*																		*
************************************************************************/

template <int N>
bool operator>=(const Set<N>& s, const Set<N>& t)
{
	return t.isSubsetOf(s);
}

/************************************************************************
*																		*
*	Function:	operator+=												*
*																		*
*	Purpose:	To add to the set the elements of the specified set		*
*																		*
************************************************************************/

template <int N>
Set<N>& Set<N>::operator+=(const Set<N>& s)
{
	elements |= s.elements;
	return *this;
}

/************************************************************************
*																		*
*	Function:	operator&=												*
*																		*
*	Purpose:	To remove from the set the elements that are not in		*
*				the specified set										*
*																		*
************************************************************************/

template <int N>
Set<N>& Set<N>::operator*=(const Set<N>& s)
{
	elements &= s.elements;
	return *this;
}

/************************************************************************
*																		*
*	Function:	operator-=												*
*																		*
*	Purpose:	To remove from the set the elements that are not in		*
*				the specified set										*
*																		*
************************************************************************/

template <int N>
Set<N>& Set<N>::operator-=(const Set<N>& s)
{
	elements &= ~s.elements;
	return *this;
}

/************************************************************************
*																		*
*	Function:	operator^=												*
*																		*
*	Purpose:	To remove from the set the elements that are not in 	*
*				both the set and the specified set						*
*																		*
************************************************************************/

template <int N>
Set<N>& Set<N>::operator^=(const Set<N>& s)
{
	elements ^= s.elements;
	return *this;
}

/************************************************************************
*																		*
*	Function:	operator+=												*
*																		*
*	Purpose:	To add the specified element to the set					*
*																		*
************************************************************************/

template <int N>
Set<N>& Set<N>::operator+=(int e)
{
	assert(e >= 0 && e < N);
	elements |= (1 << e);
	return *this;
}

/************************************************************************
*																		*
*	Function:	operator-=												*
*																		*
*	Purpose:	To remove the specified element from the set			*
*																		*
************************************************************************/

template <int N>
Set<N>& Set<N>::operator-=(int e)
{
	assert(e >= 0 && e < N);
	elements ^= (1 << e);
	return *this;
}

/************************************************************************
*																		*
*	Function:	operator[]												*
*																		*
*	Purpose:	To return the i-th element of a set. If the set			*
*				contains fewer than i elements, then -1 is returned		*
*																		*
************************************************************************/

template <int N>
int Set<N>::operator[](int i)
{
	return getElement(i);
}

#endif