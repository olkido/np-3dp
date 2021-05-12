//
//  Mesh.cpp
//  np-3dp
//
//  Created by Olga Diamanti on 12.05.21.
//

#include "Np3dp.hpp"
#include <igl/read_triangle_mesh.h>
#include <igl/avg_edge_length.h>

Np3dp::Np3dp()
:
has_vf(false),
has_ac(false)
{
  
};

bool Np3dp::read(const std::string &filename)
{
  bool ret =  igl::read_triangle_mesh(filename.c_str(), V, F);
  if (!ret)
    return false;
  stripe_mesh.read( filename.c_str() );
  avg_edge_length = igl::avg_edge_length(V, F);
  return true;
  
}

void Np3dp::invalidate()
{
  has_vf = false;
  fvf.setZero(0,0);
  vvf.setZero(0,0);

  has_ac = false;
  vac.setZero(0,0);
  //todo: what other values do we need to reset?
}

bool Np3dp::initialize_vector_field()
{
  invalidate();
  switch (method)
  {
    case Stripe:
    {
      fvf.setZero(0,0);
      vvf.setZero(V.rows(),3);
      switch (vftype)
      {
        case Radial:
        {
          fvf.setZero(0,0);
          vvf.setZero(V.rows(),3);
          stripe_mesh.assignRadialValue();
          double k = stripe_mesh.fieldDegree;
         
          for( DDG::VertexCIter v = stripe_mesh.vertices.begin();
              v != stripe_mesh.vertices.end();
              v ++ )
          {
            for( double n = 0; n < k; n++ )
            {
              DDG::Vector Z = v->fieldVector( k, n ).unit();
              vvf.row(v->index)<<Z.x, Z.y, Z.z;
            }
          }
          has_vf = true;
          break;
        };
        case LibDirectional:
        {
          break;
        };
        default:
          break;
      }
      
      break;
    };
    case GeodFolie:
    {
      vvf.setZero(0,0);
      fvf.setZero(F.rows(),3);
      switch (vftype)
      {
        case Radial:
        {
          //todo : assign radial per face vector field
          break;
        }
        case LibDirectional:
        {
          break;
        }
        default:
          break;
      }
      break;
    };
      
    default:
      break;
  }

  return has_vf;
}

bool Np3dp::compute_angular_coordinate()
{
  switch (method) {
    case Stripe:
    {
      stripe_mesh.parameterize();
      // todo: export data for display and set flag below;
//      has_ac = true;
      break;
    }
    case GeodFolie:
    {
      break;
    }
    default:
      break;
  }
  return has_ac;

}
