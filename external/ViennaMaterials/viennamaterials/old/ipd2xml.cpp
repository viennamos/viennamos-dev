/* =======================================================================
   Copyright (c) 1992-2010, Institute for Microelectronics, TU Vienna.
   http://www.iue.tuwien.ac.at
                                 ViennaMAT
                             -----------------
                             
   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
   license:    see ViennaMAT/LICENSE
   ======================================================================= */

#include <iostream>
#include <vector>

#include <boost/lexical_cast.hpp>

#include "ipd.h"
#include "tinyxml.h"


//#define CONVOUTPUT
//
// ----------------------------------------------------------
//
struct xmlwriter 
{
   xmlwriter(const char* docid)
   {
      root_tag             = "Materials";
      identifier_tag       = "identifier";
      element_tag          = "element";
      data_tag             = "data";   
      representation_tag   = "representation";      
      double_tag           = "double";         
   
      TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );  
      doc.LinkEndChild( decl );  
      
      TiXmlElement * root = new TiXmlElement( root_tag );  
      doc.LinkEndChild( root );  
      
      TiXmlElement * rootid = new TiXmlElement( identifier_tag );  
      rootid->LinkEndChild( new TiXmlText( docid ));  
      root->LinkEndChild( rootid );  	   
      
      currentnode = root;
      nodecont.push_back( root );
   }
   
   void add_section(const char* tag)
   {
      //std::cout << "# add_section: " << tag << std::endl;

      TiXmlElement * element = new TiXmlElement( element_tag );  
      currentnode->LinkEndChild( element );  
      
      TiXmlElement * elementid = new TiXmlElement( identifier_tag );  
      elementid->LinkEndChild( new TiXmlText( tag ));  
      element->LinkEndChild( elementid );  	   
      
      currentnode = element;
      nodecont.push_back( element );
   }
   
   void add_variable(const char* tag, const char* value)
   {
      //std::cout << "# add_variable: " << tag << " : " << value << std::endl;
      
      TiXmlElement * data = new TiXmlElement( data_tag );  
      currentnode->LinkEndChild( data );        

      TiXmlElement * dataid = new TiXmlElement( identifier_tag );  
      dataid->LinkEndChild( new TiXmlText( tag ));  
      data->LinkEndChild( dataid );  	         
      
      TiXmlElement * repres = new TiXmlElement( representation_tag );  
      data->LinkEndChild( repres );  	               

      TiXmlElement * double_ = new TiXmlElement( double_tag );  
      double_->LinkEndChild( new TiXmlText( value ));  
      repres->LinkEndChild( double_ );  	               
   }   
   
   void update()
   {
      nodecont.pop_back();
      currentnode = nodecont[nodecont.size()-1];
   }
   
   void print(std::string filename)
   {
      this->print(filename.c_str());
   }
   
   void print(const char* filename)
   {
      doc.SaveFile( filename );  
   }   
   
   TiXmlDocument doc;  
   
   TiXmlElement * currentnode;
   
   const char* root_tag;
   const char* identifier_tag;
   const char* element_tag;   
   const char* data_tag;
   const char* representation_tag;
   const char* double_tag;
   
   std::vector< TiXmlElement * > nodecont;
};
//
// ----------------------------------------------------------
//
template < typename T >
std::string converter(T t)
{
   std::stringstream ss;
   ss << t;
   return ss.str();
   
//   std::string retval("");
//   try{
//      retval = boost::lexical_cast<std::string>(t);
//   }
//   catch(boost::bad_lexical_cast &)
//   {
//      //std::cout << " ## boost::lexical_cast can't cast: " << t << std::endl;      
//   }   
//   return retval;
}
//
// ----------------------------------------------------------
//
const char* access_ipd_value(const char* name, ipdTreeNode_t *tn)
{
   int l, b, v, len, dim;
   int *rlen;
   double re, im, *matrix;
   char *unit, *str;
   const char* munit;

   str = unit = NULL;

   std::string retval(""); 

   //std::cout << name 
   //          << " - valtype: " << ipdGetValueType(tn) 
   //          << " - type: " << ipdGetType(tn) << std::endl;

   switch (tn->type)
   {
      case ipdINTEGER:
         v = ipdGetIntegerByName(name, &l);
      #ifdef CONVOUTPUT
         std::cout << "int: " << l << "  vs.  " << converter(l) << std::endl;
      #endif
         if(v) retval = converter(l);
         else std::cout << "error @ ipdINTEGER - name: " << name << std::endl;      
         break;
      case ipdREAL:
         v = ipdGetRealByName(name, &re);
      #ifdef CONVOUTPUT
         std::cout << "real: " << re << "  vs.  " << converter(re) << std::endl;
      #endif
         if(v) retval = converter(re);
         else std::cout << "error @ ipdREAL - name: " << name << std::endl;      
         break;
      case ipdCOMPLEX:
         v = ipdGetComplexByName(name, &re, &im);
      #ifdef CONVOUTPUT
         std::cout << "complex: " << re << " " << im << "  vs.  " << converter(re) << " " << converter(im) << std::endl;
      #endif 
         if(v) retval = converter(re) + " i" + converter(im);         
         else std::cout << "error @ ipdCOMPLEX - name: " << name << std::endl;      
         break;
      case ipdREALQUANTITY:
         v = ipdGetRealQuantityByName(name, &re, &unit);
      #ifdef CONVOUTPUT
         std::cout << "real quan: " << re << " " << unit << "  vs.  " << converter(re) << " " << converter(unit) << std::endl;
      #endif 
         if(v && (unit)) retval = converter(re) + " " + converter(unit);                  
         else std::cout << "error @ ipdREALQUANTITY - name: " << name << std::endl;      
         break;
      case ipdCOMPLEXQUANTITY:
         v = ipdGetComplexQuantityByName(name, &re, &im, &unit);
      #ifdef CONVOUTPUT
         std::cout << "real comp quan: " << re << " " << im << " " << unit << "  vs.  " << converter(re) << " " << converter(im) << " " << converter(unit) << std::endl;
      #endif 
         if(v) retval = converter(re) + " i" + converter(im) + " " + converter(unit); 
         else std::cout << "error @ ipdCOMPLEXQUANTITY - name: " << name << std::endl;      
         break;
      case ipdSTRING:
         v = ipdGetStringByName(name, &str);
      #ifdef CONVOUTPUT
         std::cout << "string: " << str << "  vs.  " << converter(str) << std::endl;
      #endif
         if(v) 
         {
            if(str)
               retval = converter(str);       
         }
         else std::cout << "error @ ipdSTRING - name: " << name << std::endl;         
         break;
      case ipdBOOLEAN:
         v = ipdGetBooleanByName(name, &b);
      #ifdef CONVOUTPUT
         std::cout << "bool: " << b << "  vs.  " << converter(b) << std::endl;
      #endif
         if(v) retval = converter(b);           
         else std::cout << "error @ ipdBOOLEAN - name: " << name << std::endl;      
         break;   
         
      case 192: // array
         v = ipdGetSloppyRealMatrixByName(name, &dim, &rlen, &matrix, &munit);
         if(v)
         {
            len = 1;
            int i;
            for (i = 0; i < dim; i++)
               len = len * rlen[i];
            retval = "[ ";
            for (i = 0; i < len; i++)
               retval += converter(matrix[i]) + " ";
            std::stringstream ss;
            ss << munit;
            retval += "] " + ss.str();
            
            //free(rlen);
            free(matrix);
         }
         else std::cout << "error @ 192 - name: " << name << std::endl;      
         break;
         
      case 2080: // concatenated strings ..
         //std::cout << "name-2080: " << name << " type: " << tn->type << std::endl;
         v = ipdGetStringByName(name, &str);
         if(v && (str)) retval = converter(str);        
         else std::cout << "error @ 2080 - name: " << name << std::endl;      
         break;
         
      case 4098: // rationals .. will be evaluated to floats
         //std::cout << "name-4098: " << name << " type: " << tn->type << std::endl;
         v = ipdGetRealByName(name, &re);
         if(v) retval = converter(re);
         else std::cout << "error @ 4098 - name: " << name << std::endl;      
         break;

      case 4106: // 
         //std::cout << "name-4106: " << name << " type: " << tn->type << std::endl;
         v = ipdGetRealQuantityByName(name, &re, &unit);
         if(v && (unit)) retval = converter(re) + " " + converter(unit);         
         else std::cout << "error @ 4106 - name: " << name << std::endl;      
         break;         
      
      default:         
          
         std::cout << "error @ ipd db access - node type not recognized: "<< name 
                   << " - type: " << ipdGetType(tn) << std::endl;
         retval = "not-accessible";
         break;
   }
   
   return retval.c_str();
}
//
// ----------------------------------------------------------
//
void recursive_traverse(ipdIterator_t * iNode, xmlwriter & xmldoc)
{
   // Traverse the ViennaIPD datastructure using the iterator
   while (ipdIteratorIsValid(iNode))
   {
      // Get the name of the current item
      ipdConstString itemName = ipdIteratorGetItemName(iNode);
   
      // If the current element is a _variable_ 
      if (ipdIteratorGetType(iNode) == ipdVARIABLE)
      {
         // Print the name of the variable
         //printf("variable: %s\n", itemName);
         
         std::string valuestring(""); 

         valuestring = access_ipd_value(iNode->tn->node.sv.name, ipdIteratorEval(iNode));

         xmldoc.add_variable( itemName, valuestring.c_str() );
      }
      
      // If the current element is a _section_
      else if (ipdIteratorGetType(iNode) == ipdSECTION)
      {
         xmldoc.add_section( itemName );

         // Print the name of the section
         //printf("section: %s\n", itemName);

         // Create a new iterator which should traverse the subsection
         ipdIterator_t  *iSubNode = NULL;

         // Set the iterator to origin at this particular section
         ipdIteratorNewByName(&iSubNode, itemName, ipdANY, ipdANY);
         
         // Step into the subsection
         ipdIteratorDoStep(iSubNode);     
         
         recursive_traverse(iSubNode, xmldoc);             
         
         xmldoc.update();
         
      }      
      
      // Next item
      ipdIteratorDoNext(iNode); 
      
   }
}
//
// ----------------------------------------------------------
//



