/* -----------------------------------------------------------------------------
** Copyright (c) 2012 Advanced Resources and Risk Technology, LLC
** All rights reserved.
**
** This file is part of Advanced Resources and Risk Technology, LLC (AR2TECH) 
** version of the open source software sgems.  It is a derivative work by 
** AR2TECH (THE LICENSOR) based on the x-free license granted in the original 
** version of the software (see notice below) and now sublicensed such that it 
** cannot be distributed or modified without the explicit and written permission 
** of AR2TECH.
**
** Only AR2TECH can modify, alter or revoke the licensing terms for this 
** file/software.
**
** This file cannot be modified or distributed without the explicit and written 
** consent of AR2TECH.
**
** Contact Dr. Alex Boucher (aboucher@ar2tech.com) for any questions regarding
** the licensing of this file/software
**
** The open-source version of sgems can be downloaded at 
** sourceforge.net/projects/sgems.
** ----------------------------------------------------------------------------*/



/**********************************************************************
** Author: Nicolas Remy
** Copyright (C) 2002-2004 The Board of Trustees of the Leland Stanford Junior
**   University
** All rights reserved.
**
** This file is part of the "grid" module of the Geostatistical Earth
** Modeling Software (GEMS)
**
** This file may be distributed and/or modified under the terms of the 
** license defined by the Stanford Center for Reservoir Forecasting and 
** appearing in the file LICENSE.XFREE included in the packaging of this file.
**
** This file may be distributed and/or modified under the terms of the
** GNU General Public License version 2 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
** See http://www.gnu.org/copyleft/gpl.html for GPL licensing information.
**
** Contact the Stanford Center for Reservoir Forecasting, Stanford University
** if any conditions of this licensing are not clear to you.
**
**********************************************************************/

#ifndef __GSTLAPPLI_GRID_RGRID_H__ 
#define __GSTLAPPLI_GRID_RGRID_H__ 
 
#include <grid/common.h>
#include <grid/rgrid_geometry.h> 
#include <grid/grid_topology.h> 
#include <grid/grid_property_manager.h> 
#include <grid/grid_region_manager.h> 
#include <grid/grid_property_set.h> 
#include <grid/sgrid_cursor.h> 
#include <grid/rgrid_gval_accessor.h> 
#include <grid/gval_iterator.h> 
#include <grid/strati_grid.h> 
#include <math/gstlpoint.h> 
#include <math/random_numbers.h> 
 
#include <vector> 
#include <algorithm> 
#include <map> 
 
 
 
class Geovalue; 
//typedef  std::vector<bool> GsTLGridRegionFlags; 
 
 
 
/** A RGrid is a regular grid, ie a grid with a cartesian topology. 
 * The RGrid can take different geometries. When an RGrid is constructed, 
 * it doesn't have a geometry. A new geometry must be allocated and passed 
 * to the RGrid. The RGrid will take care of deleting its geometry when 
 * it is destroyed.  
 * 
 * The RGrid has hooks to support faults, but this functionality is not 
 * fully implemented yet.  
 */ 
class GRID_DECL RGrid : public Strati_grid { 

public: 
   
 
  RGrid();
  RGrid( std::string name );
  virtual ~RGrid(); 
   
  virtual std::string classname() const { return "Rgrid"; }  
 
  const RGrid_geometry* geometry() const { return geom_; } 
  virtual void set_geometry(RGrid_geometry* geom); 
   
  RGrid_gval_accessor* accessor(); 
  void set_accessor(RGrid_gval_accessor* accessor); 
   
  /** Provides access to the \c Sgrid_cursor used by the grid.
  */
  virtual const SGrid_cursor* cursor() const; 
  virtual SGrid_cursor* cursor(); 
  virtual void set_cursor(SGrid_cursor cursor); 
 
  RGrid_cell_connection* cell_connection(); 
 
   
 
  // Dimensions 
  virtual GsTLInt nx() const; 
  virtual GsTLInt ny() const; 
  virtual GsTLInt nz() const; 
  virtual GsTLInt size() const;
  GsTLInt nxyz(); 

  //angles
  virtual float rotation_z() const {return geom_->rotation_z(); }
  virtual void set_rotation_z(float angle_z) {geom_->set_rotation_z(angle_z); }
 
  // Misc. 
  virtual void set_level( int level); 
  virtual int current_level() const; 
   
  virtual GsTLInt node_id ( GsTLInt index ) const {  
    return grid_cursor_->node_id( index );  
  } 
 
