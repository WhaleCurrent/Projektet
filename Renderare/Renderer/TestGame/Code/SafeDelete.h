#pragma once

template< class T > void SafeDelete(T*& pVal)
{
	delete pVal;
	pVal = nullptr;
}

template< class T > void SafeDeleteArray(T*& pVal)
{
	delete[] pVal;
	pVal = nullptr;
}