void playground()
{
   
   
//   doc.append_child().set_name("mass");
//   pugi::xml_node node = doc.child("mass");

//   pugi::xml_node module = node.append_child();
//   module.set_name("module");   
//   
//   module.append_child(pugi::node_pcdata).set_value("Module A description");
   
   
}
//
// ----------------------------------------------------------
//

int main(int argc, char** argv)
{
   const char* inputfile_ipd = argv[1];
   const char* outputfile_xml = argv[2];

   xmlwriter xmldoc("modelipd");
   
   // Initialize the Database. 
   // Normally we can use the default values, so we don't need parameters. 
   ipdInit(NULL, NULL);
   
   // Let's create a database 
   ipdCreateBase("NameOfTheDatabase", 0);

   // Read a file in the database 
   ipdReadInputDeck(inputfile_ipd);
   
   // Define and initialize a ViennaIPD iterator
   ipdIterator_t  *iNode = NULL;   

   // Create a new iterator which operates on the root level 
   ipdIteratorNewAtRootSection(&iNode, ipdANY);
   
   ipdIteratorDoStep(iNode);     
   
   recursive_traverse(iNode, xmldoc);
   
   // Free the iterator
   ipdIteratorFree(iNode);
   
   // Free the ViennaIPD datastructures
   ipdFreeAll();
   
   xmldoc.print(outputfile_xml);

   return 0;
}
