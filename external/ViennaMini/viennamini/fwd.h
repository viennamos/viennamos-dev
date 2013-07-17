#ifndef VIENNAMINI_FORWARDS_H
#define VIENNAMINI_FORWARDS_H

/** Forward declarations */


namespace viennamini
{

  //
  // TODO: Think about where to move the following keys.
  //

  struct permittivity_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(permittivity_key const & ) const { return false; }
  };

  struct mobility_electrons_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(mobility_electrons_key const & ) const { return false; }
  };
  
  struct mobility_holes_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(mobility_holes_key const & ) const { return false; }
  };  

  struct builtin_potential_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(builtin_potential_key const & ) const { return false; }
  };

  // N_D
  struct donator_doping_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(donator_doping_key const & ) const { return false; }
  };

  // N_A
  struct acceptor_doping_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(acceptor_doping_key const & ) const { return false; }
  };

  struct oxide_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(oxide_key const & ) const { return false; }
  };
  
  struct semiconductor_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(semiconductor_key const & ) const { return false; }
  };
  
  struct contact_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(contact_key const & ) const { return false; }
  };  
  
  struct material_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(material_key const & ) const { return false; }
  };    

  struct potential_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(potential_key const & ) const { return false; }
  };    

  struct electron_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(electron_key const & ) const { return false; }
  };    

  struct hole_key
  {
    // Operator< is required for compatibility with std::map
    bool operator<(hole_key const & ) const { return false; }
  };        
}

#endif
