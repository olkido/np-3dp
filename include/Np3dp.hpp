//
//  Np3dp.hpp
//  np-3dp
//
//  Created by Olga Diamanti on 12.05.21.
//

#ifndef Np3dp_hpp
#define Np3dp_hpp

#include <string>
#include <Eigen/Core>

#include "Mesh.h"

class Np3dp
{
public:
  Np3dp();
  

  // These are to showcase the menu box (imgui) capacity. See below.
  enum Method { Stripe=0, GeodFolie };
  Method method = Stripe;

  
  // These are to showcase the menu box (imgui) capacity. See below.
  enum VFType { Radial=0, LibDirectional };
  VFType vftype = Radial;
  
  Eigen::MatrixXd V;
  Eigen::MatrixXi F;
  double avg_edge_length;
  
  bool read(const std::string &filename);

  DDG::Mesh stripe_mesh;
  void invalidate();
  
  bool initialize_vector_field();
  bool has_vf;
  //per vertex vector field
  Eigen::MatrixXd vvf;
  //per face vector field
  Eigen::MatrixXd fvf;

  bool has_ac;
  //per vertex angular coordinate
  Eigen::MatrixXd vac;
  bool compute_angular_coordinate();
  
};

#endif /* Mesh_hpp */
