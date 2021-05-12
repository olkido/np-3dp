#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/imgui/ImGuiMenu.h>
#include <sstream>
#include "Np3dp.hpp"

Np3dp np3dp;

// The Viewer
igl::opengl::glfw::Viewer viewer;
// The (optional) viewer menu
igl::opengl::glfw::imgui::ImGuiMenu menu;

// display options
// show vector field
bool show_vf = false;
// show angular_coordinate
bool show_ac = false;


void update_display();
void display_vf();

// invalidate all computed data
void invalidate_data();

// This function is called every time a keyboard button is pressed
bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier);

// This function draws the menu boxes
void draw_menu_boxes();


bool key_down(igl::opengl::glfw::Viewer& viewer, unsigned char key, int modifier)
{
  if (key == 'v' || key == 'V' )
  {
    show_vf = !show_vf;
    update_display();
  }
  else if (key == 'a' || key == 'A')
  {
    show_ac = !show_ac;
    update_display();
  }
  
  return false;
}

void draw_menu_boxes()
{
  // Enable the ImGui plugin to show text labels and menu boxes.
  viewer.plugins.push_back(&menu);

  
  // Customize the menu
  // Add content to the default menu window
  menu.callback_draw_viewer_menu = [&]()
  {
    // Draw parent menu content
    menu.draw_viewer_menu();

    // Add new group
    if (ImGui::CollapsingHeader("Np-3DP", ImGuiTreeNodeFlags_DefaultOpen))
    {

      // Expose method enumeration type via custom callback
      if (ImGui::Combo("Method", (int *)(&np3dp.method), "Stripe Patterns\0Geodesic Foliations\0"))
      {
        invalidate_data();
      };

      // Expose method enumeration type via custom callback
      if (ImGui::Combo("InputVF", (int *)(&np3dp.vftype), "Radial\0Libdirectional\0"))
      {
        invalidate_data();
      };

      // Add a button
      if (ImGui::Button("VF Initialize", ImVec2(-1,0)))
      {
        np3dp.initialize_vector_field();
        update_display();
      }

      // Add a button
      if (ImGui::Button("AC Compute", ImVec2(-1,0)))
      {
        np3dp.compute_angular_coordinate();
        update_display();
      }

      // Toggle vector field display
      if (ImGui::Checkbox("(V) Show Vector Field", &show_vf))
      {
        update_display();
      }

      // Toggle angular coordinate display
      if (ImGui::Checkbox("(A) Show Angular Coordinate", &show_ac))
      {
        update_display();
      }

    }
  };

}




int main(int argc, char *argv[])
{

  // Load a np3dp in OFF/OBJ format
  if (argc <2 )
  {
    std::cout << "Need to provide np3dp filename as first argument."<< std::endl;
    return 0;
  }

  std::string meshfile = std::string(argv[1]);
  if (!np3dp.read(meshfile))
  {
    std::cout << "Could not read np3dp from "<<meshfile<< std::endl;
    return 0;
  }

  // Add the keyboard interaction
  viewer.callback_key_down = &key_down;

  // Plot the np3dp
  update_display();
  
  // Add menu boxes
  draw_menu_boxes();


  // Launch the viewer
  viewer.launch();
  
}



void update_display()
{
  viewer.data().clear(); // this clears all data, including overlays
  viewer.data().set_mesh(np3dp.V, np3dp.F); // set the np3dp to display
  // olga's color preferences :)
  viewer.data().set_colors(.99*Eigen::RowVector3d::Ones());
  viewer.core().background_color<<1.,1.,1.,1.;
  viewer.data().line_color<<173./255,174./255,103./255,1.;
  
  if (show_vf && np3dp.has_vf)
  {
    display_vf();
  }

  if (show_ac && np3dp.has_ac)
  {
    //todo: show angular coordinate
  }

}

// invalidate all computed data
void invalidate_data()
{
  np3dp.invalidate();
  update_display();
}

void display_vf()
{
  double r = 0.75 * np3dp.avg_edge_length;
  Eigen::RowVector3d color; color.setZero();
  switch (np3dp.method) {
    case Np3dp::Stripe:
    {
      // show per vertex vf
      for (unsigned vi=0;vi<np3dp.V.rows(); ++vi)
        viewer.data().add_edges
        (
         np3dp.V.row(vi),
         np3dp.V.row(vi) + r*np3dp.vvf.row(vi),
         color
        );
      break;
    };
    case Np3dp::GeodFolie:
    {
      //todo: show per face vf
      break;
    };
    default:
      break;
  }
  


}
