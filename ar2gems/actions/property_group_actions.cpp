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



#include <actions/property_group_actions.h>
#include <actions/defines.h>
#include <utils/string_manipulation.h>
#include <utils/error_messages_handler.h>
#include <appli/manager_repository.h>
#include <appli/project.h>
#include <grid/geostat_grid.h> 
#include <grid/grid_property.h>
#include <grid/grid_property_set.h>


/**
* New_property_group
*/

Named_interface* New_property_group::create_new_interface( std::string& ){
  return new New_property_group;
}
   

bool New_property_group::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
      String_Op::decompose_string( parameters, Actions::separator,
				   Actions::unique );

  if( params.size() < 2 ) {
    errors->report( "Must have at least 2 parameters, name of the grid and name the group" );
    return false;
  }

  // Get the grid
  std::string grid_name = params[0];
  SmartPtr<Named_interface> ni = Root::instance()->interface( gridModels_manager + "/" + grid_name);
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>( ni.raw_ptr() );
  if(!grid)  {
    errors->report( "The grid "+params[0]+" does not exist" );
    return false;
  }

  GsTLGridPropertyGroup* group = grid->get_group(params[1]);
  if(group)  {
    errors->report( "The goup "+params[1]+" already exist; hence cannot be created" );
    return false;
  }

  std::string type = "";
  if( params.size() == 3 ) {
    if( params[2] == "General" ) type = "";
    else type = params[2];
  }

  group = grid->add_group(params[1],type);
  if(!group)  {
    errors->report( "The goup "+params[1]+" could no be created; possibly type undefined" );
    return false;
  }

  for(int i=3; i< params.size(); i++) {
    Grid_continuous_property* prop = grid->property(params[i]);
    if(prop == NULL)  {
      errors->report( "The property "+params[i]+" does not exist" );
      return false;
    }
  }

  for(int i=3; i< params.size(); i++) {
    group->add_property(grid->property(params[i]));
  }

  proj->update();

  return true;
}

bool New_property_group::exec(){
  return true;
}
 

/**
* Add_properties_to_group::
*/
Named_interface* Add_properties_to_group::create_new_interface( std::string& ){
  return new Add_properties_to_group;
}

 
bool Add_properties_to_group::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
      String_Op::decompose_string( parameters, Actions::separator,
				   Actions::unique );

  if( params.size() < 3 ) {
    errors->report( "Must have at least 3 parameters, name of the grid and name the group and at least one property" );
    return false;
  }

  // Get the grid
  SmartPtr<Named_interface> ni = Root::instance()->interface( gridModels_manager + "/" + params[0] );
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>( ni.raw_ptr() );
  if(!grid)  {
    errors->report( "The grid "+params[0]+" does not exist" );
    return false;
  }

  GsTLGridPropertyGroup* group = grid->get_group(params[1]);
  if(!group)  {
    errors->report( "The goup "+params[1]+" does not exist" );
    return false;
  }

  for(int i=2; i< params.size(); i++) {
    Grid_continuous_property* prop = grid->property(params[i]);
    if(prop == NULL)  {
      errors->report( "The property "+params[i]+" does not exist" );
      return false;
    }
  }

  for(int i=2; i< params.size(); i++) {
    if( !group->is_member_property( params[i] ) )
      group->add_property(grid->property(params[i]));
  }

  return true;
}
bool Add_properties_to_group::exec(){

  return true;
}


/*---------------------------*/

/**
* Add_properties_to_group::
*/
Named_interface* Remove_properties_from_group::create_new_interface( std::string& ){
  return new Remove_properties_from_group;
}

 
bool Remove_properties_from_group::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){

  std::vector< std::string > params = 
      String_Op::decompose_string( parameters, Actions::separator,
				   Actions::unique );

  if( params.size() < 3 ) {
    errors->report( "Must have at least 3 parameters, name of the grid and name the group and at least one property" );
    return false;
  }

  // Get the grid
  SmartPtr<Named_interface> ni = Root::instance()->interface( gridModels_manager + "/" + params[0] );
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>( ni.raw_ptr() );
  if(!grid)  {
    errors->report( "The grid "+params[0]+" does not exist" );
    return false;
  }

  GsTLGridPropertyGroup* group = grid->get_group(params[1]);
  if(!group)  {
    errors->report( "The goup "+params[1]+" does not exist" );
    return false;
  }

  for(int i=2; i< params.size(); i++) {
    Grid_continuous_property* prop = grid->property(params[i]);
    if(prop == NULL)  {
      errors->report( "The property "+params[i]+" does not exist" );
      return false;
    }
  }

  for(int i=2; i< params.size(); i++) {
    if( group->is_member_property( params[i] ) )
      group->remove_property(grid->property(params[i]));
  }

  return true;
}
bool Remove_properties_from_group::exec(){

  return true;
}


/*-------------------*/



Named_interface*
Delete_property_in_group::create_new_interface( std::string& ){
	return new Delete_property_in_group;
}

bool Delete_property_in_group::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){
  std::vector< std::string > params =
      String_Op::decompose_string( parameters, Actions::separator,
				   Actions::unique );

  if( params.size() != 2 ) {
    errors->report( "Must have 2 parameters, name of the grid and name the group to be deleted" );
    return false;
  }

  // Get the grid
  SmartPtr<Named_interface> ni = Root::instance()->interface( gridModels_manager + "/" + params[0] );
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>( ni.raw_ptr() );
  if(!grid)  {
    errors->report( "The grid "+params[0]+" does not exist" );
    return false;
  }

  GsTLGridPropertyGroup* group = grid->get_group(params[1]);
  if(!group)  {
    errors->report( "The goup "+params[1]+" does not exist" );
    return false;
  }

  GsTLGridPropertyGroup::property_map::iterator it = group->begin_property();
  for(; it != group->end_property(); ++it){
  	grid->remove_property(it->second->name());
  }
  grid->remove_group(params[1]);
  return true;
}

bool Delete_property_in_group::exec(){
	return true;

}

/*
 *  --------------------------------
 */


Named_interface*
Remove_group::create_new_interface( std::string& ){
	return new Remove_group;
}

bool Remove_group::init( std::string& parameters, GsTL_project* proj,
                     Error_messages_handler* errors ){
  std::vector< std::string > params =
      String_Op::decompose_string( parameters, Actions::separator,
				   Actions::unique );

  if( params.size() != 2 ) {
    errors->report( "Must have 2 parameters, name of the grid and name the group to be removed" );
    return false;
  }

  // Get the grid
  SmartPtr<Named_interface> ni = Root::instance()->interface( gridModels_manager + "/" + params[0] );
  Geostat_grid* grid = dynamic_cast<Geostat_grid*>( ni.raw_ptr() );
  if(!grid)  {
    errors->report( "The grid "+params[0]+" does not exist" );
    return false;
  }

  GsTLGridPropertyGroup* group = grid->get_group(params[1]);
  if(!group)  {
    errors->report( "The group "+params[1]+" does not exist" );
    return false;
  }

  grid->remove_group(params[1]);
  return true;
}

bool Remove_group::exec(){
	return true;

}

