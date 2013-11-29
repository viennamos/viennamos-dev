#ifndef DATABASE_HPP
#define DATABASE_HPP

/*
 *
 * Copyright (c) 2013, Institute for Microelectronics, TU Wien.
 *
 * This file is part of ViennaMOS     http://viennamos.sourceforge.net/
 *
 * Contact: Josef Weinbub             weinbub@iue.tuwien.ac.at
 *
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <map>
#include <exception>
#include <iostream>

#include "boost/shared_ptr.hpp"

#include <stdexcept>


class database_exception : public std::runtime_error
{
public:
  database_exception(std::string const & str) : std::runtime_error(str) {}
};

class DataBase
{

public:

  typedef std::string             Key;
  typedef std::map<Key, boost::shared_ptr<void> >    Storage;


  DataBase()
  {

  }

  template<typename T>
  boost::shared_ptr<T> insert(Key key, boost::shared_ptr<T> value)
  {
    if(this->has_key(key))
    {
      throw database_exception("Could not insert new data the same key is already used!");
    }
    storage.insert(std::make_pair(key, value));
    return this->at<boost::shared_ptr<T> >(key);
  }

  template<typename SharedPtrT>
  SharedPtrT at(Key key)
  {
    if(!this->has_key(key))
    {
      throw database_exception("Could not find element!");
    }
    return boost::static_pointer_cast<typename SharedPtrT::element_type>(storage.at(key));
  }

  template<typename T>
  void erase(Key key)
  {
    if(this->has_key(key))
    {
      storage.erase(key); // smart pointer takes care of 'value'
    }
  }

  bool has_key(Key key)
  {
    return !(storage.find(key) == storage.end());
  }

  std::size_t size()
  {
    return storage.size();
  }

  bool empty()
  {
    return storage.empty();
  }


private:
  Storage    storage;

};


#endif // DATABASE_HPP


