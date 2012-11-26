/*
  Copyright © 2012 Jesse 'Jeaye' Wilkerson
  See licensing at:
    http://opensource.org/licenses/BSD-3-Clause

  File: JsonParser.h
  Author: Jesse 'Jeaye' Wilkerson
*/

#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <fstream>

#if !JEAYESON_STD_FSTREAM_LOAD
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#endif

#include "Defines.h"

namespace JeayeSON
{
  class Parser
  {
    public:
      template <typename T>
      static T loadFile(std::string const &jsonFile)
      {
        std::string json;

#if JEAYESON_STD_FSTREAM_LOAD
        std::ifstream file(jsonFile.c_str());
        std::size_t fileSize = 0;

        /* Ensure the file was opened. */
        if(file.is_open() == false)
          return T();

        /* Determine the file length. */
        file.seekg(0, std::ios_base::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios_base::beg);

        /* Reserve space in the string. */
        json.reserve(fileSize);

        /* Read the file. */
        std::getline(file, json, static_cast<char>(-1)); // EOF
        file.close();
#else
        boost::interprocess::file_mapping file(jsonFile.c_str(), boost::interprocess::read_only);
        boost::interprocess::mapped_region region(file, boost::interprocess::read_only);

        char *str = static_cast<char*>(region.get_address());
        std::size_t elements = region.get_size() / sizeof(char);

        /* Reserve space for the string. */
        json.reserve(elements);

        /* Copy over the data. */
        json.assign(str, elements);
#endif

        /* Load the file normally. */
        return load<T>(json);
      }

      template <typename T>
      static T load(std::string const &jsonString)
      {
        return T();
      }
  };
} /* Namespace JeayeSON */

#endif /* JSONPARSER_H */
