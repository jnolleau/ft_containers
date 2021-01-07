/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   List.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julnolle <julnolle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/04 11:07:21 by julnolle          #+#    #+#             */
/*   Updated: 2021/01/07 18:54:29 by julnolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_HPP
# define LIST_HPP

# include <string>
# include <iostream>
# include <iterator>


namespace ft
{

template<typename T>
	struct		t_list
	{
		T		value;
		t_list	*prev;
		t_list	*next;
	};

template<typename T>
	class List
	{
		typedef	t_list<T>	t_list;

	private:

	/*Attributs*/
		t_list				*_list;
		unsigned int		_size;

		void	swap_values(T& val1, T& val2) {
			T tmp;
			tmp = val1;
			val1 = val2;
			val2 = tmp;
		}

		void	delete_node(t_list *node)
		{
			if (node->next == NULL)
				node->prev->next = NULL;
			else if (node->prev == NULL)
			{
				node->next->prev = NULL;
				this->_list = this->_list->next;
			}
			else
			{
				node->prev->next = node->next;
				node->next->prev = node->prev;
			}
			delete node;
			this->_size--;
		}

		t_list *front_ptr()
		{
			return this->_list;
		}

		t_list *back_ptr()
		{
			t_list *cpy = this->_list;

			while (cpy)
			{
				cpy = cpy->next;
			}
			return cpy;
		}

	public:
		List(void);
		List(unsigned int size, const T &val);

		// template<typename InputIterator>
		// List(InputIterator first, InputIterator last);

		List(const List<T> & copy);
		~List(void);

		List<T> & operator=(List<T> const & rhs);


	// iterator begin();
	// const_iterator begin() const;
	// iterator end();
	// const_iterator end() const;
	// reverse_iterator rbegin();
	// const_reverse_iterator rbegin() const;
	// reverse_iterator rend();
	// const_reverse_iterator rend() const;

		// t_list* getList(void) const;

		bool empty() const;
		unsigned int size() const;
		unsigned int max_size() const;
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;

		// template<typename InputIterator>
		// void assign (InputIterator first, InputIterator last);
		// void assign (unsigned int n, const T& val);

		void push_front (const T& val);
		void pop_front();
		void push_back (const T& val);
		void pop_back();
		
		// iterator insert (iterator position, const T& val);
		// void insert (iterator position, size_type n, const T& val);
		// template <class InputIterator>
		// void insert (iterator position, InputIterator first, InputIterator last);
		// iterator erase (iterator position);
		// iterator erase (iterator first, iterator last);

		void swap (List& x);
		void resize (size_t n, T val);
		void clear();
		
		// void splice (iterator position, list& x);
		// void splice (iterator position, list& x, iterator i);
		// void splice (iterator position, list& x, iterator first, iterator last);

		void remove (const T& val);

		template <class Predicate>
		void remove_if (Predicate pred);

		void unique();
		template <class BinaryPredicate>
		void unique (BinaryPredicate binary_pred);
		

		void merge (List& x);
		template <class Compare>
		void merge (List& x, Compare comp);

		void sort();
		
		template <class Compare>
		void sort (Compare comp);
		
		void reverse();

		void displayList() const;
		void displayReverse() const;


		class iterator : public std::iterator<std::input_iterator_tag, T> {

		private:
			t_list *p;

		public:
			iterator(void) {}
			iterator(t_list *x) : p(x) {}
			iterator(const iterator& copy) : p(copy.p) {}
			iterator& operator++() {p = p->next;return *this;}
			iterator operator++(int) {iterator tmp(*this); operator++(); return tmp;}
			iterator& operator--() {p = p->prev;return *this;}
			iterator operator--(int) {iterator tmp(*this); operator--(); return tmp;}
			bool operator==(const iterator& rhs) const {return p==rhs.p;}
			bool operator!=(const iterator& rhs) const {return p!=rhs.p;}
			T& operator*() {return p->value;}
			~iterator(void) {}
			iterator& operator=(iterator const & rhs) {
				*this = rhs;
				return (*this);
			}

		};

		iterator begin(void) { return iterator(this->front_ptr()); }
		iterator end(void) { return iterator((this->back_ptr())); }

	};

template<typename T>
	std::ostream & operator<<(std::ostream & o, List<T> const & rhs);
}

#include "List.ipp"

#endif // LIST_HPP