  virtual GsTLInt closest_node( const location_type& P ) const {  
    appli_warning( "not yet implemented" ); 
    return -1; 
  } 
 
 
  // Property management 
  virtual Grid_continuous_property* add_property( const std::string& name );
  virtual Grid_continuous_property* add_property_from_disk( const std::string& name,
																										const std::string& filename );

  virtual Grid_weight_property* add_weight_property( const std::string& name );
  virtual Grid_weight_property* add_weight_property_from_disk( 
          const std::string& name,const std::string& filename );

  virtual Grid_categorical_property* add_categorical_property(
		  const std::string& name,
		  const std::string& definition_name="Default");
  virtual Grid_categorical_property* add_categorical_property_from_disk(
		  const std::string& name,
		  const std::string& filename,
		  const std::string& definition_name="Default");

  virtual bool remove_property( const std::string& name ) ; 
  virtual Grid_continuous_property* select_property( const std::string& name ); 
  virtual const Grid_continuous_property* selected_property() const ; 
  virtual Grid_continuous_property* selected_property() ; 
  virtual std::list<std::string> property_list() const; 
  virtual std::list<std::string> categorical_property_list() const; 
  virtual std::list<std::string> weight_property_list() const;

  virtual const Grid_continuous_property* property( const std::string& name ) const; 
  virtual Grid_continuous_property* property( const std::string& name ); 

  virtual const Grid_categorical_property* categorical_property( const std::string& name ) const;
  virtual Grid_categorical_property* categorical_property( const std::string& name );

  virtual const Grid_weight_property* weight_property( const std::string& name ) const;
  virtual Grid_weight_property* weight_property( const std::string& name );

  virtual MultiRealization_property*  
    add_multi_realization_property( const std::string& name ); 
 

  //--------------------------- 
  // PropertyGroup management 
  virtual GsTLGridPropertyGroup* add_group( const std::string& name, const std::string& type  );   
  virtual std::list<std::string> get_group_names(const std::string& type = "") const;   
  virtual unsigned int group_size() const;   
  virtual GsTLGridPropertyGroup* get_group( const std::string& name );   
  virtual const GsTLGridPropertyGroup* get_group( const std::string& name ) const;
  virtual void remove_group( const std::string& name );

  // Region  managment
  virtual const Grid_region* region(const std::string& name) const;
  virtual Grid_region* region(const std::string& name); 
  virtual Grid_region* add_region(const std::string& name); 
  virtual bool remove_region(const std::string& name); 
  virtual bool select_region(const std::string& region_name); 
  //virtual bool select_region(const std::vector<std::string>& region_names);

  virtual const Grid_region* selected_region() const; 
  virtual Grid_region* selected_region();
  virtual void unselect_region();
  virtual std::list<std::string> region_list() const;

  virtual bool is_inside_selected_region(int node_id) const;
 
  // Direct access     
  virtual Geovalue geovalue( GsTLInt gindex ); 
  virtual Geovalue geovalue( GsTLInt i, GsTLInt j, GsTLInt k ); 
  virtual Geovalue geovalue( const GsTLGridNode& gn ); 
   
  virtual bool is_informed( GsTLInt i,  GsTLInt j, GsTLInt k ); 
  virtual bool is_informed( const GsTLGridNode& gn ); 
  virtual bool is_informed( int index, int prop_id ) ; 
 
  virtual bool is_valid( GsTLInt node_id ); 
 
  virtual bool contains( GsTLInt i, GsTLInt j, GsTLInt k ); 
  virtual bool contains( const GsTLGridNode& gn ); 
  //  bool contains( const GsTLPoint& gn ); 
     
  virtual location_type location( int index ) const; 
  virtual location_type location( int i, int j, int k ) const; 
 
 
  // neighborhood definition 
  virtual Neighborhood* neighborhood( double x, double y, double z, 
				      double ang1, double ang2, double ang3, 
				      const Covariance<location_type>* cov=0, 
				      bool only_harddata = false,
              const Grid_region* region = 0,
              Coordinate_mapper* coord_mapper=0); 
  virtual Neighborhood* neighborhood( const GsTLTripletTmpl<double>& dim, 
				      const GsTLTripletTmpl<double>& angles, 
				      const Covariance<location_type>* cov=0, 
				      bool only_harddata = false,
              const Grid_region* region = 0,
              Coordinate_mapper* coord_mapper=0); 
  virtual Window_neighborhood* window_neighborhood( const Grid_template& templ ); 
   
 
  // Iterators 
   
