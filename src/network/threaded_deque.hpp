#pragma once
#ifndef THREADED_DEQUE_HPP
#define THREADED_DEQUE_HPP

#include <deque>

#ifdef _MSVC_LANG
	#define CPP_VERSION _MSVC_LANG
#else // ^^^ defined( _MSVC_LANG ) / !defined( _MSVC_LANG ) vvv
	#define CPP_VERSION __cplusplus
#endif

template < class _Ty, class _Alloc = std::allocator< _Ty > >
class threaded_deque : protected std::deque< _Ty, _Alloc >
{
private:
	/* data */
public:
#if CPP_VERSION < 201103L
	explicit deque( const Allocator& alloc = Allocator() );
	explicit deque( size_type count, const T& value = T(),
                const Allocator& alloc = Allocator() );
	
	deque() : deque( Allocator() ) {}
	explicit deque( const Allocator& alloc );
	explicit deque( size_type count,
                const Allocator& alloc = Allocator() );
	deque( size_type count, const T& value,
       const Allocator& alloc = Allocator() );
	template< class InputIt >
	deque( InputIt first, InputIt last,
       const Allocator& alloc = Allocator() );
	template< container-compatible-range<T> R >
	deque( std::from_range_t, R&& rg,
       const Allocator& alloc = Allocator() );
	deque( const deque& other );
	deque( deque&& other );
	deque( const deque& other, const Allocator& alloc );
	deque( const deque& other,
       const std::type_identity_t<Allocator>& alloc );
	deque( deque&& other, const Allocator& alloc );
	deque( deque&& other, const std::type_identity_t<Allocator>& alloc );
	deque( std::initializer_list<T> init,
       const Allocator& alloc = Allocator() );
};

#endif // THREADED_DEQUE_HPP