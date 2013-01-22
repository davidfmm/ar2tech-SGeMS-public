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
** Author: Jianbing Wu, Alexandre Boucher
**
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

#ifndef __filtersim_is_categorical_H__
#define __filtersim_is_categorical_H__

// c/c++
#include <iterator>  // for distance
#include <set>  // for set
#include <cmath> // for ceil and floor

#include <GsTLAppli/grid/grid_model/grid_property.h>    // for GsTLGridProperty


bool is_number( const std::string& str ) 
{
    for( std::string::const_iterator it = str.begin() ; it != str.end() ; ++it ) 
    {
        if( !isdigit( *it ) && *it != '.' && *it != '-' && *it != '+' ) return false;
    }

    return true;
}


template <typename T> inline T round(const T& _value) 
{
    return ( _value >= static_cast<T>( 0 ) )
        ? static_cast<T>( std::floor( _value + 0.5 ) )
        : static_cast<T>( std::ceil( _value - 0.5 ) );
} 

// --- is_categorical ---------------------------------------------------------
template <class T>  inline bool is_categorical(T value) 
{   
    if ( ( value < 0 ) || ( value != round( value ) ) ) 
        return false;
    else
        return true;
} // is_categorical


// --- check whether the current property is integer ----------------------------
bool is_integer_prop(GsTLGridProperty* prop, int& nb_cat)
{
	std::set<int> categories;

    for (int i=0; i<prop->size(); i++)
    {
        if ( prop->is_informed(i) )
        {
            if ( !is_categorical( prop->get_value(i) ) )
                return false;
            else
                categories.insert( prop->get_value(i) );
        }
    }

    // total number of categories
    nb_cat = categories.size();

    return true;
}

// --- check whether the current property is indicator  -------------------------
// a valid indicator must be integer, and gradually increase from 0
// the maximun indicator must be "nb_cat-1"
bool is_indicator_prop(GsTLGridProperty* prop, int& nb_cat)
{

    int i, value;
	std::set<int> categories;

    for (i=0; i<prop->size(); i++)
    {
        if ( prop->is_informed(i) )
        {
            if ( !is_categorical( prop->get_value(i) ) )
                return false;
            else
                categories.insert( prop->get_value(i) );
        }
    }
    
    for (i=0; i<prop->size(); i++)
    {
        if ( prop->is_informed(i) )
        {
            value = prop->get_value(i);
            if ( value != std::distance(categories.begin(), categories.find(value) ) )
                return false;
        }
    }

    nb_cat = categories.size();

    return true;
}


#endif // __filtersim_is_categorical_H__