  virtual iterator begin( Grid_continuous_property* prop = 0 ); 
  virtual iterator end( Grid_continuous_property* prop = 0 ); 
  virtual const_iterator begin( const Grid_continuous_property* prop = 0 ) const; 
  virtual const_iterator end( const Grid_continuous_property* prop = 0 ) const; 
  virtual void init_random_path( bool from_scratch = true ); 

  /** Returns a random path to the "begining" of the selected property. 
  * Be careful that in the current implementation, the path visits all 
  * the nodes of the grid, whether they are informed or not.
  */
  virtual random_path_iterator random_path_begin( Grid_continuous_property* prop = 0 ); 

  /** Returns a random path to the "end" of the selected property. 
  * Be careful that in the current implementation, the path visits all 
  * the nodes of the grid, whether they are informed or not.
  */
  virtual random_path_iterator random_path_end( Grid_continuous_property* prop = 0 ); 
   
 
  //TL modified 
  bool reNameProperty(std::string oldName, std::string newName);

// GsTL_object_item implementation
  virtual QString item_type() const;
  virtual GsTL_object_item *child(int row);
  virtual const GsTL_object_item *child(int row) const;
  virtual int childCount() const;
  virtual int columnCount() const;
  virtual QVariant item_data(int column) const;

  virtual GsTL_object_item* regions_item(){
	  return &region_manager_;
  }


 protected: 
  void update_topology(); 

  void clear_selected_region_from_property();
 
//  typedef std::map< std::string, GsTLGridRegionFlags* > Region_map; 
 
 protected: 
  RGrid_geometry* geom_; 
  //GsTLGridTopology* cell_connection_; 
  RGrid_cell_connection* cell_connection_;
  bool connection_is_updated_; 
 
  Grid_property_group_manager group_manager_;
  Grid_region_manager region_manager_;
  Grid_property_manager property_manager_; 
  
  
 
//  std::map<std::string, GsTLGridRegionFlags*> regions_; 
  RGrid_gval_accessor* accessor_; 
  SGrid_cursor* grid_cursor_; 
 
 
 protected: 
  std::vector<GsTLInt> grid_path_; 
}; 
 
 
//=========================== 
// creation function 
Named_interface* create_Rgrid( std::string& ); 
 
 
 
 
/*__________________________ INLINE FUNCTIONS __________________ */ 
 
 
 
 
inline 
GsTLInt RGrid::nx() const { 
  if (geom_) return geom_->dim(0); else return 0; 
} 
 
inline 
GsTLInt RGrid::ny() const { 
  if (geom_) return geom_->dim(1); else return 0; 
} 
 
inline 
GsTLInt RGrid::nz() const { 
  if (geom_) return geom_->dim(2); else return 0; 
} 
 
inline 
GsTLInt RGrid::nxyz() { 
  return nx()*ny()*nz(); 
} 

inline
GsTLInt RGrid::size() const { 
  return geom_->size(); 
} 

 
inline  
RGrid_gval_accessor* RGrid::accessor() { 
  return accessor_; 
} 
 
 
inline  
const SGrid_cursor* RGrid::cursor() const { 
  return grid_cursor_; 
} 


inline  
SGrid_cursor* RGrid::cursor() { 
  return grid_cursor_; 
} 


inline 
void RGrid::set_cursor(SGrid_cursor cursor) { 
  delete grid_cursor_;
  grid_cursor_ = new SGrid_cursor(cursor);
} 
 
/* 
inline 
RGrid_geometry* RGrid::geometry() { 
  return geom_; 
} 
*/ 
 
 
 
inline 
RGrid_cell_connection* RGrid::cell_connection() { 
  update_topology(); 
  return cell_connection_; 
} 
 
inline 
void RGrid::update_topology() { 
  if( connection_is_updated_ ) { 
    return; 
  } 
     
  if( geom_ == 0 ) { 
    delete cell_connection_; 
    cell_connection_ = 0; 
    connection_is_updated_ = false; 
    return; 
  } 
     
  if( cell_connection_ != 0 ) { 
    delete cell_connection_; 
  } 
  cell_connection_ = new RGrid_cell_connection(geom_); 
  connection_is_updated_ = true; 
} 
 
inline 
bool RGrid::contains(GsTLInt i, GsTLInt j, GsTLInt k) { 
  return (grid_cursor_->check_triplet(i, j, k)); 
} 
 
