/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: julnolle <julnolle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/02 10:29:50 by julnolle          #+#    #+#             */
/*   Updated: 2021/02/10 18:37:20 by julnolle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MAP_HPP
# define MAP_HPP

# include <string>
# include <iostream>
# include <cstddef>
# include <exception>
# include <sstream>
# include <cstring>
# include "integral_traits.hpp"
# include "functional.hpp"
# include "Map_iterator.hpp"

# define LEFT	-1
# define RIGHT	1
# define THIS	0

namespace ft {

template<typename Key, typename T, typename Compare = std::less<Key> >
class map {

public:

	typedef Key										key_type;
	typedef T										mapped_type;
	// typedef Tree_node<key_type, mapped_type>		node_type;
	typedef std::pair<const Key, T>					value_type;
	typedef Tree_node<value_type>					node_type;
	typedef Compare									key_compare;
	typedef Map_iterator<value_type>				iterator;
	typedef Map_iterator<const value_type>			const_iterator;
	typedef std::reverse_iterator<const_iterator>	const_reverse_iterator;
	typedef std::reverse_iterator<iterator>			reverse_iterator;
	typedef	size_t									size_type;
	typedef	ptrdiff_t								difference_type;

	class value_compare
	{	// in C++98, it is required to inherit binary_function<value_type,value_type,bool>
		friend class map;
		
		protected:
			Compare comp;
	 		 value_compare (Compare c) : comp(c) {}  // constructed with map's comparison object
		public:
			bool operator() (const value_type& x, const value_type& y) const
			{
				return comp(x.first, y.first);
			}
	};
	

	explicit map (const key_compare& comp = key_compare()) : _root(NULL), _size(0), _comp(comp)
	{}

	template <class InputIterator>
	map (InputIterator first, InputIterator last, const key_compare& comp = key_compare());

	map (const map& x);

	map& operator=(map const & x);
	~map () {}

	iterator begin() { return iterator(LeftMost(this->_root)); }
	const_iterator begin() const { return const_iterator(LeftMost(this->_root)); }

	iterator end() { return iterator(); }
	const_iterator end() const { return const_iterator(); }

/*	
	reverse_iterator rbegin(void) { return reverse_iterator(this->end()); }
	const_reverse_iterator rbegin(void) const { return const_reverse_iterator(this->end()); }

	reverse_iterator rend(void) { return reverse_iterator(this->begin()); }
	const_reverse_iterator rend(void) const { return const_reverse_iterator(this->begin()); }
*/
	bool empty() const
	{
		return (this->_root == NULL);
	}

	size_type size() const
	{
		return (this->_size);
	}

	size_type max_size() const;
	mapped_type& operator[] (const key_type& k);

	// std::pair<iterator,bool> insert (const value_type& val);
	iterator insert (iterator position, const value_type& val);
	template <class InputIterator>
	void insert (InputIterator first, InputIterator last);

	void erase (iterator position);
	// size_type erase (const key_type& k);
	void erase (iterator first, iterator last);

	void swap (map& x);
	void clear ()
	{
		deleteRecurse(this->_root);
		this->_size = 0;
	}

	key_compare key_comp() const;

	value_compare value_comp() const;

	// iterator find (const key_type& k);
	const_iterator find (const key_type& k) const;
	size_type count (const key_type& k) const;
	iterator lower_bound (const key_type& k);
	const_iterator lower_bound (const key_type& k) const;
	iterator upper_bound (const key_type& k);
	const_iterator upper_bound (const key_type& k) const;

	std::pair<const_iterator,const_iterator>	equal_range (const key_type& k) const;
	std::pair<iterator,iterator>				equal_range (const key_type& k);

	void displayMap()
	{
		printInorder (this->_root);
	}

	void printInorder(node_type *node)
	{
		if (node == NULL)
			return ;
		printInorder(node->left);
		// std::cout << "this->root: " << this->_root->value.first << std::endl;
		if (node->side == 'h')
			std::cout << "map[" << node->value.first << "] = " << node->value.second << " (HEAD)" << std::endl;
		else
			std::cout << "map[" << node->value.first << "] = " << node->value.second << " (" << node->side << ")" << std::endl;
		printInorder(node->right);
	}

	void insert (const value_type& val)
	{
		node_type *n = new node_type(val); // for typename Pair
		// node_type *n = new node_type(val.first, val.second);

		if (this->_root == NULL)
		{
			this->_root = n;
			++this->_size;
			return ;
		}
		insertRecurse(this->_root, n);
		++this->_size;
	}

	void insertRecurse(node_type *node, node_type *n)
	{	
  		// Descente récursive dans l'arbre jusqu'à atteindre une feuille
		if (node != NULL && this->_comp(n->value.first, node->value.first)) {
			if (node->left != NULL) {
				insertRecurse(node->left, n);
				return;
			}
			else
			{
				n->side = 'L';
				n->parent = node;
				node->left = n;
			}
		} 
		else if (node != NULL) {
			if (node->right != NULL) {
				insertRecurse(node->right, n);
				return;
			}
			else
			{
				n->side = 'R';
				n->parent = node;
				node->right = n;
			}
		}
	}

	void deleteRecurse(node_type *&node)
	{
		if (node)
		{
			if (node->left)
				deleteRecurse(node->left);
			if (node->right)
				deleteRecurse(node->right);
		// std::cout << "Deleting: " << node->key << std::endl;
			delete node;
			node = NULL;
		}
	}

	bool find (const key_type& k)
	{
		if (this->_root == NULL)
			return (false);
		return findRecurse(this->_root, k);
	}

	bool findRecurse(node_type *node, const key_type& k)
	{
		// std::cerr << "FIND" << std::endl;
		if (node)
		{
			// std::cerr << "key: " << node->key << std::endl;
			if (this->_comp(k, node->value.first))
				return findRecurse(node->left, k);
			else if (this->_comp(node->value.first, k))
				return findRecurse(node->right, k);
			else
				return (true);
		}
		return (false);
	}

	void erase (const key_type& k)
	{
		if (this->_root == NULL)
			return ;
		eraseRecurse(this->_root, k);
	}
	
	node_type* RightMost(node_type* node)
	{
		// node_type *cpy = node;
		while (node->right)
		{
			node = node->right;
		}
		return node;
	}

	node_type* LeftMost(node_type* node)
	{
		// node_type *cpy = node;
		while (node->left)
		{
			node = node->left;
		}
		return node;
	}
	
	node_type* searchRightest(node_type* node, node_type* &parent = NULL)
	{
		// node_type *cpy = node;
		while (node->right)
		{
			if (parent)
				parent = node;
			node = node->right;
		}
		return node;
	}

	node_type* searchLeftest(node_type* node, node_type* &parent = NULL)
	{
		// node_type *cpy = node;
		while (node->left)
		{
			if (parent)
				parent = node;
			node = node->left;
		}
		return node;
	}

	void eraseRecurse(node_type*& node, const key_type& k)
	{
		if (node == NULL)
			return;
		if (node->value.first == k)
		{
			if (node->left == NULL && node->right == NULL)
			{
				delete node;
				node = NULL;
				--this->_size;
				return ;
			}
			else if (node->left == NULL)
			{
				this->delete_node(node, node->right);
				return ;
			}
			else if (node->right == NULL)
			{
				this->delete_node(node, node->left);
				return ;
			}
			else
			{
				node_type *last_parent = node;
				node_type* min = searchRightest(node->left, last_parent);				
				if (node == this->_root) // FOR DEBUG
					min->side = 'h';	//
				
				if (last_parent != node)
				{
					last_parent->right = NULL;
					min->left = node->left;
				}
				min->right = node->right;
				node->right->parent = min;
				node->left->parent = min;
				
				this->delete_node(node, min);
				return ;
			}
		}
		else if (k < node->value.first)
			eraseRecurse(node->left, k);
		else
			eraseRecurse(node->right, k);
	}

private:
	node_type	*_root;
	size_type	_size;
	key_compare	_comp;

	void delete_node(node_type*& node, node_type* &child)
	{
		child->parent = node->parent;
		delete node;
		node = child;
		--this->_size;
	}
};

// std::ostream & operator<<(std::ostream & o, map const & rhs);

#endif // MAP_HPP


} // ft