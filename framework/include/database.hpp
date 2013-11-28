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

class DataBase
{

public:

  typedef std::string             Key;
  typedef std::map<Key, void*>    Storage;


  DataBase()
  {

  }

  template<typename T>
  T insert(Key key, T value)
  {
    if(this->has_entry(key))
    {
      std::cerr << "[DataBase][Error] Could not insert new element as it's already stored" << std::endl;
      return value;
    }
    storage.insert(std::pair<Key, T>(key, value));
    return this->value<T>(key);
  }

  template<typename T>
  T value(Key key)
  {
    if(!this->has_entry(key))
    {
      std::cerr << "[DataBase][Error] Could find element" << std::endl;
      return NULL;
    }
    return static_cast<T>(storage.at(key));
  }

  template<typename T>
  void delete_entry(Key key)
  {
    if(this->has_entry(key))
    {
      delete this->value<T>(key);
      storage.erase(key);
    }
  }

  bool has_entry(Key key)
  {
    if(storage.find(key) == storage.end()) return false;
    else return true;
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