inline 
bool RGrid::contains(const GsTLGridNode& gn) { 
  return (grid_cursor_->check_triplet(gn[0], gn[1], gn[2])); 
} 
 
 
inline 
void  RGrid::set_level( int level) { 
  grid_cursor_->set_multigrid_level(level); 
} 
 
inline 
int RGrid::current_level() const { 
  return grid_cursor_->multigrid_level(); 
} 
 
inline  
const Grid_continuous_property* RGrid::property( const std::string& name ) const { 
  return property_manager_.get_property( name ); 
} 
 
inline  
Grid_continuous_property* RGrid::property( const std::string& name ) { 
  return property_manager_.get_property( name ); 
} 
 
inline
const Grid_categorical_property* RGrid::categorical_property( const std::string& name ) const{
	 return property_manager_.get_categorical_property( name );
}

inline
Grid_categorical_property* RGrid::categorical_property( const std::string& name ){
	return property_manager_.get_categorical_property( name );
}

inline
const Grid_weight_property* RGrid::weight_property( const std::string& name ) const{
	 return property_manager_.get_weight_property( name );
}

inline
Grid_weight_property* RGrid::weight_property( const std::string& name ){
	return property_manager_.get_weight_property( name );
}

inline const Grid_continuous_property* RGrid::selected_property() const { 
  return property_manager_.selected_property(); 
} 
 
inline Grid_continuous_property* RGrid::selected_property() { 
  return property_manager_.selected_property(); 
} 
 
 
inline 
Grid_region* RGrid::add_region(const std::string& name) { 
  return region_manager_.add_region(name); 
} 
 
inline 
bool RGrid::remove_region(const std::string& name) {

  bool is_selected_property = false;  
  if(selected_region() != NULL) {
    is_selected_property= selected_region()->name() == name;
  }
  bool ok =  region_manager_.remove_region(name);
  if(!ok) return false;

  if( is_selected_property )  
    clear_selected_region_from_property();
  return true; 

} 
 
inline 
Grid_region* RGrid::region(const std::string& name) { 
  return region_manager_.get_region(name);
} 

inline 
const Grid_region* RGrid::region(const std::string& name) const { 
  return region_manager_.get_region(name);
} 

inline const Grid_region* RGrid::selected_region() const { 
  return region_manager_.selected_region(); 
} 
 
inline Grid_region* RGrid::selected_region() { 
  return region_manager_.selected_region(); 
}

inline bool RGrid::select_region(const std::string& region_name) {
  Grid_region* region = region_manager_.select_region( region_name );
  if( region == NULL && !region_name.empty() ) return false;
  Grid_property_manager::Property_name_iterator it = property_manager_.names_begin();
  for(; it != property_manager_.names_end(); ++it) {
    Grid_continuous_property* prop = property_manager_.get_property( *it );
    prop->set_region(region);
  }
  return true;
} 

inline void RGrid::unselect_region() {
  region_manager_.unselect_region();
  clear_selected_region_from_property();
}

inline bool RGrid::is_inside_selected_region(int node_id) const {
  const Grid_region* region = region_manager_.selected_region();
  // When no region is selected, use the full grid
  if( !region ) return true;  
  return region->is_inside_region(node_id);
}


inline 
Geovalue RGrid::geovalue( GsTLInt gindex ) { 
  int i,j,k; 
  grid_cursor_->coords( gindex, i,j,k ); 
  accessor_->set_geovalue( gindex, geom_->coordinates( i,j,k ) ); 
  return (*accessor_->node()); 
} 
 
 
inline 
Geovalue RGrid::geovalue( GsTLInt i, GsTLInt j, GsTLInt k ) { 
  accessor_->set_geovalue( grid_cursor_->node_id( i,j,k ), 
			   geom_->coordinates( i,j,k ) ); 
  return (*accessor_->node()); 
} 
 
 
inline 
Geovalue RGrid::geovalue(const GsTLGridNode& gn ) { 
  return RGrid::geovalue( gn[0], gn[1], gn[2] ); 
} 
 
 
inline 
bool RGrid::is_valid( GsTLInt node_id ) { 
  return (node_id >= 0) && (node_id < geom_->size() ); 
} 
 
inline 
bool RGrid::is_informed( GsTLInt i,  GsTLInt j, GsTLInt k ) { 
  int ind = grid_cursor_->node_id( i,j,k ); 
  if ( ind < 0 )  
    return false; 
 
  return property_manager_.selected_property()->is_informed( ind ); 
 
} 
 
inline 
bool RGrid::is_informed( const GsTLGridNode& p ) { 
  return RGrid::is_informed( p[0], p[1], p[2] ); 
} 
 
inline  
bool RGrid::is_informed( int node_id, int prop_id ) { 
  if ( is_valid( node_id ) )  
    return false; 
 
  return property_manager_.get_property( prop_id )->is_informed( node_id ); 
} 
 
 
inline RGrid::location_type 
RGrid::location( int node_id ) const { 
  
  // the Sgrid Cursor approach does not work with
  // Multiple grid as the resulting ijk will be
  // in the current grid.
  //int i,j,k;
  //grid_cursor_->coords( node_id, i,j,k);
  GsTLInt max_nxy = geom_->dim(0)*geom_->dim(1);
	GsTLInt inxy = node_id % max_nxy; 
	GsTLInt k = (node_id - inxy)/max_nxy; 
	GsTLInt j = (inxy - node_id%geom_->dim(0))/geom_->dim(0); 
	GsTLInt i = inxy%geom_->dim(0);

  //This is still potentially faulty for 3D grid as the z may be in
  // Stratigraphic coordinates, see manual
  return geom_->coordinates( i,j,k ); 
} 
 
 
inline RGrid::location_type 
RGrid::location( int i, int j, int k ) const { 
  return geom_->coordinates( i,j,k ); 
} 
 
 
inline 
RGrid::iterator RGrid::begin( Grid_continuous_property* prop ) { 
  Grid_continuous_property* property = prop;
  if( !prop )
    property = property_manager_.selected_property();

  return iterator( this, property, 0, grid_cursor_->max_index(),  
		               LinearMapIndex() ); 
} 
 
inline 
RGrid::iterator RGrid::end( Grid_continuous_property* prop ) { 
  Grid_continuous_property* property = prop;
  if( !prop )
    property = property_manager_.selected_property();

  return iterator( this, property,
               	   grid_cursor_->max_index(), grid_cursor_->max_index(), 
	                 LinearMapIndex() ); 
} 
 
inline 
RGrid::const_iterator RGrid::begin( const Grid_continuous_property* prop ) const { 
  const Grid_continuous_property* property = prop;
  if( !prop )
    property = property_manager_.selected_property();

  return const_iterator( this, property, 0, grid_cursor_->max_index(),  
		               LinearMapIndex() ); 
} 
 
inline 
RGrid::const_iterator RGrid::end( const Grid_continuous_property* prop ) const { 
  const Grid_continuous_property* property = prop;
  if( !prop )
    property = property_manager_.selected_property();

  return const_iterator( this, property,
               	   grid_cursor_->max_index(), grid_cursor_->max_index(), 
	                 LinearMapIndex() ); 
} 
 
 
inline RGrid::random_path_iterator 
RGrid::random_path_begin( Grid_continuous_property* prop ) { 
  if( int(grid_path_.size()) != grid_cursor_->max_index() )  
    init_random_path(); 
 
  Grid_continuous_property* property = prop;
  if( !prop )
    property = property_manager_.selected_property();

  return random_path_iterator( this, property, 
	                  		       0, grid_cursor_->max_index(),  
		                           TabularMapIndex(&grid_path_) ); 
} 
 
inline RGrid::random_path_iterator 
RGrid::random_path_end( Grid_continuous_property* prop ) { 
  if( int(grid_path_.size()) != grid_cursor_->max_index() )  
    init_random_path(); 
 
  Grid_continuous_property* property = prop;
  if( !prop )
    property = property_manager_.selected_property();

  return random_path_iterator( this, property, 
			       grid_path_.size(),  
			       grid_path_.size(), 
			       TabularMapIndex(&grid_path_) ); 
} 
 


inline GsTLGridPropertyGroup* 
RGrid::add_group( const std::string& name, const std::string& type ) {
  return group_manager_.add_group(name,type);
}

inline std::list<std::string> 
RGrid::get_group_names(const std::string& type) const {
  return group_manager_.group_names(type);
}

inline unsigned int RGrid::group_size() const {
  return group_manager_.size();
}

inline GsTLGridPropertyGroup* 
RGrid::get_group( const std::string& name ){
  return group_manager_.get_group(name);
}
inline const GsTLGridPropertyGroup* 
RGrid::get_group( const std::string& name ) const{
  return group_manager_.get_group(name);
}
 
inline void
RGrid::remove_group( const std::string& name ){
	group_manager_.remove_group(name);
}

#endif 
 
 
